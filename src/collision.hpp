#pragma once
#include "geomerty.hpp"
#include <functional>

namespace RedCannonBall {
    struct CollisionResponse {
        bool isIntersecting;
        // In case ima add anything in the future...
    };

    class CollisionHandler {
    public:
        RedCannonBall::World& world;
        std::function<CollisionResponse(RedCannonBall::Entity&, RedCannonBall::Entity&)> collisionMap[2][2];
        std::function<void(RedCannonBall::Entity&, RedCannonBall::Entity&, CollisionResponse&)> solveMap[2][2];
        CollisionHandler(RedCannonBall::World& world);

        CollisionResponse test(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2);
        void solve(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision);

        CollisionResponse testCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2);

        void solveCircleCircle(RedCannonBall::Entity& circle1, RedCannonBall::Entity& circle2, CollisionResponse& collision);
    };
} // namespace RedCannonBall