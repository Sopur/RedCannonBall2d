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
#define ENTITY_SIZE_MIN_X 100
#define ENTITY_SIZE_MIN_Y 100
#define ENTITY_SIZE_MAX_X 1000
#define ENTITY_SIZE_MAX_Y 1000

#define timeNow()     std::chrono::high_resolution_clock::now()
#define durationMS()  float(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())
#define durationMCS() float(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count())

double randZeroToOne() {
    return std::rand() / (RAND_MAX + 1.);
}

double random(double min, double max) {
    return randZeroToOne() * (max - min) + min;
}

int main() {
    // Log setup
    auto seed = std::time(nullptr);
    std::cout << "Setup:"
              << "\n\tArena width:\t" << ARENA_WIDTH
              << "\n\tArena height:\t" << ARENA_HEIGHT
              << "\n\tEntity count:\t" << ENTITY_COUNT
              << "\n\tMin size:\t" << ENTITY_SIZE_MIN_X << 'x' << ENTITY_SIZE_MIN_Y
              << "\n\tMax size:\t" << ENTITY_SIZE_MAX_X << 'x' << ENTITY_SIZE_MAX_Y
              << "\n\tRandom seed:\t" << seed
              << "\n\n";
    std::srand(seed);

    // Initialize
    RedCannonBall::QuadTree qt(RedCannonBall::Vector2d(ARENA_WIDTH, ARENA_HEIGHT));

    // Generate entities
    std::vector<RedCannonBall::Bound2d> entities;
    for (int i = 0; i < ENTITY_COUNT; i++) {
        entities.push_back(RedCannonBall::Bound2d(
            random(ENTITY_SIZE_MAX_X, ARENA_WIDTH - ENTITY_SIZE_MAX_X),
            random(ENTITY_SIZE_MAX_Y, ARENA_HEIGHT - ENTITY_SIZE_MAX_Y),
            random(ENTITY_SIZE_MIN_X, ENTITY_SIZE_MAX_X),
            random(ENTITY_SIZE_MIN_Y, ENTITY_SIZE_MAX_Y)));
    }

    // Run insert bench
    auto t1 = timeNow();
    for (int i = 1; i < ENTITY_COUNT; i++) {
        qt.insert(entities[i], i);
    }
    auto t2 = timeNow();
    std::cout << "Inserting " << ENTITY_COUNT << ": " << durationMS() << "ms\t(" << durationMCS() / ENTITY_COUNT << " μs/insrt)\n";

    // Run get bench
    int entitiesFetched = 0;
    t1 = timeNow();
    for (int i = 0; i < ENTITY_COUNT; i++) {
        entitiesFetched += qt.get(entities[i]).size();
    }
    t2 = timeNow();
    std::cout << "Fetching " << ENTITY_COUNT << ":  " << durationMS() << "ms\t(" << durationMCS() / ENTITY_COUNT << " μs/fetch)\n";
    std::cout << "Retrieved " << entitiesFetched << " entities (" << float(entitiesFetched) / float(ENTITY_COUNT) << " ent/fetch)\n";
    return 0;
}