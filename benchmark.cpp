#include "include/transition.h"
#include "include/utils.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>

void extract_time(int num_particles, int num_thread) {
    int num_step = 50;
    std::vector<Body> bodies(num_particles);
    initialize_random(bodies, num_particles);

    auto start = std::chrono::steady_clock::now();

    for (size_t t = 0 ; t < num_step ; ++t)
        update(bodies, num_thread);
    
    auto finish = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

    std::cout << num_particles << " " << num_thread << " " << elapsed / 1e6 << "\n";
}

int main() {
    std::vector<int> range_num_particle = {2, 50, 500, 1000, 5000, 10000};
    std::vector<int> range_num_thread;
    // std::string file = "no_bht_time.txt";

    for (int i = 1 ; i < 11 ; ++i)
        range_num_thread.push_back(i);
    
    for (int nPoints : range_num_particle)
    for (int nThreads : range_num_thread)
        extract_time(nPoints, nThreads);
    
    return 0;
}
