#include "../include/struct_body.h"

Body operator+(const Body &a, const Body &b)
{
    return Body(
        a.mass + b.mass,
        a.radius + b.radius,
        (a.center[0] * a.mass + b.center[0] * b.mass) / (a.mass + b.mass),
        (a.center[1] * a.mass + b.center[1] * b.mass) / (a.mass + b.mass),
        (a.speed[0] * a.mass + b.speed[0] * b.mass) / (a.mass + b.mass),
        (a.speed[1] * a.mass + b.speed[1] * b.mass) / (a.mass + b.mass));
}
void operator+=(Body &a, const Body &b)
{
    a.mass += b.mass;
    a.radius += b.radius;
    a.center = (a.center * a.mass + b.center * b.mass) / (a.mass);
    a.speed += (a.speed * a.mass + b.speed * b.mass) / (a.mass);
}

void operator*=(Body &a, const double &factor)
{
    a.mass *= factor;
    a.radius *= factor;
    a.center *= factor;
    a.speed *= factor;
}

void operator/=(Body &a, const double &factor)
{
    a.mass /= factor;
    a.radius /= factor;
    a.center /= factor;
    a.speed /= factor;
}