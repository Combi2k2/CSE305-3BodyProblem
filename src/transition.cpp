#include "../include/transition.h"
#include "../include/constant.h"
#include "../optim/optim.h"

#include <math.h>
#include <cfloat>
#include <climits>
#include <thread>
#include <numeric>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <algorithm>

#ifdef BARNES_HUT
static void insertThread(const std::vector<Body> &bodies, size_t start, size_t end) {
    for (size_t i = start ; i < end ; ++i)
        Optimizer_BarnesHut::insert(bodies[i].center);
}
#endif

static void computeThread(std::vector<Body> &bodies, size_t start, size_t end, std::vector<Vector> &a, size_t N) {
#ifdef BARNES_HUT
    for (size_t i = start ; i < end ; ++i)
        Optimizer_BarnesHut::query(bodies[i].center, a[i], THETA);
#else
    for (size_t i = start ; i < end ; ++i) {
        a[i] = Vector(0, 0);

        for (size_t j = 0 ; j < N ; ++j) {
            double dx = bodies[j].center.pos[0] - bodies[i].center.pos[0];
            double dy = bodies[j].center.pos[1] - bodies[i].center.pos[1];
            double dist = dx * dx + dy * dy + 0.01;

            double acceleration = G * bodies[j].center.mass / (dist * dist * dist);

            a[i][0] += acceleration * dx;
            a[i][1] += acceleration * dy;
        }
    }
#endif
}

void update(std::vector<Body> &bodies, int nThreads) {
    size_t N = bodies.size();

    if (nThreads > N)
        nThreads = N;
 
    std::vector<Vector> acc(N);
    std::vector<std::thread> t(nThreads);

    size_t BLOCK_SIZE = (N + nThreads - 1) / nThreads;
#ifdef BARNES_HUT
    double xMin = bodies[0].center.pos[0], xMax = bodies[0].center.pos[0];
    double yMin = bodies[0].center.pos[1], yMax = bodies[0].center.pos[1];

    for (const Body &obj : bodies) {
        xMin = std::min(xMin, obj.center.pos[0]);
        xMax = std::max(xMax, obj.center.pos[0]);
        yMin = std::min(yMin, obj.center.pos[1]);
        yMax = std::max(yMax, obj.center.pos[1]);
    }
    double box_size = std::max({-xMin, xMax, -yMin, yMax}) * 2;
    Optimizer_BarnesHut::setRoot(0, 0, box_size);
    
    for (size_t i = 0 ; i < nThreads ; ++i) {
        size_t l = std::min(i * BLOCK_SIZE, N);
        size_t r = std::min(l + BLOCK_SIZE, N);

        t[i] = std::thread(&insertThread, std::cref(bodies), l, r);
    }
    for (size_t i = 0 ; i < nThreads ; ++i)
        t[i].join();
#endif
    for (size_t i = 0 ; i < nThreads ; ++i) {
        size_t l = std::min(i * BLOCK_SIZE, N);
        size_t r = std::min(l + BLOCK_SIZE, N);

        t[i] = std::thread(&computeThread, std::ref(bodies), l, r, std::ref(acc), N);
    }
    for (size_t i = 0 ; i < nThreads ; ++i)
        t[i].join();

    for (size_t i = 0 ; i < N ; ++i) {
        bodies[i].speed      += DELTAT * acc[i];
        bodies[i].center.pos += DELTAT * bodies[i].speed;
    }
}