#ifndef STRUCT_BODY_H
#define STRUCT_BODY_H

#include "struct_vector.h"

struct Body
{
    double mass;
    double radius;
    Vector center;
    Vector speed;

    explicit Body(double mass = 1, double r = 1, double x = 0, double y = 0, double dx = 0, double dy = 0)
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
void operator/=(Body &a, const double &factor);

#endif