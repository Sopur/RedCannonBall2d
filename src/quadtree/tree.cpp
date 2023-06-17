#pragma once
#include "tree.hpp"
#include "child.cpp"
#include "child.hpp"
#include "vec.hpp"

template <size_t QT_MAX_HOLD>
RedCannonBall::QuadTree<QT_MAX_HOLD>::QuadTree(RedCannonBall::Vector2d dimensions):
    area(0, 0, dimensions.x, dimensions.y),
    root(area, 0b00) {
}

template <size_t QT_MAX_HOLD>
RedCannonBall::QuadTree<QT_MAX_HOLD>::~QuadTree() {
    foundObjs.dealloc();
}

template <size_t QT_MAX_HOLD>
void RedCannonBall::QuadTree<QT_MAX_HOLD>::clear(void) {
    root.clear();
}

template <size_t QT_MAX_HOLD>
void RedCannonBall::QuadTree<QT_MAX_HOLD>::insert(Bound2d bound, QTID id) {
    if ((bound.x + bound.width) >= area.width || (bound.y + bound.height) >= area.height || (bound.x - bound.width) <= 0 || (bound.y - bound.height) <= 0) {
        std::cerr << "QUADTREE ERROR: Inserting \"" << id << "\" out of bounds: ";
        bound.print();
        exit(1);
    }

    QTMatrix matrix = {0};
    QTNode node = {bound, id};
    root.insert(node, matrix);
}

template <size_t QT_MAX_HOLD>
void RedCannonBall::QuadTree<QT_MAX_HOLD>::remove(Bound2d location, QTID id) {
    QTMatrix matrix = {0};
    QTNode node = {location, id};
    root.remove(node, matrix);
}

template <size_t QT_MAX_HOLD>
void RedCannonBall::QuadTree<QT_MAX_HOLD>::move(Bound2d location, Bound2d newLocation, QTID id) {
    remove(location, id);
    insert(newLocation, id);
}

template <size_t QT_MAX_HOLD>
void RedCannonBall::QuadTree<QT_MAX_HOLD>::collisions(std::function<void(IALStaticVector<QTID, QT_MAX_HOLD>& collisions)> callback) {
    root.collisions(callback);
}

template <size_t QT_MAX_HOLD>
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree<QT_MAX_HOLD>::get(Bound2d area) {
    QTMatrix matrix = {0};
    Box square(area);
    foundObjs.clear();
    root.get(foundObjs, square, matrix);
    foundObjs.removeDups();
    return foundObjs;
}

template <size_t QT_MAX_HOLD>
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree<QT_MAX_HOLD>::getAll(void) {
    foundObjs.clear();
    root.getAll(foundObjs);
    foundObjs.removeDups();
    return foundObjs;
}
