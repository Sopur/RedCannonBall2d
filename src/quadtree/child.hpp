#pragma once
#include "../geomerty.hpp"
#include "vec.hpp"

#define QT_NULL_ID  0
#define QT_MAX_HOLD 10

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
        QTNode contents[QT_MAX_HOLD];
        int contentsUsed;
        QuadTreeChild* splits[2][2];
        bool hasSplit;

    public:
        Bound2d area;

        QuadTreeChild(Bound2d area, const ChildIndex childIndex);
        ~QuadTreeChild();

        void split();
        void clear(void);
        void insert(RedCannonBall::QTNode& node, QTMatrix& matrix);
        void remove(RedCannonBall::QTNode& node, QTMatrix& matrix);
        void get(IALVector<RedCannonBall::QTID>& output, Box& area, QTMatrix& matrix);
        void getAll(IALVector<RedCannonBall::QTID>& output);
    };
} // namespace RedCannonBall
