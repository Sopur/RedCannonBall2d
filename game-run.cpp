#include "src/engine.cpp"
#include "src/geo-gen.hpp"
#include "src/geomerty.hpp"
#include "src/render.hpp"

RedCannonBall::PhysAttr movementSpeed = 200;
RedCannonBall::PhysAttr cameraSpeed = 2;
RedCannonBall::PhysAttr zoomSpeed = 0.005;
RedCannonBall::PhysAttr timeStep = 1;

double randZeroToOne() {
    return std::rand() / (RAND_MAX + 1.);
}

double random(double min, double max) {
    return randZeroToOne() * (max - min) + min;
}

int main() {
    RedCannonBall::Renderer engine("Physics sim", 1000, 1000, {0, timeStep}, RedCannonBall::Vector2d(0), 0.1);
    // engine.engine.insertEntity(RedCannonBall::EntityGen::invPolygon(11, 200), {0, -300}, 999, 9999, 0.5, 0.01);

    // for (int i = 0; i < 1; i++) {
    // }

    // engine.engine.insertEntity(RedCannonBall::Circle(0, 0, 30), 10, 1, 0.5, 0.01);
    //  engine.engine.insertEntity(RedCannonBall::EntityGen::invPolygon(5, 200), {0, 0}, 9999, 9999, 0.01, 0.01);
    // engine.engine.insertEntity(RedCannonBall::EntityGen::polygon(6, 700), {0, 0}, 0, 10, 0.5, 0.01);
    engine.engine.insertEntity(RedCannonBall::Circle(0, 0, 3000), 100, 10, 1, 0.1);

    for (int i = 0; i < 5000; i++) {
        engine.engine.insertEntity(RedCannonBall::Circle(random(-90000, 90000), random(-90000, 90000), random(100, 1000)), 10, 10, 1, 0.1);
    }

    // engine.engine.insertEntity(RedCannonBall::EntityGen::polygon(10, 10), {0, 300}, 10, 0.5, 1, 0.1);

    engine.run(int(1 / timeStep), 99999, [&engine] {
        auto adjustment = (GetMouseWheelMove() * zoomSpeed);
        if ((engine.fov + adjustment) > 0) {
            engine.fov += adjustment;
        }
        if (IsKeyDown(KEY_D)) engine.camera.x += cameraSpeed / zoomSpeed;
        if (IsKeyDown(KEY_A)) engine.camera.x -= cameraSpeed / zoomSpeed;
        if (IsKeyDown(KEY_W)) engine.camera.y -= cameraSpeed / zoomSpeed;
        if (IsKeyDown(KEY_S)) engine.camera.y += cameraSpeed / zoomSpeed;

        RedCannonBall::Entity& player = engine.engine.world.entities[0];
        player.force.zero();
        if (IsKeyDown(KEY_RIGHT)) player.force.x += movementSpeed;
        if (IsKeyDown(KEY_LEFT)) player.force.x -= movementSpeed;
        if (IsKeyDown(KEY_UP)) player.force.y -= movementSpeed;
        if (IsKeyDown(KEY_DOWN)) player.force.y += movementSpeed;

        engine.camera.lerpTo(player, 0.05);
    });
}