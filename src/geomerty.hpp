#pragma once

#include <cmath>
#include <iostream>
#include <math.h>
#include <raylib.h>
#include <vector>

#define IMMOVABLE_MASS 999999.0

namespace RedCannonBall {
    class Line;

    typedef double PhysAttr;
    typedef unsigned long PhysID;
    typedef std::vector<Line> Mesh;

    enum EntityType {
        Entity_Circle,
        Entity_Polygon,
    };

    PhysAttr lerp(PhysAttr a, PhysAttr b, PhysAttr x) {
        return a + x * (b - a);
    }

    class Vector2d {
    public:
        PhysAttr x;
        PhysAttr y;

        Vector2d():
            x(0),
            y(0) {}
        Vector2d(PhysAttr x):
            x(x),
            y(x) {}
        Vector2d(PhysAttr x, PhysAttr y):
            x(x),
            y(y) {}

        Vector2d operator+(Vector2d vec) {
            return Vector2d(x + vec.x, y + vec.y);
        }
        Vector2d operator+=(Vector2d vec) {
            x += vec.x;
            y += vec.y;
            return *this;
        }

        Vector2d operator-(Vector2d vec) {
            return Vector2d(x - vec.x, y - vec.y);
        }
        Vector2d operator-=(Vector2d vec) {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

        Vector2d operator*(Vector2d vec) {
            return Vector2d(x * vec.x, y * vec.y);
        }
        Vector2d operator*=(Vector2d vec) {
            x *= vec.x;
            y *= vec.y;
            return *this;
        }

        Vector2d operator/(Vector2d vec) {
            return Vector2d(x / vec.x, y / vec.y);
        }
        Vector2d operator/=(Vector2d vec) {
            x /= vec.x;
            y /= vec.y;
            return *this;
        }

        PhysAttr cross(Vector2d vec) {
            return x * vec.y - y * vec.x;
        }

        PhysAttr dot(Vector2d vec) {
            return x * vec.x + y * vec.y;
        }

        PhysAttr magnitudeSquared() {
            return x * x + y * y;
        };

        PhysAttr magnitude() {
            return std::sqrt(magnitudeSquared());
        };

        Vector2d normalize() {
            PhysAttr mag = magnitude();

            if (mag == 0) return Vector2d(0);
            return Vector2d(x / mag, y / mag);
        }

        Vector2d proj(Vector2d vec) {
            PhysAttr k = dot(vec) / vec.dot(vec);
            return vec * k;
        }

        PhysAttr hypot2(Vector2d vec) {
            return ((*this) - vec).dot(((*this) - vec));
        }

        PhysAttr distance(Vector2d vec) {
            return ((*this) - vec).magnitude();
        }

        Vector2d scale(PhysAttr scalar) {
            return (*this) * scalar;
        }

        Vector2d rotate(Vector2d center, PhysAttr angle) {
            PhysAttr s = std::sin(angle);
            PhysAttr c = std::cos(angle);
            Vector2d vec = *this;

            // Translate the point back to origin
            vec.x -= center.x;
            vec.y -= center.y;

            // Rotate it
            float xnew = vec.x * c - vec.y * s;
            float ynew = vec.x * s + vec.y * c;

            // Translate back
            vec.x = xnew + center.x;
            vec.y = ynew + center.y;
            return vec;
        }

        Vector2d project2d(Vector2d origin, PhysAttr fov, PhysAttr screenWidth, PhysAttr screenHeight) {
            return Vector2d((x - origin.x) * fov + screenWidth / 2, (y - origin.y) * fov + screenHeight / 2);
        }

        PhysAttr sum() {
            return x + y;
        }

        void lerpTo(Vector2d to, PhysAttr b) {
            x = lerp(x, to.x, b);
            y = lerp(y, to.y, b);
        }

        bool isZero() {
            return sum() == 0;
        }

        void zero(void) {
            x = 0;
            y = 0;
        }

        void print() {
            std::cout << "{ " << x << ", " << y << " }\n";
        }
    };

    class Line {
    public:
        Vector2d a;
        Vector2d b;
        Line() = default;
        Line(Vector2d p1, Vector2d p2):
            a(p1),
            b(p2) {}

        Vector2d direction() {
            return b - a;
        }

        Vector2d middle() {
            return {(a.x + b.x) / 2, (a.y + b.y) / 2};
        }

        Line center(Vector2d& point) {
            return Line(a + point, b + point);
        }

        Line project2d(Vector2d origin, PhysAttr fov, PhysAttr screenWidth, PhysAttr screenHeight) {
            return Line(a.project2d(origin, fov, screenWidth, screenHeight), b.project2d(origin, fov, screenWidth, screenHeight));
        }

        Line rotate(Vector2d center, PhysAttr angle) {
            return Line(a.rotate(center, angle), b.rotate(center, angle));
        }
    };

