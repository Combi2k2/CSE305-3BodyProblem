#include "../include/transition.h"
#include "../include/constant.h"

#include <cfloat>
#include <climits>
#include <thread>
#include <numeric>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <algorithm>

void computeForceThread(Body *objs, size_t start, size_t end, Vector *f, size_t N) {
    for (size_t i = start ; i < end ; ++i) {
        f[i] = Vector(0, 0);

        for (size_t j = 0 ; j < N ; ++j) {
            if (i == j) continue;

            double size = G * objs[i].mass * objs[j].mass / (objs[i].center - objs[j].center).norm2();
            Vector dir = objs[j].center - objs[i].center;

            dir.normalize();

            f[i] += dir * size;
        }
    }
}

void update(Body *objs, size_t N, int nThreads) {
    Vector *f = new Vector[N];

    size_t BLOCK_SIZE = (N + nThreads - 1) / nThreads;
    size_t start = 0;
    size_t end = BLOCK_SIZE;

    std::thread t[nThreads - 1];

    for (size_t i = 0 ; i < nThreads - 1 ; ++i) {
        t[i] = std::thread(&computeForceThread, objs, start, end, f, N);

        start = end;
        end = start + BLOCK_SIZE;
    }
    computeForceThread(objs, start, N, f, N);

    for (size_t i = 0 ; i < N ; ++i) {
        objs[i].center += DELTAT * objs[i].speed;
        objs[i].speed  += DELTAT / objs[i].mass * f[i];
    }

    delete f;
}