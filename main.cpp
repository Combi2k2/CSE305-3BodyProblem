#include "include/transition.h"
#include "include/utils.h"

#include <iostream>

void initBodies(Body *objs, size_t N) {
    for (int i = 0 ; i < N ; ++i) {
        objs[i].mass = 100;
        objs[i].radius = 5;

        objs[i].center[0] = Random();
        objs[i].center[1] = Random();
        objs[i].speed[0] = 0;
        objs[i].speed[1] = 0;
    }
}

int main(int argc, char **argv) {
    int N = 100;
    int step = 500;
    Body objs[100];

    initBodies(objs, N);

    for (int t = 0 ; t < step ; ++t) {
        update(objs, N);

        for (int i = 0 ; i < N ; ++i)   {
            std::cout << objs[i].center[0] << " " << objs[i].center[1] << "\n";
        }
    }
}