#include "../include/transition.h"
#include "../include/constant.h"

#include <math.h>
#include <cfloat>
#include <climits>
#include <thread>
#include <numeric>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <algorithm>

static void computeForceThread(std::vector<Body> &bodies, size_t start, size_t end, std::vector<Vector> &a, size_t N) {
    for (size_t i = start ; i < end ; ++i) {
        a[i] = Vector(0, 0);

        for (size_t j = 0 ; j < N ; ++j) {
            double dx = bodies[j].center[0] - bodies[i].center[0];
            double dy = bodies[j].center[1] - bodies[i].center[1];
            double dist_sq = dx * dx + dy * dy + 0.01;

            double acceleration = G * bodies[j].mass / dist_sq;

            a[i][0] += acceleration * dx / sqrt(dist_sq);
            a[i][1] += acceleration * dy / sqrt(dist_sq);
        }
    }
}

void update(std::vector<Body> &bodies, int nThreads) {
    size_t N = bodies.size();

    std::vector<Vector> acc(N);
    std::vector<std::thread> t(nThreads);

    size_t BLOCK_SIZE = (N + nThreads - 1) / nThreads;

    for (size_t i = 0 ; i < nThreads ; ++i) {
        size_t l = std::min(i * BLOCK_SIZE, N);
        size_t r = std::min(l + BLOCK_SIZE, N);

        t[i] = std::thread(&computeForceThread, std::ref(bodies), l, r, std::ref(acc), N);
    }
    for (size_t i = 0 ; i < nThreads ; ++i)
        t[i].join();

    for (size_t i = 0 ; i < N ; ++i) {
        bodies[i].speed  += DELTAT * acc[i];
        bodies[i].center += DELTAT * bodies[i].speed;
    }
}