#pragma once
#include "collision.hpp"
#include "geomerty.hpp"
#include <algorithm>
#include <cmath>
#include <functional>

RedCannonBall::CollisionHandler::CollisionHandler(RedCannonBall::World& world):
    world(world) {
    // Collision testers
    collisionMap[Entity_Circle][Entity_Circle] = [this](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) -> CollisionResponse {
        return this->testCircleCircle(circle1, circle2);
    };

    // Collision solvers
    solveMap[Entity_Circle][Entity_Circle] = [this](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) -> void {
        solveCircleCircle(circle1, circle2, collision);
    };
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::test(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2) {
    return collisionMap[entity1.type][entity2.type](entity1, entity2);
}
void RedCannonBall::CollisionHandler::solve(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision) {
    solveMap[entity1.type][entity2.type](entity1, entity2, collision);
}

RedCannonBall::CollisionResponse RedCannonBall::CollisionHandler::testCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) {
    RedCannonBall::Vector2d delta = {
        circle2.x - circle1.x,
        circle2.y - circle1.y,
    };
    auto distanceSquared = delta.x * delta.x + delta.y * delta.y;
    auto radiusSum = circle1.radius + circle2.radius;

    if (distanceSquared <= radiusSum * radiusSum) {
        return CollisionResponse({true});
    }
    return CollisionResponse({false});
}

void RedCannonBall::CollisionHandler::solveCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) {
    auto restitution = std::min(circle1.restitution, circle2.restitution);
    auto friction = std::max(circle1.friction, circle2.friction);

    RedCannonBall::Vector2d delta = {
        circle2.x - circle1.x,
        circle2.y - circle1.y,
    };
    auto distanceSquared = delta.x * delta.x + delta.y * delta.y;
    auto distance = std::sqrt(distanceSquared);
    auto radiusSum = circle1.radius + circle2.radius;
    auto ratio = (distance + circle1.radius - circle2.radius) / (2 * distance);

    RedCannonBall::Vector2d normal = {delta.x / distance, delta.y / distance};
    RedCannonBall::Vector2d tangent = {-normal.y, normal.x};

    RedCannonBall::Vector2d relativeVelocity = {circle2.velocity.x - circle1.velocity.x, circle2.velocity.y - circle1.velocity.y};
    auto normalVelocity = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;
    auto tangentVelocity = relativeVelocity.x * tangent.x + relativeVelocity.y * tangent.y;

    RedCannonBall::Vector2d impulse = {
        normal.x * (1 + restitution) * normalVelocity,
        normal.y * (1 + restitution) * normalVelocity,
    };
    RedCannonBall::Vector2d frictionImpulse = {
        tangent.x * tangentVelocity * friction,
        tangent.y * tangentVelocity * friction,
    };

    impulse.x += frictionImpulse.x;
    impulse.y += frictionImpulse.y;

    auto impulseMag = std::sqrt(impulse.x * impulse.x + impulse.y * impulse.y);
    auto invMassSum = circle1.invMass + circle2.invMass;
    auto impulseMagScaled = impulseMag / invMassSum;

    RedCannonBall::Vector2d impulseScaled = {
        impulse.x / invMassSum,
        impulse.y / invMassSum,
    };

    circle1.velocity.x += impulseScaled.x * circle1.invMass;
    circle1.velocity.y += impulseScaled.y * circle1.invMass;
    circle2.velocity.x -= impulseScaled.x * circle2.invMass;
    circle2.velocity.y -= impulseScaled.y * circle2.invMass;

    RedCannonBall::PhysAttr overlap = std::max(RedCannonBall::PhysAttr(0.0), radiusSum - distance + RedCannonBall::PhysAttr(0.5f));
    RedCannonBall::Vector2d correction = {
        normal.x * overlap / invMassSum,
        normal.y * overlap / invMassSum,
    };

    // No need to add extra "*" for circle1, since it is impossible for it to be unmovable
    circle1.x -= correction.x * circle1.invMass;
    circle1.y -= correction.y * circle1.invMass;

    // Multiply by boolean to 0 out the corrections if it's false
    circle2.x += correction.x * circle2.invMass * circle2.isMoveable;
    circle2.y += correction.y * circle2.invMass * circle2.isMoveable;
}
