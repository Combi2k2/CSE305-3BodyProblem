#include "../include/utils.h"
#include <assert.h>

#define MODULUS    2147483647
#define MULTIPLIER 48271
#define DEFAULT    123456789

static long seed = DEFAULT;

double Random()
/* ----------------------------------------------------------------
 * Random returns a pseudo-random real number uniformly distributed 
 * between 0.0 and 1.0. 
 * ----------------------------------------------------------------
 */
{
    const long Q = MODULUS / MULTIPLIER;
    const long R = MODULUS % MULTIPLIER;
    long t;

    t = MULTIPLIER * (seed % Q) - R * (seed / Q);
    if (t > 0) 
        seed = t;
    else 
        seed = t + MODULUS;
    
    return ((double) seed / MODULUS);
}

void initialize_random(std::vector<Body> &bodies, int N) {
    double total_mass = 0;
    double mvx = 0;
    double mvy = 0;

    for (Body &obj : bodies) {
        obj.mass = Random() * 40.0 / N;
        obj.radius = 5;

		obj.center[0] = Random() * 2 - 1;
        obj.center[1] = Random() * 2 - 1;

		obj.speed[0] = Random() * 2 - 1;
		obj.speed[1] = Random() * 2 - 1;

        mvx += obj.speed[0] * obj.mass;
        mvy += obj.speed[1] * obj.mass;

        total_mass += obj.mass;
    }
    for (Body &obj : bodies) {
        obj.radius = obj.mass / total_mass * 100;
        obj.speed[0] -= mvx / total_mass;
        obj.speed[1] -= mvy / total_mass;
    }
}
void initialize_4galaxies(std::vector<Body> &bodies, int N) {
    double total_mass = 0;
    double mvx = 0;
    double mvy = 0;

    for (int i = 0 ; i < N ; ++i) {
		if (i < 4) {
            bodies[i].mass = Random() + 20;
            bodies[i].center[0] = dx[i];
            bodies[i].center[1] = dy[i];
            bodies[i].speed[0] = Random() * 2 - 1;
            bodies[i].speed[1] = Random() * 2 - 1;
		} else {
            int idx = std::min(int(Random() * 4), 3);
            bodies[i].mass = Random();
            bodies[i].center[0] = dx[idx] + Random() * 2 - 1;
            bodies[i].center[1] = dy[idx] + Random() * 2 - 1;
            bodies[i].speed[0] = Random() * 2 - 1;
            bodies[i].speed[1] = Random() * 2 - 1;
		}
		total_mass += bodies[i].mass;
        mvx += bodies[i].mass * bodies[i].speed[0];
        mvy += bodies[i].mass * bodies[i].speed[1];
	}
    mvx /= total_mass;
    mvy /= total_mass;

    for (Body &obj : bodies) {
        obj.speed[0] -= mvx;
        obj.speed[1] -= mvy;
        obj.radius = obj.mass / total_mass * 100;
        obj.mass   = obj.mass / total_mass * 20.0;
    }
}
void initialize_solarsystem(std::vector<Body> &bodies, int N) {
    assert(N == 9);

    bodies[0] = Body(1.989e30, 696340e3,        0, 0, 0,       0);  // Sun
    bodies[1] = Body(3.285e23, 2439.7e3,  57.91e9, 0, 0, 47.36e3);  // Mercury
    bodies[2] = Body(4.867e24, 6051.8e3, 108.21e9, 0, 0, 35.02e3);  // Venus
    bodies[3] = Body(5.972e24, 6371.0e3, 149.60e9, 0, 0, 29.78e3);  // Earth
    bodies[4] = Body(6.417e23, 3389.5e3, 227.92e9, 0, 0, 24.07e3);  // Mars
    bodies[5] = Body(1.898e27,  69911e3, 778.57e9, 0, 0, 13.07e3);  // Jupiter
    bodies[6] = Body(5.683e26,  58232e3, 1.434e12, 0, 0,  9.68e3);  // Saturn
    bodies[7] = Body(8.681e25,  25362e3, 2.871e12, 0, 0,  6.80e3);  // Uranus
    bodies[8] = Body(1.024e26,  24622e3, 4.495e12, 0, 0,  5.43e3);  // Neptune
}

bool check_collision(const Body &a, const Body &b) {
    double distance = (a.center - b.center).norm();
    return distance <= std::max(a.radius, b.radius);
}