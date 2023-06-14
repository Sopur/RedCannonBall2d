#pragma once
#include "../geomerty.hpp"
#include "child.hpp"
#include <unordered_set>

namespace RedCannonBall {
    class QuadTree {
    public:
        Bound2d area;
        QuadTreeChild root;

        QuadTree(RedCannonBall::Vector2d dimensions, int maxHold = 10, int holdIncrementor = 1, int initDepth = 4, int childInitDepth = 0);
        ~QuadTree();

        void clear(void);
        void insert(Bound2d bound, QTID id);
        void remove(Bound2d location, QTID id);
        void move(Bound2d location, Bound2d newLocation, QTID id);
        std::unordered_set<QTID> get(Bound2d area);
        std::unordered_set<QTID> getAll(void);
    };
} // namespace RedCannonBall
