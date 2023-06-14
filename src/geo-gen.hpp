#pragma once
#include "geomerty.hpp"
#include <cmath>
#include <raylib.h>

#define PIx2 (PI * 2)

namespace RedCannonBall {
    namespace EntityGen {
        // Square/octogon like shape
        Mesh polygon(int points, PhysAttr radius) {
            Mesh lines;
            lines.reserve(points);

            for (int i = 0; i < points;) {
                PhysAttr theta = (PIx2 * i) / points;
                Line line;
                line.a = {radius * std::cos(theta), radius * std::sin(theta)};
                i++;
                theta = (PIx2 * i) / points;
                line.b = {radius * std::cos(theta), radius * std::sin(theta)};
                lines.push_back(line);
            }

            return lines;
        }

        // Star like shape
        Mesh invPolygon(int points, PhysAttr radius) {
            points *= 2;
            Mesh lines;
            lines.reserve(points);

            for (int i = 0; i < points;) {
                PhysAttr theta = (PIx2 * i) / points;
                Line line;

                if (i % 2 == 0) {
                    line.a = RedCannonBall::Vector2d((radius / 2) * std::cos(theta), (radius / 2) * std::sin(theta));
                    i++;
                    theta = (PIx2 * i) / points;
                    line.b = RedCannonBall::Vector2d(radius * std::cos(theta), radius * std::sin(theta));
                } else {
                    line.a = RedCannonBall::Vector2d(radius * std::cos(theta), radius * std::sin(theta));
                    i++;
                    theta = (PIx2 * i) / points;
                    line.b = RedCannonBall::Vector2d((radius / 2) * std::cos(theta), (radius / 2) * std::sin(theta));
                }

                lines.push_back(line);
            }

            return lines;
        }
    } // namespace EntityGen
} // namespace RedCannonBall