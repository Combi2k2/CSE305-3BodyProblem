#include "include/struct_body.h"
#include "include/struct_vector.h"
#include "include/constant.h"

void compute_force_2_particles(Body &p1, Body &p2, Vector &force)
// Compute the interacting force between 2 bodies
{
  double p1x = p1.center[0];
  double p1y = p1.center[1];
  double p2x = p2.center[0];
  double p2y = p2.center[1];
  if (p1x == p2x && p1y == p2y)
  {
    // 2 bodies are at the same position then no force exerted
    force[0] = 0;
    force[1] = 0;
  }
  else
  {
    double d = sqrt(pow((p1x - p2x), 2) + pow((p1y - p2y), 2));
    double force_gratitude = G * p1.mass * p2.mass / pow(d, 2);
    double fx = force_gratitude * (p2x - p1x) / d;
    double fy = force_gratitude * (p2y - p1y) / d;
    force[0] = fx;
    force[1] = fy;
  }
}