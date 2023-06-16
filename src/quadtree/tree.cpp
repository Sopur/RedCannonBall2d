#pragma once
#include "tree.hpp"
#include "child.cpp"
#include "child.hpp"
#include "vec.hpp"

void removeDup(IALVector<RedCannonBall::QTID>& vec) {
    std::sort(vec.begin(), vec.end());
    vec.slice(std::unique(vec.begin(), vec.end()));
}

RedCannonBall::QuadTree::QuadTree(RedCannonBall::Vector2d dimensions):
    area(0, 0, dimensions.x, dimensions.y),
    root(area, 0b00) {
}
RedCannonBall::QuadTree::~QuadTree() {
    foundObjs.dealloc();
}

void RedCannonBall::QuadTree::clear(void) {
    root.clear();
}
void RedCannonBall::QuadTree::insert(Bound2d bound, QTID id) {
    if ((bound.x + bound.width) >= area.width || (bound.y + bound.height) >= area.height || (bound.x - bound.width) <= 0 || (bound.y - bound.height) <= 0) {
        std::cerr << "QUADTREE ERROR: Inserting \"" << id << "\" out of bounds: ";
        bound.print();
        exit(1);
    }

    QTMatrix matrix = {0};
    QTNode node = {bound, id};
    root.insert(node, matrix);
}
void RedCannonBall::QuadTree::remove(Bound2d location, QTID id) {
    QTMatrix matrix = {0};
    QTNode node = {location, id};
    root.remove(node, matrix);
}
void RedCannonBall::QuadTree::move(Bound2d location, Bound2d newLocation, QTID id) {
    remove(location, id);
    insert(newLocation, id);
}
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree::get(Bound2d area) {
    QTMatrix matrix = {0};
    Box square(area);
    foundObjs.clear();
    root.get(foundObjs, square, matrix);
    removeDup(foundObjs);
    return foundObjs;
}
IALVector<RedCannonBall::QTID>& RedCannonBall::QuadTree::getAll(void) {
    foundObjs.clear();
    root.getAll(foundObjs);
    removeDup(foundObjs);
    return foundObjs;
}
