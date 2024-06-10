#ifndef STRUCT_BODY_H
#define STRUCT_BODY_H

#include "struct_vector.h"
#include "struct_particle.h"

struct Body {
    Particle center;
    double radius;
    Vector speed;

    Body(double m = 1, double r = 1, double x = 0, double y = 0, double dx = 0, double dy = 0) {
        this->speed  = Vector(dx, dy);
        this->center = Particle(m, x, y);
        this->radius = r;
    }
    Body &operator=(const Body &other) {
        if (this == &other)
            return *this;
        
        this->center = other.center;
        this->radius = other.radius;
        this->speed  = other.speed;

        return *this;
    }
};

Body operator + (const Body &a, const Body &b);
void operator += (Body &a, const Body &b);
void operator *= (Body &a, const double &factor);
void operator /= (Body &a, const double &factor);

#endif