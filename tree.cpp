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

#define ARENA_WIDTH       100000
#define ARENA_HEIGHT      100000
#define ENTITY_COUNT      10000
#define ENTITY_SIZE_MIN_X 10
#define ENTITY_SIZE_MIN_Y 10
#define ENTITY_SIZE_MAX_X 1000
#define ENTITY_SIZE_MAX_Y 1000
#define FETCH_SIZE_X      10
#define FETCH_SIZE_Y      10

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

double random(double min, double max) {
    return randZeroToOne() * (max - min) + min;
}

int main() {
    auto seed = std::time(nullptr);
    std::cout << "Setup:"
              << "\n\tArena width:\t" << ARENA_WIDTH
              << "\n\tArena height:\t" << ARENA_HEIGHT
              << "\n\tEntity count:\t" << ENTITY_COUNT
              << "\n\tMin size:\t" << ENTITY_SIZE_MIN_X << 'x' << ENTITY_SIZE_MIN_Y
              << "\n\tMax size:\t" << ENTITY_SIZE_MAX_X << 'x' << ENTITY_SIZE_MAX_Y
              << "\n\tFetch size:\t" << FETCH_SIZE_X << 'x' << FETCH_SIZE_Y
              << "\n\tRandom seed:\t" << seed
              << "\n\n";
    std::srand(seed);

    RedCannonBall::QuadTree qt(RedCannonBall::Vector2d(ARENA_WIDTH, ARENA_HEIGHT), 10, 0, 0, 0);

    auto t1 = timeNow();
    for (int i = 1; i < ENTITY_COUNT; i++) {
        qt.insert(RedCannonBall::Bound2d(
                      random(ENTITY_SIZE_MAX_X, ARENA_WIDTH - ENTITY_SIZE_MAX_X),
                      random(ENTITY_SIZE_MAX_Y, ARENA_HEIGHT - ENTITY_SIZE_MAX_Y),
                      random(ENTITY_SIZE_MIN_X, ENTITY_SIZE_MAX_X),
                      random(ENTITY_SIZE_MIN_Y, ENTITY_SIZE_MAX_Y)),
            i);
    }
    auto t2 = timeNow();
    std::cout << "Inserting " << ENTITY_COUNT << " entities: " << duration() << "ms\n";

    int entitiesFetched = 0;
    t1 = timeNow();
    for (int i = 0; i < ENTITY_COUNT; i++) {
        entitiesFetched += qt.get(RedCannonBall::Bound2d(
                                      random(FETCH_SIZE_X, ARENA_WIDTH - FETCH_SIZE_X),
                                      random(FETCH_SIZE_Y, ARENA_HEIGHT - FETCH_SIZE_Y),
                                      FETCH_SIZE_X,
                                      FETCH_SIZE_Y))
                               .size();
    }
    t2 = timeNow();
    std::cout << "Fetching " << ENTITY_COUNT << " times: " << duration() << "ms\n";
    std::cout << entitiesFetched << " entities (" << float(entitiesFetched) / float(ENTITY_COUNT) << " ent/fetch) were retrieved\n";
    return 0;
}