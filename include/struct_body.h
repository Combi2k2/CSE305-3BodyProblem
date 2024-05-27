#ifndef STRUCT_BODY_H
#define STRUCT_BODY_H

#include "struct_vector.h"

struct Body {
    double m;
    Vector position;
    Vector velocity;

    Body(const double &m, const Vector &position, const Vector &velocity) {
        this->m = m;
        this->position = position;
        this->velocity = velocity;
    }
};

#endif