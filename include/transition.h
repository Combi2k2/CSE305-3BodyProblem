#ifndef TRANSITION_H
#define TRANSITION_H

#include "struct_vector.h"
#include "struct_body.h"

#include <vector>

void update(std::vector<Body> &bodies, int nThreads = 1);

#endif