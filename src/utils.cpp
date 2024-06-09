#include "../include/utils.h"
#include <assert.h>

#define MODULUS    2147483647
#define MULTIPLIER 48271
#define DEFAULT    123456789

static long seed = DEFAULT;
double SCALE = 1.0;

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
    SCALE = 200;

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
    SCALE = 100;

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

    bodies[0] = Body(1.0,     4.6491e-3,      0, 0, 0,     0);  // Sun
    bodies[1] = Body(1.66e-7, 1.6315e-5,  0.387, 0, 0, 1.581);  // Mercury
    bodies[2] = Body(2.45e-6, 4.7850e-5,  0.723, 0, 0, 1.176);  // Venus
    bodies[3] = Body(3.00e-6, 4.2635e-5,  1.0,   0, 0, 1.0);    // Earth
    bodies[4] = Body(3.22e-7, 2.2662e-5,  1.524, 0, 0, 0.802);  // Mars
    bodies[5] = Body(9.55e-4, 0.4672,     5.204, 0, 0, 0.438);  // Jupiter
    bodies[6] = Body(2.85e-4, 0.3885,     9.583, 0, 0, 0.323);  // Saturn
    bodies[7] = Body(4.37e-5, 0.1691,    19.229, 0, 0, 0.227);  // Uranus
    bodies[8] = Body(5.15e-5, 0.1643,    30.07,  0, 0, 0.182);  // Neptune

    SCALE = 30;
}

bool check_collision(const Body &a, const Body &b) {
    double distance = (a.center - b.center).norm();
    return distance <= std::max(a.radius, b.radius);
}