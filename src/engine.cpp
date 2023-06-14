#pragma once
#include "engine.hpp"
#include "collision.cpp"
#include "collision.hpp"
#include "geomerty.hpp"
#include "quadtree/tree.cpp"
#include "quadtree/tree.hpp"
#include <cstddef>

#define VELOCITY_FLOOR 0.1

RedCannonBall::Engine::Engine(Settings settings):
    nextID(1),
    tree(Vector2d(1000000), 10, 0, 0, 0),
    world({settings, {}}),
    collisions(world) {}
RedCannonBall::Engine::~Engine() {}

void RedCannonBall::Engine::iteration() {
    staticIteration(&world);
}
void RedCannonBall::Engine::iteration(int n) {
    staticIteration(&world, n);
}

void RedCannonBall::Engine::staticIteration(World* arena) {
    tree.clear();
    for (auto& entity : arena->entities) {
        tree.insert(entity.getCircleNode(tree.area), entity.id);
    }

    for (auto& entity : arena->entities) {
        if (!entity.isMoveable) continue;
        Vector2d force = {0, world.settings.gravity};

        // Euler's method with angular velocity
        Vector2d acceleration = (force + entity.force) / Vector2d(entity.mass);
        entity.velocity += acceleration * arena->settings.timeStep;
        entity += entity.velocity * arena->settings.timeStep;

        PhysAttr angularAcceleration = entity.angularForce / entity.inertia;
        entity.angularVelocity += angularAcceleration * arena->settings.timeStep;
        entity.rotation += entity.angularVelocity * arena->settings.timeStep;

        if (entity.type == EntityType::Entity_Polygon) {
            entity.updateLines();
        }

        auto possibleCollisions = tree.get(entity.getCircleNode(tree.area));
        for (auto entityID : possibleCollisions) {
            if (entity.id == entityID) continue;
            auto& entityB = world.entities[entityID - 1];

            auto collision = collisions.test(entity, entityB);
            if (collision.isIntersecting) {
                entity.collision = collision.intersection;
                collisions.solve(entity, entityB, collision);
            }
        }
    }
}

void RedCannonBall::Engine::staticIteration(World* arena, int n) {
    for (int i = 0; i < n; i++) {
        staticIteration(arena);
    }
}

void RedCannonBall::Engine::insertEntity(Circle circle, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction) {
    auto entity = Entity(mass, inertia, restitution, friction, circle, nextID++);
    tree.insert(entity.getCircleNode(tree.area), entity.id);
    world.entities.push_back(entity);
}

void RedCannonBall::Engine::insertEntity(Mesh lines, Vector2d position, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction) {
    world.entities.push_back(Entity(mass, inertia, restitution, friction, lines, position, nextID++));
}
