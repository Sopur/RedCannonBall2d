#pragma once
#include "collision.hpp"
#include "geomerty.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <raylib.h>

#define OVERLAP_OFFSET 0.5

RedCannonBall::CollisionResponse testCircleLine(RedCannonBall::Entity& circle, RedCannonBall::Line& line) {
    auto& A = line.a;
    auto& B = line.b;
    auto& C = circle;

    auto AC = C - A;
    auto AB = B - A;
    auto D = AC.proj(AB) + A;
    auto AD = D - A;
    auto k = std::abs(AB.x) > std::abs(AB.y) ? AD.x / AB.x : AD.y / AB.y;

    RedCannonBall::Vector2d closest;
    if (k <= 0.0) {
        closest = A;
    } else if (k >= 1.0) {
        closest = B;
    } else {
        closest = D;
    }

    return {closest.distance(circle) <= circle.radius, closest};
}

RedCannonBall::CollisionResponse testLineLine(RedCannonBall::Line& lineA, RedCannonBall::Line& lineB) {
    auto directionA =
        ((lineB.b.x - lineB.a.x) * (lineA.a.y - lineB.a.y) - (lineB.b.y - lineB.a.y) * (lineA.a.x - lineB.a.x)) /
        ((lineB.b.y - lineB.a.y) * (lineA.b.x - lineA.a.x) - (lineB.b.x - lineB.a.x) * (lineA.b.y - lineA.a.y));
    auto directionB =
        ((lineA.b.x - lineA.a.x) * (lineA.a.y - lineB.a.y) - (lineA.b.y - lineA.a.y) * (lineA.a.x - lineB.a.x)) /
        ((lineB.b.y - lineB.a.y) * (lineA.b.x - lineA.a.x) - (lineB.b.x - lineB.a.x) * (lineA.b.y - lineA.a.y));
    return {
        directionA >= 0 && directionA <= 1 && directionB >= 0 && directionB <= 1,
        RedCannonBall::Vector2d(lineA.a.x + directionA * (lineA.b.x - lineA.a.x), lineA.a.y + directionA * (lineA.b.y - lineA.a.y)),
    };
}

