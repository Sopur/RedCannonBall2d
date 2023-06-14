#pragma once
#include "../geomerty.hpp"
#include <unordered_set>

#define QT_NULL_ID 0

namespace RedCannonBall {
    typedef unsigned long QTID;
    typedef unsigned char ChildIndex;
    typedef ChildIndex QTMatrix[4];

    struct QTNode {
        Box bound;
        QTID id;
    };

    class QuadTreeChild {
    private:
        const ChildIndex childIndex;
        QTNode* contents;
        int contentsUsed;
        QuadTreeChild* splits[2][2];
        bool hasSplit;

    public:
        Bound2d area;
        int maxHold;
        int holdIncrementor;
        int extendedDepth;

        QuadTreeChild(Bound2d area, const ChildIndex childIndex, int maxHold, int holdIncrementor, int initDepth, int extendedDepth);
        ~QuadTreeChild();

        void split(int depth);
        void clear(void);
        void insert(RedCannonBall::QTNode& node, QTMatrix& matrix);
        void remove(RedCannonBall::QTNode& node, QTMatrix& matrix);
        void get(std::unordered_set<RedCannonBall::QTID>& output, Box& area, QTMatrix& matrix);
        void getAll(std::unordered_set<RedCannonBall::QTID>& output);
    };
} // namespace RedCannonBall
