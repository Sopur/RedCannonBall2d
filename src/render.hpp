#pragma once
#include "engine.hpp"
#include "geomerty.hpp"
#include <functional>
#include <raylib.h>

namespace RedCannonBall {
    class Renderer {
    private:
        Bound2d screen;
        const char* windowName;

    public:
        Vector2d camera;
        RedCannonBall::PhysAttr fov;

        Engine engine;

        Renderer(const char* windowName, int w, int h, Settings physicsSettings, Vector2d camera, RedCannonBall::PhysAttr fov):
            screen(0, 0, w, h),
            windowName(windowName),
            engine(physicsSettings),
            camera(camera),
            fov(fov) {
            InitWindow(screen.width, screen.width, windowName);
            SetTargetFPS(60);
        }

        void run(std::function<void()> onFrame) {
            while (!WindowShouldClose()) {
                onFrame();
                BeginDrawing();
                ClearBackground(RAYWHITE);

                for (auto entity : engine.world.entities) {
                    auto projected = entity.project2d(camera, fov, screen.width, screen.height);

                    switch (entity.type) {
                    case EntityType::Circle: {
                        DrawCircle(projected.x, projected.y, entity.radius * fov, RED);
                        break;
                    }
                    }
                }

                EndDrawing();
                engine.iteration();
            }
            CloseWindow();
        }
    };
} // namespace RedCannonBall