RedCannonBall::CollisionHandler::CollisionHandler(RedCannonBall::World& world):
    world(world) {
    // Collision testers
    collisionMap[Entity_Circle][Entity_Circle] = [this](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) -> CollisionResponse {
        return this->testCircleCircle(circle1, circle2);
    };
    collisionMap[Entity_Circle][Entity_Polygon] = [this](RedCannonBall::Entity& circle, RedCannonBall::Entity& polygon) -> CollisionResponse {
        return this->testPolygonCircle(polygon, circle);
    };

    collisionMap[Entity_Polygon][Entity_Circle] = [this](RedCannonBall::Entity& polygon, RedCannonBall::Entity& circle) -> CollisionResponse {
        return this->testPolygonCircle(polygon, circle);
    };
    collisionMap[Entity_Polygon][Entity_Polygon] = [this](RedCannonBall::Entity& polygon1, RedCannonBall::Entity& polygon2) -> CollisionResponse {
        return this->testPolygonPolygon(polygon1, polygon2);
    };

    // Collision solvers
    solveMap[Entity_Circle][Entity_Circle] = [this](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) -> void {
        solveCircleCircle(circle1, circle2, collision);
    };
    solveMap[Entity_Circle][Entity_Polygon] = [this](RedCannonBall::Entity& circle, RedCannonBall::Entity& polygon, CollisionResponse& collision) -> void {
        solvePolygonCircle(polygon, circle, collision);
    };

    solveMap[Entity_Polygon][Entity_Circle] = [this](RedCannonBall::Entity& polygon, RedCannonBall::Entity& circle, CollisionResponse& collision) -> void {
        solvePolygonCircle(polygon, circle, collision);
    };
    solveMap[Entity_Polygon][Entity_Polygon] = [this](RedCannonBall::Entity& polygon1, RedCannonBall::Entity& polygon2, CollisionResponse& collision) -> void {
        solvePolygonPolygon(polygon1, polygon2, collision);
    };
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::test(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2) {
    // Find the right code to test the two shapes
    return collisionMap[entity1.type][entity2.type](entity1, entity2);
}
void RedCannonBall::CollisionHandler::solve(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision) {
    // Find the right code to solve the two shapes
    solveMap[entity1.type][entity2.type](entity1, entity2, collision);
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::testCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) {
    Vector2d delta = circle2 - circle1;
    auto radiusSum = circle1.radius + circle2.radius;

    // Intersection point doesn't need to be calculated
    return CollisionResponse({delta.magnitudeSquared() <= radiusSum * radiusSum});
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::testPolygonCircle(RedCannonBall::Entity& polygon, RedCannonBall::Entity& circle) {
    for (auto& line : polygon.projectedLines) {
        auto collision = testCircleLine(circle, line);
        if (collision.isIntersecting) {
            return collision;
        }
    }
    return CollisionResponse({false});
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::testPolygonPolygon(RedCannonBall::Entity& polygon1, RedCannonBall::Entity& polygon2) {
    for (auto& lineA : polygon1.projectedLines) {
        for (auto& lineB : polygon2.projectedLines) {
            auto collision = testLineLine(lineA, lineB);
            if (collision.isIntersecting) {
                return collision;
            }
        }
    }

    return CollisionResponse({false});
}

void RedCannonBall::CollisionHandler::solveCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) {
    // Get restitution and friction
    auto restitution = std::min(circle1.restitution, circle2.restitution);
    auto friction = std::max(circle1.friction, circle2.friction);

    // Calculate velocity changes, no need to do math with rotation since they're circles
    Vector2d delta = circle2 - circle1;
    auto distance = delta.magnitude();
    auto radiusSum = circle1.radius + circle2.radius;
    auto ratio = (distance + circle1.radius - circle2.radius) / (2 * distance);

    Vector2d normal = delta / distance;
    Vector2d tangent = {-normal.y, normal.x};

    Vector2d relativeVelocity = circle2.velocity - circle1.velocity;
    auto normalVelocity = relativeVelocity.dot(normal);
    auto tangentVelocity = relativeVelocity.dot(tangent);

    Vector2d impulse = normal * ((1 + restitution) * normalVelocity);
    Vector2d frictionImpulse = tangent * (tangentVelocity * friction);

    impulse += frictionImpulse;

    auto impulseMag = impulse.magnitude();
    auto invMassSum = circle1.invMass + circle2.invMass;
    auto impulseMagScaled = impulseMag / invMassSum;

    Vector2d impulseScaled = impulse / invMassSum;

    // 0 out calculations if unmovable
    circle1.velocity += impulseScaled * circle1.invMass * circle1.isMoveable;
    circle2.velocity -= impulseScaled * circle2.invMass * circle2.isMoveable;

    // Fix positions
    RedCannonBall::PhysAttr overlap = std::max(RedCannonBall::PhysAttr(0.0), radiusSum - distance + RedCannonBall::PhysAttr(OVERLAP_OFFSET));
    Vector2d correction = normal * (overlap / invMassSum);

    circle1 -= correction * circle1.invMass * circle1.isMoveable;
    circle2 += correction * circle2.invMass * circle2.isMoveable;
}

void RedCannonBall::CollisionHandler::solvePolygonCircle(RedCannonBall::Entity& polygon, RedCannonBall::Entity& circle, CollisionResponse& collision) {
    // Get restitution and friction
    auto restitution = std::min(polygon.restitution, circle.restitution);
    auto friction = std::max(polygon.friction, circle.friction);

    // Calculate velocity changes with angular math
    Vector2d delta = circle - collision.intersection;
    auto distance = delta.magnitude();
    auto radius = circle.radius;

    Vector2d normal = delta / distance;
    Vector2d tangent = {-normal.y, normal.x};

    // Relative velocity
    Vector2d relativeVelocity = circle.velocity - polygon.velocity;
    relativeVelocity -= normal * Vector2d(polygon.angularVelocity).cross(normal);

    auto normalVelocity = relativeVelocity.dot(normal);
    auto tangentVelocity = relativeVelocity.dot(tangent);

    Vector2d impulse = normal * ((1 + restitution) * normalVelocity);
    Vector2d frictionImpulse = tangent * (tangentVelocity * friction);

    impulse += frictionImpulse;

    auto impulseMag = impulse.magnitude();
    auto invMassSum = circle.invMass + polygon.invMass;
    auto impulseMagScaled = impulseMag / invMassSum;

    Vector2d impulseScaled = impulse / invMassSum;

    // Angular impulse
    auto angularImpulse = (collision.intersection - polygon).cross(impulse);
    polygon.angularVelocity += angularImpulse * polygon.invInertia * polygon.isMoveable;

    // 0 out calculations if unmovable
    circle.velocity -= impulseScaled * circle.invMass * circle.isMoveable;
    polygon.velocity += impulseScaled * polygon.invMass * polygon.isMoveable;

    // Fix positions
    RedCannonBall::PhysAttr overlap = std::max(RedCannonBall::PhysAttr(0.0), radius - distance + RedCannonBall::PhysAttr(OVERLAP_OFFSET));
    Vector2d correction = normal * (overlap / invMassSum);

    circle += correction * circle.invMass * circle.isMoveable;
    polygon -= correction * polygon.invMass * polygon.isMoveable;
}

void RedCannonBall::CollisionHandler::solvePolygonPolygon(RedCannonBall::Entity& polygon1, RedCannonBall::Entity& polygon2, CollisionResponse& collision) {
    polygon1.x = -200;
    polygon1.y = -200;
}