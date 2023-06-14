#pragma once
#include "child.hpp"
#include <cstring>
#include <unordered_set>

#define FIND_SECTOR(vec) \
    splits[(vec.x > area.mid.x)][(vec.y > area.mid.y)]

RedCannonBall::QuadTreeChild::QuadTreeChild(Bound2d area, const ChildIndex childIndex, int maxHold, int holdIncrementor, int initDepth, int extendedDepth):
    childIndex(childIndex),
    area(area),
    maxHold(maxHold),
    holdIncrementor(holdIncrementor),
    extendedDepth(extendedDepth) {
    if (area.width + area.height < 0.0001) {
        std::cerr << "QUADTREE ERROR: Attempted to split too much: ";
        area.print();
        exit(1);
    }
    if (initDepth != 0) {
        contents = nullptr;
        split(initDepth - 1);
        return;
    }
    hasSplit = false;
    contents = new QTNode[maxHold];
    clear();
}
RedCannonBall::QuadTreeChild::~QuadTreeChild() {
    if (hasSplit) {
        delete splits[0][0];
        delete splits[1][0];
        delete splits[0][1];
        delete splits[1][1];
    } else {
        delete[] contents;
    }
}

void RedCannonBall::QuadTreeChild::split(int depth) {
    hasSplit = true;
    auto newHold = maxHold + holdIncrementor;
    splits[0][0] = new QuadTreeChild(Bound2d(area.x, area.y, area.half.x, area.half.y), 0b00, newHold, holdIncrementor, depth, extendedDepth);
    splits[1][0] = new QuadTreeChild(Bound2d(area.mid.x, area.y, area.half.x, area.half.y), 0b10, newHold, holdIncrementor, depth, extendedDepth);
    splits[0][1] = new QuadTreeChild(Bound2d(area.x, area.mid.y, area.half.x, area.half.y), 0b01, newHold, holdIncrementor, depth, extendedDepth);
    splits[1][1] = new QuadTreeChild(Bound2d(area.mid.x, area.mid.y, area.half.x, area.half.y), 0b11, newHold, holdIncrementor, depth, extendedDepth);

    if (contents != nullptr) {
        for (int i = 0; i < maxHold; i++) {
            if (contents[i].id != QT_NULL_ID) {
                QTMatrix matrix = {0};
                insert(contents[i], matrix);
            }
        }
        delete[] contents;
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
        std::memset(contents, 0, maxHold * sizeof(QTNode));
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
        for (int i = 0; i < maxHold; i++) {
            if (contents[i].id == QT_NULL_ID) {
                contents[i] = node;
                contentsUsed++;
                break;
            }
        }

        if (contentsUsed == maxHold) {
            split(extendedDepth);
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
        for (int i = 0; i < maxHold; i++) {
            if (contents[i].id == node.id) {
                contents[i].id = QT_NULL_ID;
                contentsUsed--;
                break;
            }
        }
    }
}
void RedCannonBall::QuadTreeChild::get(std::unordered_set<RedCannonBall::QTID>& output, Box& box, QTMatrix& matrix) {
    if (matrix[childIndex] == 1) return;
    matrix[childIndex] = 1;

    if (hasSplit) {
        QTMatrix getMatrix = {0};
        FIND_SECTOR(box.topRight)->get(output, box, getMatrix);
        FIND_SECTOR(box.topLeft)->get(output, box, getMatrix);
        FIND_SECTOR(box.bottomRight)->get(output, box, getMatrix);
        FIND_SECTOR(box.bottomLeft)->get(output, box, getMatrix);
    } else {
        for (int i = 0; i < maxHold; i++) {
            if (contents[i].id != QT_NULL_ID) {
                output.insert(contents[i].id);
            }
        }
    }
}
void RedCannonBall::QuadTreeChild::getAll(std::unordered_set<RedCannonBall::QTID>& output) {
    if (hasSplit) {
        splits[0][0]->getAll(output);
        splits[1][0]->getAll(output);
        splits[0][1]->getAll(output);
        splits[1][1]->getAll(output);
    } else {
        for (int i = 0; i < maxHold; i++) {
            if (contents[i].id != QT_NULL_ID) {
                output.insert(contents[i].id);
            }
        }
    }
}
