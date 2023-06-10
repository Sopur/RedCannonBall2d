#include "engine.hpp"
#include "collision.cpp"
#include "collision.hpp"
#include "geomerty.hpp"

RedCannonBall::Engine::Engine(Settings settings):
    nextID(0),
    world({settings, {}}),
    collisions(world) {}
RedCannonBall::Engine::~Engine() {}

void RedCannonBall::Engine::iteration() {
    for (auto& entity : world.entities) {
        if (!entity.isMoveable) continue;
        entity.applyForce(Vector2d(0, world.settings.gravity));

        for (auto& entityB : world.entities) {
            if (entity.id == entityB.id) continue;

            auto collision = collisions.test(entity, entityB);
            if (collision.isIntersecting) {
                collisions.solve(entity, entityB, collision);
            }
        }

        // Euler's method
        Vector2d acceleration = entity.force / Vector2d(entity.mass);
        entity.velocity += acceleration * world.settings.timeStep;
        entity += entity.velocity * world.settings.timeStep;

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
