#pragma once
#include "./geomerty.hpp"
#include <vector>

namespace RedCannonBall {
    struct Settings {
        PhysAttr gravity;
    };

    struct World {
        Settings settings;
        std::vector<Entity> entities;
    };

    class Engine {
    private:
        PhysID nextID;

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