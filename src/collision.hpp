#pragma once
#include "geomerty.hpp"
#include <algorithm>
#include <cmath>

namespace CollisionHandler {
    struct CollisionResponse {
        bool isIntersecting;
        // In case ima add anything in the future...
    };

    namespace Test {
        CollisionResponse CircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) {
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

    } // namespace Test

    namespace Solve {
        void solveCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) {
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

            circle1.x -= correction.x * circle1.invMass;
            circle1.y -= correction.y * circle1.invMass;

            // Multiply by boolean to 0 out the corrections if it's false
            circle2.x += correction.x * circle2.invMass * circle2.isMoveable;
            circle2.y += correction.y * circle2.invMass * circle2.isMoveable;
        }
    } // namespace Solve

    static CollisionResponse (*collisionMap[1][1])(RedCannonBall::Entity&, RedCannonBall::Entity&) = {
        // Circle
        {
            // Circle
            [](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2) -> CollisionResponse {
                return Test::CircleCircle(circle1, circle2);
            },
        },
    };

    static void (*solveMap[2][2])(RedCannonBall::Entity&, RedCannonBall::Entity&, CollisionResponse&) = {
        // Circle
        {
            // Circle
            [](RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision) -> void {
                Solve::solveCircleCircle(circle1, circle2, collision);
            },
        },
    };
} // namespace CollisionHandler
