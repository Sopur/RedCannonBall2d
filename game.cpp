#include "src/engine.cpp"
#include "src/geomerty.hpp"
#include "src/render.hpp"

int main() {
    RedCannonBall::Renderer engine("Physics sim", 900, 900, {1}, RedCannonBall::Vector2d(0), 1);
    engine.engine.insertEntity(RedCannonBall::Circle(0, 120, 200), 0, 1, 0.1);
    engine.engine.insertEntity(RedCannonBall::Circle(-100, -300, 50), 10, 1, 0.1);
    engine.engine.insertEntity(RedCannonBall::Circle(-100, -310, 50), 10, 1, 0.1);
    engine.engine.insertEntity(RedCannonBall::Circle(-100, -320, 50), 10, 1, 0.1);
    engine.engine.insertEntity(RedCannonBall::Circle(-100, -330, 50), 10, 1, 0.1);
    engine.run([&engine] {
        engine.fov -= 0.001;
    });
}