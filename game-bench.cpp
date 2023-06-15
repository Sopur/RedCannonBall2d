#include "src/engine.cpp"
#include "src/engine.hpp"
#include "src/geo-gen.hpp"
#include "src/geomerty.hpp"
#include "src/render.hpp"
#include <chrono>

#define timeNow()  std::chrono::high_resolution_clock::now()
#define duration() std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()

double randZeroToOne() {
    return std::rand() / (RAND_MAX + 1.);
}

double random(double min, double max) {
    return randZeroToOne() * (max - min) + min;
}

#define ENTITY_AMOUNT   10000
#define SPAWN_X         90000
#define SPAWN_Y         90000
#define ENTITY_SIZE_MIN 100
#define ENTITY_SIZE_MAX 400

int main() {
    RedCannonBall::Engine engine({0, 1});

    for (int i = 0; i < ENTITY_AMOUNT; i++) {
        engine.insertEntity(RedCannonBall::Circle(random(-SPAWN_X, SPAWN_X), random(-SPAWN_Y, SPAWN_Y), random(ENTITY_SIZE_MIN, ENTITY_SIZE_MAX)), 10, 10, 1, 0.1);
    }

    long averageSpeed = 0;
    for (int i = 0; i < 1000; i++) {
        auto t1 = timeNow();
        engine.iteration();
        auto t2 = timeNow();
        auto timed = duration();
        averageSpeed = (averageSpeed + timed) / 2;
        std::cout << "Iteration #" << i << ":\ttime: " << timed << "ms\taverage: " << averageSpeed << "ms\n";
    }
    return 0;
}