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
        obj.center.mass = Random() * 40.0 / N;
        obj.radius = 5;
        
        obj.center.pos[0] = Random() * 2 - 1;
        obj.center.pos[1] = Random() * 2 - 1;

		obj.speed[0] = Random() * 2 - 1;
		obj.speed[1] = Random() * 2 - 1;

        mvx += obj.speed[0] * obj.center.mass;
        mvy += obj.speed[1] * obj.center.mass;

        total_mass += obj.center.mass;
    }
    for (Body &obj : bodies) {
        obj.radius = obj.center.mass / total_mass * 50;
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
            bodies[i].center.mass = Random() + 20;
            bodies[i].center.pos[0] = dx[i];
            bodies[i].center.pos[1] = dy[i];
            bodies[i].speed[0] = Random() * 2 - 1;
            bodies[i].speed[1] = Random() * 2 - 1;
		} else {
            int idx = std::min(int(Random() * 4), 3);
            bodies[i].center.mass = Random();
            bodies[i].center.pos[0] = dx[idx] + Random() * 2 - 1;
            bodies[i].center.pos[1] = dy[idx] + Random() * 2 - 1;
            bodies[i].speed[0] = Random() * 2 - 1;
            bodies[i].speed[1] = Random() * 2 - 1;
		}
		total_mass += bodies[i].center.mass;
        mvx += bodies[i].center.mass * bodies[i].speed[0];
        mvy += bodies[i].center.mass * bodies[i].speed[1];
	}
    mvx /= total_mass;
    mvy /= total_mass;

    for (Body &obj : bodies) {
        obj.speed[0] -= mvx;
        obj.speed[1] -= mvy;
        obj.radius = obj.center.mass / total_mass * 50;
        obj.center.mass = obj.center.mass / total_mass * 20.0;
    }
}
void initialize_solarsystem(std::vector<Body> &bodies, int N) {
    assert(N == 9);

    bodies[0] = Body(1.0,     1.6491e-1,      0, 0, 0,     0);  // Sun
    bodies[1] = Body(1.66e-7, 1.6315e-5,  0.387, 0, 0, 1.581);  // Mercury
    bodies[2] = Body(2.45e-6, 4.7850e-5,  0.723, 0, 0, 1.176);  // Venus
    bodies[3] = Body(3.00e-6, 4.2635e-5,  1.0,   0, 0, 1.0);    // Earth
    bodies[4] = Body(3.22e-7, 2.2662e-5,  1.524, 0, 0, 0.802);  // Mars
    bodies[5] = Body(9.55e-4, 0.4672e-1,  5.204, 0, 0, 0.438);  // Jupiter
    bodies[6] = Body(2.85e-4, 0.3885e-1,  9.583, 0, 0, 0.323);  // Saturn
    bodies[7] = Body(4.37e-5, 0.1691e-1, 19.229, 0, 0, 0.227);  // Uranus
    bodies[8] = Body(5.15e-5, 0.1643e-1, 30.07,  0, 0, 0.182);  // Neptune

    SCALE = 30;
}
void computeAcceleration(const Particle &a, const Particle &b, Vector &acc) {
    double dx = b.pos[0] - a.pos[0];
    double dy = b.pos[1] - a.pos[1];
    double dist = sqrt(dx * dx + dy * dy + 0.01);

    double acceleration = G * b.mass / (dist * dist * dist);

    acc[0] += acceleration * dx;
    acc[1] += acceleration * dy;
}

bool check_collision(const Body &a, const Body &b) {
    double distance = (a.center.pos - b.center.pos).norm();
    return distance <= std::max(a.radius, b.radius);
}