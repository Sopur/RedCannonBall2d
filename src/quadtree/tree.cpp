#pragma once
#include "tree.hpp"
#include "child.cpp"
#include "child.hpp"
#include "vec.hpp"

template <int MaxHold>
RedCannonBall::QuadTree<MaxHold>::QuadTree(RedCannonBall::Vector2d dimensions):
    area(0, 0, dimensions.x, dimensions.y),
    root(area, 0b00) {
}

template <int MaxHold>
RedCannonBall::QuadTree<MaxHold>::~QuadTree() {
    foundObjs.dealloc();
}

template <int MaxHold>
void RedCannonBall::QuadTree<MaxHold>::clear(void) {
    root.clear();
}

template <int MaxHold>
void RedCannonBall::QuadTree<MaxHold>::insert(Bound2d bound, QTID id) {
    if ((bound.x + bound.width) >= area.width || (bound.y + bound.height) >= area.height || (bound.x - bound.width) <= 0 || (bound.y - bound.height) <= 0) {
        std::cerr << "QUADTREE ERROR: Inserting \"" << id << "\" out of bounds: ";
        bound.print();
        exit(1);
    }

    QTMatrix matrix = {0};
    QTNode node = {bound, id};
    root.insert(node, matrix);
}

template <int MaxHold>
void RedCannonBall::QuadTree<MaxHold>::remove(Bound2d location, QTID id) {
    QTMatrix matrix = {0};
    QTNode node = {location, id};
    root.remove(node, matrix);
}

template <int MaxHold>
void RedCannonBall::QuadTree<MaxHold>::move(Bound2d location, Bound2d newLocation, QTID id) {
    remove(location, id);
    insert(newLocation, id);
}

template <int MaxHold>
void RedCannonBall::QuadTree<MaxHold>::collisions(std::function<void(IALStaticVector<QTID, MaxHold>& collisions)> callback) {
    root.collisions(callback);
}

template <int MaxHold>
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree<MaxHold>::get(Bound2d area) {
    QTMatrix matrix = {0};
    Box square(area);
    foundObjs.clear();
    root.get(foundObjs, square, matrix);
    foundObjs.removeDups();
    return foundObjs;
}

template <int MaxHold>
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree<MaxHold>::getAll(void) {
    foundObjs.clear();
    root.getAll(foundObjs);
    foundObjs.removeDups();
    return foundObjs;
}
