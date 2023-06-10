#include "src/engine.cpp"
#include "src/geomerty.hpp"
#include "src/render.hpp"

int main() {
    RedCannonBall::PhysAttr timeStep = 1;
    RedCannonBall::Renderer engine("Physics sim", 700, 700, {1, timeStep}, RedCannonBall::Vector2d(0), 0.2);
    engine.engine.insertEntity(RedCannonBall::Circle(0, 1500, 1600), 0, 0.5, 0.1);
    for (int i = 0; i < 1000; i++) {
        engine.engine.insertEntity(RedCannonBall::Circle(-500, -300 - (100 * i), 30), 10, 1, 0.1);
    }
    engine.run(int(1 / timeStep), [&engine] {
        // engine.fov -= 0.001;
    });
}