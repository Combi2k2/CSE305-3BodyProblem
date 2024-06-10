#include "../include/struct_body.h"

Body operator + (const Body &a, const Body &b) {
    Body ret;
    ret.center = a.center + b.center;
    ret.radius = a.radius + b.radius;
    ret.speed = (a.speed * a.center.mass + b.speed * b.center.mass) / (a.center.mass + b.center.mass);

    return ret;
}
void operator += (Body &a, const Body &b) {
    a.speed = (a.speed * a.center.mass + b.speed * b.center.mass) / (a.center.mass + b.center.mass);
    a.radius += b.radius;
    a.center.mass   += b.center.mass;
}
void operator *= (Body &a, const double &factor) {
    a.center *= factor;
    a.radius *= factor;
    a.speed *= factor;
}
void operator /= (Body &a, const double &factor) {
    a.center /= factor;
    a.radius /= factor;
    a.speed /= factor;
}