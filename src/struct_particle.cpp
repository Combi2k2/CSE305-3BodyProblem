#include "../include/struct_particle.h"

Particle operator + (const Particle &a, const Particle &b) {
    return Particle(
        a.mass + b.mass,
        (a.pos[0] * a.mass + b.pos[0] * b.mass) / (a.mass + b.mass),
        (a.pos[1] * a.mass + b.pos[1] * b.mass) / (a.mass + b.mass)
    );
}
void operator += (Particle &a, const Particle &b) {
    a.pos = (a.pos * a.mass + b.pos * b.mass) / (a.mass + b.mass);
    a.mass += b.mass;
}
void operator *= (Particle &a, const double &factor) {
    a.mass *= factor;
    a.pos *= factor;
}
void operator /= (Particle &a, const double &factor) {
    a.mass /= factor;
    a.pos /= factor;
}