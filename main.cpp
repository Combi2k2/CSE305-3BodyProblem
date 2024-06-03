#include "include/constant.h"
#include "include/struct_vector.h"
#include "include/struct_body.h"

double force(const Body *o1, Body *o2) {
    return G * o1->m * o2->m / (o1->position - o2->position).norm2();
}

void update(Body *objs, size_t N) {
    for (size_t i = 0 ; i < N ; ++i) {
        Vector f(0, 0, 0);

        for (size_t j = 0 ; j < N ; ++j) {
            if (i == j) continue;

            double size = force(objs + i, objs + j);
            Vector dir = objs[j].position - objs[i].position;

            dir.normalize();

            f = f + dir * size;
        }
        objs[i].position = objs[i].position + DELTAT * objs[i].velocity;
        objs[i].velocity = objs[i].velocity + DELTAT * f * (1.0 / objs[i].m);
    }
}

int main() {
    const double MASS_OF_JUPITER = 1.899e27;
    const double MASS_OF_EARTH = 5.974e24;
    const double MASS_OF_MOON  = 7.348e22;
    
    
}