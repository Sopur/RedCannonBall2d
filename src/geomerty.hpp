#pragma once

namespace RedCannonBall {
    typedef float PhysAttr;

    enum class EntityType {
        Circle,
        Rectangle,
        Mesh
    };

    class Vector2d {
    public:
        PhysAttr x;
        PhysAttr y;

        Vector2d(void) = default;
        Vector2d(PhysAttr x);
        Vector2d(PhysAttr x, PhysAttr y);

        Vector2d copy(void);

        Vector2d operator+(Vector2d vec);
        Vector2d operator+=(Vector2d vec);
        Vector2d operator-(Vector2d vec);
        Vector2d operator-=(Vector2d vec);
        Vector2d operator*(Vector2d vec);
        Vector2d operator*=(Vector2d vec);
        Vector2d operator/(Vector2d vec);
        Vector2d operator/=(Vector2d vec);

        PhysAttr angleTo(Vector2d vec);
        PhysAttr distance(Vector2d vec);
        Vector2d distanceXY(Vector2d vec);

        void zero(void);
        void lerpTo(Vector2d to, PhysAttr x);
    };

    class Bound2d: public Vector2d {
    public:
        PhysAttr width;
        PhysAttr height;

        Bound2d(PhysAttr x, PhysAttr y, PhysAttr width, PhysAttr height);

        void align(void);
    };

    class Circle: public Vector2d {
    public:
        PhysAttr radius;

        Circle(PhysAttr x, PhysAttr y, PhysAttr radius);

        operator Bound2d();
    };

    class PhysicsObject: public Vector2d {
    public:
        PhysAttr rotation;
        PhysAttr mass;
        Vector2d velocity;
        Vector2d acceleration;
        Vector2d force;
        Vector2d angularVel;
        Vector2d angularAccel;
        Vector2d angularForce;

        PhysicsObject(PhysAttr x, PhysAttr y, PhysAttr mass);

        PhysicsObject copy(void);

        void applyForce(Vector2d vec);
        void resetForce(void);
    };
} // namespace RedCannonBall
