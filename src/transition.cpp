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

static void computeThread(const std::vector<Body> &bodies, size_t start, size_t end, std::vector<Vector> &a, size_t N) {
    for (size_t i = start ; i < end ; ++i) {
        a[i] = Vector(0, 0);
#ifdef BARNES_HUT
        Optimizer_BarnesHut::query(bodies[i].center, a[i], THETA);
#else
        for (size_t j = 0 ; j < N ; ++j)
            computeAcceleration(bodies[i].center, bodies[j].center, a[i]);
#endif
    }
}

void update(std::vector<Body> &bodies, int nThreads) {
    size_t N = bodies.size();

    if (nThreads > N)
        nThreads = N;
 
    std::vector<Vector> acc(N);
    std::vector<std::thread> workers(nThreads-1);

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
    double box_size = std::max(xMax - xMin, yMax - yMin) * 2;
    Optimizer_BarnesHut::setRoot((xMin + xMax) / 2, (yMin + yMax) / 2, box_size);

    for (size_t i = 0 ; i < nThreads-1 ; ++i) {
        size_t l = i * BLOCK_SIZE;
        size_t r = l + BLOCK_SIZE;

        workers[i] = std::thread(&insertThread, std::cref(bodies), l, r);
    }
    insertThread(bodies, nThreads*BLOCK_SIZE-BLOCK_SIZE, N);

    for (size_t i = 0 ; i < nThreads-1 ; ++i)
        workers[i].join();
#endif
    for (size_t i = 0 ; i < nThreads-1 ; ++i) {
        size_t l = i * BLOCK_SIZE;
        size_t r = l + BLOCK_SIZE;

        workers[i] = std::thread(&computeThread, std::cref(bodies), l, r, std::ref(acc), N);
    }
    computeThread(bodies, (nThreads-1) * BLOCK_SIZE, N, acc, N);
    
    for (size_t i = 0 ; i < nThreads-1 ; ++i)
        workers[i].join();
    
    for (size_t i = 0 ; i < N ; ++i) {
        bodies[i].speed      += DELTAT * acc[i];
        bodies[i].center.pos += DELTAT * bodies[i].speed;
    }
}