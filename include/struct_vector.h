#ifndef STRUCT_VECTOR_H
#define STRUCT_VECTOR_H

#include <math.h>

struct Vector {
    double data[2];

    Vector(double x = 0, double y = 0) {
        data[0] = x;
        data[1] = y;
    }
    inline double norm2() const {
        return data[0] * data[0] + data[1] * data[1];
    }
    inline double norm() const {
        return sqrt(norm2());
    }
    inline void normalize() {
        double n = norm();
        data[0] /= n;
        data[1] /= n;
    }
    inline double  operator[](int i) const {
        return data[i];
    }
    inline double& operator[](int i) {
        return data[i];
    };
};

Vector operator + (const Vector &a, const Vector &b);
Vector operator - (const Vector &a, const Vector &b);
Vector operator * (const Vector &a, const Vector &b);
Vector operator * (const double &a, const Vector& b);
Vector operator * (const Vector &a, const double &b);
Vector operator / (const Vector &a, const double &b);
double dot(const Vector &a, const Vector &b);

void operator += (Vector &a, const Vector &b);
void operator -= (Vector &a, const Vector &b);
void operator *= (Vector &a, const Vector &b);
void operator *= (Vector &a, const double &b);
void operator /= (Vector &a, const double &b);

#endif