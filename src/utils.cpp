#include "../include/utils.h"

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
    double vx = 0;
    double vy = 0;

    for (Body &obj : bodies) {
        obj.mass = 20.0 / N;
        obj.radius = 5;

		obj.center[0] = Random() * 2 - 1;
        obj.center[1] = Random() * 2 - 1;

		obj.speed[0] = Random() * 2 - 1;
		obj.speed[1] = Random() * 2 - 1;

        vx += obj.speed[0] * obj.mass;
        vy += obj.speed[1] * obj.mass;

        total_mass += obj.mass;
    }
    vx /= total_mass;
    vy /= total_mass;

    for (Body &obj : bodies) {
        obj.speed[0] -= vx;
        obj.speed[1] -= vy;
    }
}
void initialize_4galaxies(std::vector<Body> &bodies, int N) {
    double total_mass = 0;
    double mvx = 0;
    double mvy = 0;

    for (int i = 0 ; i < N ; ++i) {
		if (i < 4) {
            bodies[i].mass = Random() + 5;
            bodies[i].center[0] = dx[i];
            bodies[i].center[1] = dy[i];
            bodies[i].speed[0] = Random();
            bodies[i].speed[1] = Random();
		} else {
            int idx = rand() % 4;
            bodies[i].mass = Random();
            bodies[i].center[0] = dx[i] + Random() * 2 - 1;
            bodies[i].center[1] = dy[i] + Random() * 2 - 1;
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
        obj.mass = obj.mass / total_mass * 20.0;
    }
}


bool check_collision(const Body &a, const Body &b) {
    double distance = (a.center - b.center).norm();
    return distance <= a.radius + b.radius;
}