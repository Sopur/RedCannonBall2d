#pragma once
#include "child.hpp"
#include "vec.hpp"
#include <algorithm>
#include <cstring>

#define FIND_SECTOR(vec) \
    splits[(vec.x > area.mid.x)][(vec.y > area.mid.y)]

RedCannonBall::QuadTreeChild::QuadTreeChild(Bound2d area, const ChildIndex childIndex):
    childIndex(childIndex),
    area(area) {
    if (area.width + area.height < 0.0001) {
        std::cerr << "QUADTREE ERROR: Attempted to split too much: ";
        area.print();
        exit(1);
    }
    hasSplit = false;
    clear();
}
RedCannonBall::QuadTreeChild::~QuadTreeChild() {
    if (hasSplit) {
        delete splits[0][0];
        delete splits[1][0];
        delete splits[0][1];
        delete splits[1][1];
    }
}

void RedCannonBall::QuadTreeChild::split() {
    hasSplit = true;
    splits[0][0] = new QuadTreeChild(Bound2d(area.x, area.y, area.half.x, area.half.y), 0b00);
    splits[1][0] = new QuadTreeChild(Bound2d(area.mid.x, area.y, area.half.x, area.half.y), 0b10);
    splits[0][1] = new QuadTreeChild(Bound2d(area.x, area.mid.y, area.half.x, area.half.y), 0b01);
    splits[1][1] = new QuadTreeChild(Bound2d(area.mid.x, area.mid.y, area.half.x, area.half.y), 0b11);

    int matrix = 0;
    for (int i = 0; i < QT_MAX_HOLD; i++) {
        if (contents[i].id != QT_NULL_ID) {
            insert(contents[i], (QTMatrix&) matrix);
            matrix = 0;
        }
    }
}

void RedCannonBall::QuadTreeChild::clear(void) {
    if (hasSplit) {
        splits[0][0]->clear();
        splits[1][0]->clear();
        splits[0][1]->clear();
        splits[1][1]->clear();
    } else {
        contentsUsed = 1;
        for (int i = 0; i < QT_MAX_HOLD; i++) {
            contents[i].id = QT_NULL_ID;
        }
    }
}

void RedCannonBall::QuadTreeChild::insert(RedCannonBall::QTNode& node, QTMatrix& matrix) {
    if (matrix[childIndex] == 1) return;
    matrix[childIndex] = 1;

    if (hasSplit) {
        QTMatrix insertMatrix = {0};
        FIND_SECTOR(node.bound.topRight)->insert(node, insertMatrix);
        FIND_SECTOR(node.bound.topLeft)->insert(node, insertMatrix);
        FIND_SECTOR(node.bound.bottomRight)->insert(node, insertMatrix);
        FIND_SECTOR(node.bound.bottomLeft)->insert(node, insertMatrix);
    } else {
        for (int i = 0; i < QT_MAX_HOLD; i++) {
            if (contents[i].id == QT_NULL_ID) {
                contents[i] = node;
                contentsUsed++;
                break;
            }
        }

        if (contentsUsed == QT_MAX_HOLD) {
            split();
        }
    }
}

void RedCannonBall::QuadTreeChild::remove(RedCannonBall::QTNode& node, QTMatrix& matrix) {
    if (matrix[childIndex] == 1) return;
    matrix[childIndex] = 1;

    if (hasSplit) {
        QTMatrix removeMatrix = {0};
        FIND_SECTOR(node.bound.topRight)->remove(node, removeMatrix);
        FIND_SECTOR(node.bound.topLeft)->remove(node, removeMatrix);
        FIND_SECTOR(node.bound.bottomRight)->remove(node, removeMatrix);
        FIND_SECTOR(node.bound.bottomLeft)->remove(node, removeMatrix);
    } else {
        for (int i = 0; i < QT_MAX_HOLD; i++) {
            if (contents[i].id == node.id) {
                contents[i].id = QT_NULL_ID;
                contentsUsed--;
                break;
            }
        }
    }
}
void RedCannonBall::QuadTreeChild::get(IALVector<RedCannonBall::QTID>& output, Box& box, QTMatrix& matrix) {
    if (matrix[childIndex] == 1) return;
    matrix[childIndex] = 1;

    if (hasSplit) {
        QTMatrix getMatrix = {0};
        FIND_SECTOR(box.topRight)->get(output, box, getMatrix);
        FIND_SECTOR(box.topLeft)->get(output, box, getMatrix);
        FIND_SECTOR(box.bottomRight)->get(output, box, getMatrix);
        FIND_SECTOR(box.bottomLeft)->get(output, box, getMatrix);
    } else {
        for (int i = 0; i < QT_MAX_HOLD; i++) {
            if (contents[i].id != QT_NULL_ID) {
                output.push_back(contents[i].id);
            }
        }
    }
}
void RedCannonBall::QuadTreeChild::getAll(IALVector<RedCannonBall::QTID>& output) {
    if (hasSplit) {
        splits[0][0]->getAll(output);
        splits[1][0]->getAll(output);
        splits[0][1]->getAll(output);
        splits[1][1]->getAll(output);
    } else {
        for (int i = 0; i < QT_MAX_HOLD; i++) {
            if (contents[i].id != QT_NULL_ID) {
                output.push_back(contents[i].id);
            }
        }
    }
}
