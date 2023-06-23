#pragma once
#include "engine.hpp"
#include "../hsg.cpp"
#include "collision.cpp"
#include "collision.hpp"
#include "geomerty.hpp"
#include "quadtree/child.hpp"
#include "quadtree/tree.cpp"
#include "quadtree/tree.hpp"
#include <cstddef>

#define VELOCITY_FLOOR 0.1

RedCannonBall::Engine::Engine(Settings settings):
    nextID(0),
    tree(Vector2d(1000000)),
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

        tree.remove(entity.bound, entity.id);
        entity.bound = entity.getCircleNode(tree.area);
        tree.insert(entity.bound, entity.id);
    }

    tree.collisions([this](IALStaticVector<QTID, 10>& entityIDs) {
        for (int i = 0; i < entityIDs.length; i++) {
            auto& entityA = world.entities[entityIDs[i]];
            for (int j = 0; j < entityIDs.length; j++) {
                if (i == j) continue;
                auto& entityB = world.entities[entityIDs[j]];

                auto collision = collisions.test(entityA, entityB);
                if (collision.isIntersecting) {
                    entityA.collision = collision.intersection;
                    collisions.solve(entityA, entityB, collision);
                }
            }
        }
    });
}

void RedCannonBall::Engine::staticIteration(World* arena, int n) {
    for (int i = 0; i < n; i++) {
        staticIteration(arena);
    }
}

void RedCannonBall::Engine::insertEntity(Circle circle, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction) {
    auto entity = Entity(mass, inertia, restitution, friction, circle, nextID++);
    entity.bound = entity.getCircleNode(tree.area);
    tree.insert(entity.bound, entity.id);
    world.entities.push_back(entity);
}

void RedCannonBall::Engine::insertEntity(Mesh lines, Vector2d position, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction) {
    world.entities.push_back(Entity(mass, inertia, restitution, friction, lines, position, nextID++));
}
