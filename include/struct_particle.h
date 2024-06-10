#ifndef STRUCT_PARTICLE_H
#define STRUCT_PARTICLE_H

#include "struct_vector.h"

struct Particle {
    Vector pos;
    double mass;

    Particle(double m = 0, double x = 0, double y = 0) : mass(m), pos(x, y) {}
    ~Particle() = default;
};

Particle operator + (const Particle &a, const Particle &b);
void operator += (Particle &a, const Particle &b);
void operator *= (Particle &a, const double &factor);
void operator /= (Particle &a, const double &factor);

#endif