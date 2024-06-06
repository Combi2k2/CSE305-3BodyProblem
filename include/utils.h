#ifndef UTILS_H
#define UTILS_H

#include "struct_body.h"
#include "struct_vector.h"
#include "constant.h"

#include <vector>

double Random();

void initialize_random(std::vector<Body> &bodies, int N);
void initialize_4galaxies(std::vector<Body> &bodies, int N);
void initialize_solarsystem(std::vector<Body> &bodies, int N);

bool check_collision(const Body &a, const Body &b);

#endif