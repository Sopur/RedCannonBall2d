#include "src/quadtree/tree.cpp"
#include "src/geomerty.hpp"
#include "src/quadtree/child.hpp"
#include "src/quadtree/tree.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#define ARENA_WIDTH   100000
#define ARENA_HEIGHT  100000
#define ENTITY_COUNT  50
#define ENTITY_SIZE_X 6
#define ENTITY_SIZE_Y 6
#define FETCH_SIZE_X  10
#define FETCH_SIZE_Y  10

#define timeNow()  std::chrono::high_resolution_clock::now()
#define duration() std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()

void logSet(std::vector<RedCannonBall::QTID> data) {
    std::stringstream output;
    for (auto id : data) {
        output << id << " ";
    }
    std::cout << "[" << data.size() << "]:\t{ " << output.str() << "}\n";
}

double randZeroToOne() {
    return std::rand() / (RAND_MAX + 1.);
}

double random(int x) {
    return randZeroToOne() * x;
}

int main() {
    auto seed = std::time(nullptr);
    std::cout << "Setup:"
              << "\n\tArena width:\t" << ARENA_WIDTH
              << "\n\tArena height:\t" << ARENA_HEIGHT
              << "\n\tEntity count:\t" << ENTITY_COUNT
              << "\n\tEntity size:\t" << ENTITY_SIZE_X << 'x' << ENTITY_SIZE_Y
              << "\n\tFetch size:\t" << FETCH_SIZE_X << 'x' << FETCH_SIZE_Y
              << "\n\tRandom seed:\t" << seed
              << "\n\n";
    std::srand(seed);

    RedCannonBall::QuadTree qt(RedCannonBall::Vector2d(ARENA_WIDTH, ARENA_HEIGHT), 10, 3, 0);

    RedCannonBall::Bound2d lastBound;
    auto t1 = timeNow();
    for (int i = 1; i < ENTITY_COUNT; i++) {
        lastBound.x = random(qt.area.width);
        lastBound.y = random(qt.area.height);
        lastBound.width = ENTITY_SIZE_X;
        lastBound.height = ENTITY_SIZE_Y;
        qt.insert(lastBound, i);
    }
    auto t2 = timeNow();
    std::cout << "Insert time: " << duration() << "ms\n";

    int entitiesFetched = 0;
    t1 = timeNow();
    for (int i = 0; i < ENTITY_COUNT; i++) {
        entitiesFetched += qt.get(RedCannonBall::Bound2d(random(qt.area.width), random(qt.area.height), FETCH_SIZE_X, FETCH_SIZE_Y)).size();
    }
    t2 = timeNow();
    std::cout << "Fetch time: " << duration() << "ms\n";
    std::cout << entitiesFetched << " entities were retrieved\n";
    return 0;
}