#pragma once
#include "../hsg.cpp"
#include "collision.hpp"
#include "geo-gen.hpp"
#include "geomerty.hpp"
#include "quadtree/child.hpp"
#include "quadtree/tree.hpp"
#include <vector>

namespace RedCannonBall {
    class Engine {
    private:
        PhysID nextID;
        CollisionHandler collisions;
        QuadTree<10> tree;

    public:
        World world;

        Engine(Settings settings);
        ~Engine();

        void iteration();      // Preform single iterations
        void iteration(int n); // Preform multiple iterations

        void staticIteration(World* world);        // Preform iterations on a specific world
        void staticIteration(World* world, int n); // Preform multiple iterations on a specific world

        void insertEntity(Circle circle, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction);
        void insertEntity(Mesh lines, Vector2d position, PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction);
    };
} // namespace RedCannonBall