#include "include/transition.h"
#include "include/utils.h"

#include <iostream>

int main(int argc, char **argv) {
    int N = 20;
    int step = 500;
    std::vector<Body> bodies(N);

    initialize_random(bodies, N);

    for (int t = 0 ; t < step ; ++t) {
        update(bodies, 2);

        for (int i = 0 ; i < N ; ++i)   {
            std::cout << bodies[i].center[0] << " " << bodies[i].center[1] << "\n";
        }
    }
}