    class Bound2d: public Vector2d {
    public:
        PhysAttr width;
        PhysAttr height;
        Vector2d half;
        Vector2d mid;

        Bound2d():
            Vector2d(0),
            width(0),
            height(0) {
        }

        Bound2d(PhysAttr x, PhysAttr y, PhysAttr width, PhysAttr height):
            Vector2d(x, y),
            width(width),
            height(height),
            half({width * 0.5f, height * 0.5f}),
            mid({x + half.x, y + half.y}) {}

        // Centers the bound
        void align(void) {
            mid.x = x;
            mid.y = y;
            x += -width / 2;
            y += -height / 2;
        }

        // Localize from a 0,0 point to a point around a bound
        Bound2d localize(Bound2d& area) {
            return Bound2d(x + area.width / 2, y + area.height / 2, width, height);
        }

        void print() {
            std::cout << "{ x: " << x << ", y: " << y << ", w: " << width << ", h: " << height << " }\n";
        }
    };

    class Box {
    public:
        Vector2d topLeft;
        Vector2d topRight;
        Vector2d bottomLeft;
        Vector2d bottomRight;

        Box() = default;
        Box(Bound2d& bound) {
            topLeft = bound;
            topRight = bound + Vector2d(bound.width, 0);
            bottomLeft = bound + Vector2d(0, bound.height);
            bottomRight = bound + Vector2d(bound.width, bound.height);
        }
    };

    class Circle: public Vector2d {
    public:
        PhysAttr radius;

        Circle(PhysAttr x, PhysAttr y, PhysAttr radius):
            Vector2d(x, y),
            radius(radius) {}

        operator Bound2d() {
            PhysAttr r2 = radius * 2;
            return Bound2d(x, y, r2, r2);
        }
    };

    class PhysicsObject: public Vector2d {
    public:
        PhysAttr mass;
        PhysAttr inertia;
        PhysAttr rotation;
        PhysAttr restitution;
        PhysAttr friction;
        PhysAttr angularVelocity;
        Vector2d velocity;
        Vector2d force;
        PhysAttr angularForce;
        PhysAttr invMass;
        PhysAttr invInertia;
        bool isMoveable;

        PhysicsObject(PhysAttr x, PhysAttr y, PhysAttr mass, PhysAttr inertia, PhysAttr rotation, PhysAttr restitution, PhysAttr friction):
            Vector2d(x, y),
            mass(mass),
            inertia(inertia),
            rotation(rotation),
            restitution(restitution),
            friction(friction),
            angularVelocity(0),
            angularForce(0) {
            if (mass == 0) {
                mass = IMMOVABLE_MASS;
                inertia = IMMOVABLE_MASS;
                isMoveable = false;
            } else {
                isMoveable = true;
            }
            invMass = 1 / mass;
            invInertia = 1 / inertia;
        }

        void applyForce(Vector2d vec) {
            force += vec;
        }
        void resetForce(void) {
            force.zero();
        }
    };

    class Entity: public PhysicsObject {
    public:
        const EntityType type;
        const PhysID id;
        Bound2d bound;

        // Debug
        Vector2d collision;

        // Circle shape case
        PhysAttr radius;

        // Polygon shape case
        Mesh lines;
        Mesh projectedLines;

        Entity(PhysAttr mass, PhysAttr inertia, PhysAttr rotation, PhysAttr restitution, PhysAttr friction, const EntityType type, const PhysID id):
            type(type),
            id(id),
            PhysicsObject(0, 0, mass, inertia, rotation, restitution, friction) {}
        Entity(PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction, Circle circle, PhysID id):
            type(EntityType::Entity_Circle),
            id(id),
            PhysicsObject(circle.x, circle.y, mass, inertia, 0, restitution, friction),
            radius(circle.radius) {
        }
        Entity(PhysAttr mass, PhysAttr inertia, PhysAttr restitution, PhysAttr friction, Mesh lines, Vector2d position, PhysID id):
            type(EntityType::Entity_Polygon),
            id(id),
            PhysicsObject(position.x, position.y, mass, inertia, 0, restitution, friction),
            lines(lines) {
            projectedLines.resize(lines.size());
            updateLines();
        }

        void updateLines() {
            auto size = lines.size();
            for (size_t i = 0; i < size; i++) {
                projectedLines[i] = lines[i].rotate(0, rotation * (PI / 180)).center(*this);
            }
        }

        Bound2d getCircleNode(Bound2d& area) {
            auto bound = Bound2d(Circle(x, y, radius));
            bound.align();
            return bound.localize(area);
        }
    };

    struct Settings {
        PhysAttr gravity;
        PhysAttr timeStep;
    };

    struct World {
        Settings settings;
        std::vector<Entity> entities;
    };
} // namespace RedCannonBall
