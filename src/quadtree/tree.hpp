#pragma once
#include "../geomerty.hpp"
#include "child.hpp"
#include "vec.hpp"
#include <functional>

namespace RedCannonBall {
    template <int MaxHold>
    class QuadTree {
    private:
        IALVector<QTID> foundObjs;

    public:
        Bound2d area;
        QuadTreeChild<MaxHold> root;

        QuadTree(RedCannonBall::Vector2d dimensions);
        ~QuadTree();

        void clear(void);
        void insert(Bound2d bound, QTID id);
        void remove(Bound2d location, QTID id);
        void move(Bound2d location, Bound2d newLocation, QTID id);
        void collisions(std::function<void(IALStaticVector<QTID, MaxHold>& collisions)> callback);
        IALVector<QTID>& get(Bound2d area);
        IALVector<QTID>& getAll(void);
    };
} // namespace RedCannonBall
