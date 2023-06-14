#pragma once
#include "geomerty.hpp"
#include <functional>

namespace RedCannonBall {
    struct CollisionResponse {
        bool isIntersecting;
        Vector2d intersection;
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

        CollisionResponse testCircleCircle(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2);
        CollisionResponse testPolygonCircle(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2);
        CollisionResponse testPolygonPolygon(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2);

        void solveCircleCircle(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision);
        void solvePolygonCircle(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision);
        void solvePolygonPolygon(RedCannonBall::Entity& entity1, RedCannonBall::Entity& entity2, CollisionResponse& collision);
    };
} // namespace RedCannonBall