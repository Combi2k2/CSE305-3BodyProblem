#ifndef STRUCT_BODY_H
#define STRUCT_BODY_H

#include "struct_vector.h"

struct Body
{
    double mass;
    double radius;
    Vector center;
    Vector speed;

    explicit Body(double mass, double r, double x, double y, double dx, double dy)
    {
        this->mass = mass;
        this->radius = r;
        this->center = Vector(x, y);
        this->speed = Vector(dx, dy);
    }
    Body &operator=(const Body &other)
    {
        if (this == &other)
            return *this;

        this->mass = other.mass;
        this->center = other.center;
        this->radius = other.radius;
        this->speed = other.speed;

        return *this;
    }
};

Body operator+(const Body &a, const Body &b);
void operator+=(Body &a, const Body &b);
void operator*=(Body &a, const double &factor);

#endif