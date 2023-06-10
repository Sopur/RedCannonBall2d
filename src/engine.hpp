#pragma once
#include "./geomerty.hpp"
#include "collision.hpp"
#include <vector>

namespace RedCannonBall {
    class Engine {
    private:
        PhysID nextID;
        CollisionHandler collisions;

    public:
        World world;

        Engine(Settings settings);
        ~Engine();

        void iteration();      // Preform single iterations
        void iteration(int n); // Preform multiple iterations

        void staticIteration(World* world);        // Preform iterations on a specific world
        void staticIteration(World* world, int n); // Preform multiple iterations on a specific world

        void insertEntity(Circle circle, PhysAttr mass, PhysAttr restitution, PhysAttr friction);
    };
} // namespace RedCannonBall