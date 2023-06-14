#pragma once
#include "engine.hpp"
#include "geomerty.hpp"
#include <functional>
#include <raylib.h>
#include <string>

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

        void run(int iterationsPerFrame, int entityDrawLimit, std::function<void()> onFrame) {
            std::cout << "ENGINE: Running " << iterationsPerFrame << " iterations per frame\n";
            while (!WindowShouldClose()) {
                onFrame();
                BeginDrawing();
                ClearBackground(RAYWHITE);

                int entitiesDrawn = 0;
                for (auto entity : engine.world.entities) {
                    if (entitiesDrawn >= entityDrawLimit) break;
                    /*if (!entity.collision.isZero()) {
                        auto projectedCollision = entity.collision.project2d(camera, fov, screen.width, screen.height);
                        DrawCircle(projectedCollision.x, projectedCollision.y, 5 * fov, BLUE);
                    }*/

                    switch (entity.type) {
                    case EntityType::Entity_Circle: {
                        auto projected = entity.project2d(camera, fov, screen.width, screen.height);
                        auto projectedRadius = entity.radius * fov;
                        if ((projected.x - projectedRadius) > screen.width || (projected.y - projectedRadius) > screen.height || (projected.x + projectedRadius) < 0 || (projected.y + projectedRadius) < 0) {
                            continue;
                        }
                        entitiesDrawn++;
                        DrawCircle(projected.x, projected.y, projectedRadius, RED);

                        // No point on rendering if the radius is too small
                        if (projectedRadius < 5) break;
                        auto treeNode = Bound2d(Circle(entity.x, entity.y, entity.radius));
                        treeNode.align();
                        auto projectedTree = treeNode.project2d(camera, fov, screen.width, screen.height);
                        DrawRectangleLines(projectedTree.x, projectedTree.y, treeNode.width * fov, treeNode.height * fov, BLUE);
                    }
                    case EntityType::Entity_Polygon: {
                        for (auto& line : entity.projectedLines) {
                            Line projectedLine = line.project2d(camera, fov, screen.width, screen.height);
                            DrawLine(projectedLine.a.x, projectedLine.a.y, projectedLine.b.x, projectedLine.b.y, BLUE);
                        }
                    }
                    }
                }
                DrawFPS(10, 10);
                DrawText((std::to_string(entitiesDrawn) + " Drawn").c_str(), 100, 10, 20, BLACK);

                EndDrawing();
                engine.iteration(iterationsPerFrame);
            }
            CloseWindow();
        }
    };
} // namespace RedCannonBall