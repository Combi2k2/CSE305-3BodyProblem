#include "../include/struct_body.h"

Body operator + (const Body &a, const Body &b) {
    return Body(
        a.mass + b.mass,
        a.radius + b.radius,
        a.center[0] + b.center[0],
        a.center[1] + b.center[1],
        a.speed[0] + b.speed[0],
        a.speed[1] + b.speed[1]
    );
}
void operator += (Body &a, const Body &b) {
    a.mass += b.mass;
    a.radius += b.radius;
    a.center += b.center;
    a.speed += b.speed;
}
void operator *= (Body &a, const double &factor) {
    a.mass *= factor;
    a.radius *= factor;
    a.center *= factor;
    a.speed *= factor;
}