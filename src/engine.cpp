#include "engine.hpp"
#include "collision.hpp"
#include "geomerty.hpp"

RedCannonBall::Engine::Engine(Settings settings):
    nextID(0),
    world({settings, {}}) {}
RedCannonBall::Engine::~Engine() {}

void RedCannonBall::Engine::iteration() {
    for (auto& entity : world.entities) {
        if (!entity.isMoveable) continue;
        entity.applyForce(Vector2d(0, world.settings.gravity));

        for (auto& entityB : world.entities) {
            if (entity.id == entityB.id) continue;
            auto collision = CollisionHandler::collisionMap[int(entity.type)][int(entityB.type)](entity, entityB);
            if (collision.isIntersecting) {
                CollisionHandler::solveMap[int(entity.type)][int(entityB.type)](entity, entityB, collision);
            }
        }

        // This is a rlly shitty implementation :skull:
        Vector2d acceleration = entity.force / Vector2d(entity.mass);
        entity.velocity += acceleration;
        entity += entity.velocity;

        entity.resetForce();
    }
}
void RedCannonBall::Engine::iteration(int n) {
    for (int i = 0; i < n; i++) {
        iteration();
    }
}

void RedCannonBall::Engine::staticIteration(World* world) {}
void RedCannonBall::Engine::staticIteration(World* world, int n) {}

void RedCannonBall::Engine::insertEntity(Circle circle, PhysAttr mass, PhysAttr restitution, PhysAttr friction) {
    world.entities.push_back(Entity(mass, restitution, friction, circle, nextID++));
}
