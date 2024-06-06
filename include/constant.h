#ifndef CONSTANT_H
#define CONSTANT_H

#define DEFAULT_N 10000   // Number of particles
#define DEFAULT_TIME 1000 // Number if iterations
#define G      1.0        // Gravitational constant, m3 kg-1 s-2
#define XBOUND 1.0e6      // Width of space
#define YBOUND 1.0e6      // Height of space
#define RBOUND 10         // Upper bound on radius
#define DELTAT 0.01       // Time increament
#define THETA 1.0         // Opening angle, for approximation in BH

// #define MASS_OF_JUPITER 1.899e27   //  Sample masses for testing
// #define MASS_OF_EARTH 5.974e24     //  and comparing the effect 
// #define MASS_OF_MOON 7.348e22      //  of using different values 
// #define MASS_OF_UNKNOWN 1.899e12   //  for m.

const double dx[] = {-1, 1, -1, 1};
const double dy[] = {-1, -1, 1, 1};

#endif