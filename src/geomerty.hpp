#pragma once

#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>

#define IMMOVABLE_MASS 999999.0f

namespace RedCannonBall {
    typedef double PhysAttr;
    typedef unsigned long PhysID;

    enum EntityType {
        Entity_Circle,
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

        Vector2d project2d(Vector2d origin, PhysAttr fov, PhysAttr screenWidth, PhysAttr screenHeight) {
            return Vector2d((x - origin.x) * fov + screenWidth / 2, (y - origin.y) * fov + screenHeight / 2);
        }

        PhysAttr sum() {
            return x + y;
        }

        void lerpTo(Vector2d to, PhysAttr x) {
            x = lerp(x, to.x, x);
            y = lerp(y, to.y, y);
        }

        void zero(void) {
            x = 0;
            y = 0;
        }

        bool isZero(void) {
            return x + y == 0;
        }

        void print() {
            std::cout << "{ " << x << ", " << y << " }\n";
        }
    };

    class Bound2d: public Vector2d {
    public:
        PhysAttr width;
        PhysAttr height;

        Bound2d(PhysAttr x, PhysAttr y, PhysAttr width, PhysAttr height):
            Vector2d(x, y),
            width(width),
            height(height) {}

        void align(void) {
            x += -width / 2;
            y += -height / 2;
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
        PhysAttr rotation;
        PhysAttr restitution;
        PhysAttr friction;
        PhysAttr angularVel;
        Vector2d velocity;
        Vector2d force;
        PhysAttr invMass;
        bool isMoveable;

        PhysicsObject(PhysAttr x, PhysAttr y, PhysAttr mass, PhysAttr rotation, PhysAttr restitution, PhysAttr friction):
            Vector2d(x, y),
            mass(mass),
            rotation(rotation),
            restitution(restitution),
            friction(friction),
            angularVel(0) {
            if (mass == 0) {
                mass = IMMOVABLE_MASS;
                isMoveable = false;
            } else {
                isMoveable = true;
            }
            invMass = 1 / mass;
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
        const PhysID id;
        const EntityType type;

        // Circle shape case
        PhysAttr radius;

        Entity(PhysAttr mass, PhysAttr rotation, PhysAttr restitution, PhysAttr friction, EntityType type, PhysID id):
            id(id),
            type(type),
            PhysicsObject(0, 0, mass, rotation, restitution, friction) {}
        Entity(PhysAttr mass, PhysAttr restitution, PhysAttr friction, Circle circle, PhysID id):
            id(id),
            type(EntityType::Entity_Circle),
            PhysicsObject(0, 0, mass, 0, restitution, friction) {
            setProperties(circle);
        }

        void setProperties(Circle& circle) {
            x = circle.x;
            y = circle.y;
            radius = circle.radius;
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
