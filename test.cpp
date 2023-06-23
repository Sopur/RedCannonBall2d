#include "quadtree/quadtree.h"

#define ARENA_WIDTH  100000
#define ARENA_HEIGHT 100000

static void QTQuery(QUADTREE* qt, unsigned int entityID) {
    // Call on every intersection
}

static void QTNodeQuery(QUADTREE* qt, const QUADTREE_NODE_INFO* Info) {
    // Call on every split
}

static uint8_t QTUpdate(QUADTREE* Quadtree, uint32_t EntityIdx) {
    // Preform update on every entity
}

static int QTIsColliding(const QUADTREE* Quadtree, uint32_t EntityAIdx, uint32_t EntityBIdx) {
    // Collision test
    return 1;
}

static void QTCollideEntities(QUADTREE* Quadtree, uint32_t EntityAIdx, uint32_t EntityBIdx) {
    // On collision
}

int main() {
    QUADTREE Quadtree;

    Quadtree.X = 0;
    Quadtree.Y = 0;
    Quadtree.W = ARENA_WIDTH;
    Quadtree.H = ARENA_HEIGHT;

    Quadtree.Query = QTQuery;
    Quadtree.NodeQuery = QTNodeQuery;
    Quadtree.Update = QTUpdate;
    Quadtree.IsColliding = QTIsColliding;
    Quadtree.Collide = QTCollideEntities;
    Quadtree.MinSize = 0.8f;

    QuadtreeInit(&Quadtree);

    QuadtreeQueryNodes(&Quadtree, 0, 0, 100000, 100000);
    QuadtreeQuery(&Quadtree, 0, 0, 100000, 100000);
    return 0;
}
