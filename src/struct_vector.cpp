#include "../include/struct_vector.h"

Vector operator + (const Vector &a, const Vector &b) {
    return Vector(a[0] + b[0], a[1] + b[1]);
}
Vector operator - (const Vector &a, const Vector &b) {
    return Vector(a[0] - b[0], a[1] - b[1]);
}
Vector operator * (const Vector &a, const Vector &b) {
    return Vector(a[0] * b[0], a[1] * b[1]);
}
Vector operator * (const double &a, const Vector &b) {
    return Vector(a * b[0], a * b[1]);
}
Vector operator * (const Vector &a, const double &b) {
    return Vector(a[0] * b, a[1] * b);
}
Vector operator / (const Vector &a, const double &b) {
    return Vector(a[0] / b, a[1] / b);
}
double dot(const Vector& a, const Vector& b) {
    return a[0] * b[0] + a[1] * b[1];
}

void operator += (Vector &a, const Vector &b) {
    a[0] += b[0];
    a[1] += b[1];
}
void operator -= (Vector &a, const Vector &b) {
    a[0] -= b[0];
    a[1] -= b[1];
}
void operator *= (Vector &a, const Vector &b) {
    a[0] *= b[0];
    a[1] *= b[1];
}
void operator *= (Vector &a, const double &b) {
    a[0] *= b;
    a[1] *= b;
}
void operator /= (Vector &a, const double &b) {
    a[0] /= b;
    a[1] /= b;
}