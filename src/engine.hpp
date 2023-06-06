#pragma once
#include "./geomerty.hpp"
#include <vector>

namespace RedCannonBall {
    struct Settings {
        PhysAttr gravity;
    };

    struct World {
    };

    class Engine {
    public:
        Engine(Settings settings);
        ~Engine();

        void iteration();      // Preform single iterations
        void iteration(int n); // Preform multiple iterations

        void staticIteration(World* world);        // Preform iterations on a specific world
        void staticIteration(World* world, int n); // Preform multiple iterations on a specific world
    };
} // namespace RedCannonBall