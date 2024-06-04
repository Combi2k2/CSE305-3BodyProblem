#include "include/struct_body.h"
#include "include/struct_vector.h"
#include "src/utils.h"
#include <vector>

class TreeNode
{
public:
  int num_particles;
  Vector box_center;
  double box_size;
  Body mass_center;
  Body first_individual;
  TreeNode *quadrants[4]; // Each node has 4 children, representing the 4 quadrants it is divided into

  TreeNode()
  {
    num_particles = 0;
    box_center = Vector(0, 0);
    box_size = pow(2, 32);
    mass_center = Body(1, 1, 0, 0, 0, 0);
    first_individual = Body(1, 1, 0, 0, 0, 0);
    for (int i = 0; i < 4; i++)
    {
      quadrants[i] = nullptr;
    }
  }

  TreeNode(Vector &_box_center, double &_box_size)
  {
    num_particles = 0;
    box_center = _box_center; // Make sure to check that all bodies lies in this boxWidth
    mass_center = Body(1, 1, 0, 0, 0, 0);
    first_individual = Body(1, 1, 0, 0, 0, 0);
    for (int i = 0; i < 4; i++)
    {
      quadrants[i] = nullptr;
    }
  }

  Vector quadrant_center(int quad)
  {
    // Compute the center of each quadrant given its index:
    // 0: NW, 1: NE, 2: SW, 3: SE
    double coord_x;
    double coord_y;
    Vector c;
    if (quad == 0)
    {
      coord_x = box_center[0] - box_size / 2;
      coord_y = box_center[1] + box_size / 2;
    }
    else if (quad == 1)
    {
      coord_x = box_center[0] + box_size / 2;
      coord_y = box_center[1] + box_size / 2;
    }
    else if (quad == 2)
    {
      coord_x = box_center[0] - box_size / 2;
      coord_y = box_center[1] - box_size / 2;
    }
    else
    {
      coord_x = box_center[0] + box_size / 2;
      coord_y = box_center[1] - box_size / 2;
    }
    c[0] = coord_x;
    c[1] = coord_y;
    return c;
  }

  int quad_index(Vector pos)
  {
    // Return the index of the quadrant that a particle at position pos belongs to
    if (pos[0] <= box_center[0] && pos[1] >= box_center[1])
      return 0;
    else if (pos[0] > box_center[0] && pos[1] >= box_center[1])
      return 1;
    else if (pos[0] <= box_center[0] && pos[1] < box_center[1])
      return 2;
    else
      return 3;
  }

  void compute_force(Body &subject, double theta, Vector &force)
  {
    if (num_particles == 1)
    {
      // External node
      compute_force_2_particles(subject, mass_center, force);
    }
    else
    {
      double d = (subject.center - mass_center.center).norm();
      double s = box_size;
      if (s / d < theta)
      {
        // The internal node is sufficiently far away, then treat it as a mass center
        compute_force_2_particles(subject, mass_center, force);
      }
      else
      {
        // The internal node is not far away, then traverse recursively to its children quadrant
        for (int i = 0; i < 4; i++)
        {
          if (quadrants[i] != nullptr)
          {
            Vector current_force(0, 0);
            quadrants[i]->compute_force(subject, theta, current_force);
            force = force + current_force;
          }
        }
      }
    }
  }

  void insert(TreeNode *node, Body &particle)
  {
    if (node->num_particles == 0)
    {
      // No particle yet, then add the new particle to the tree directly by assigning it to the first individual particle of the box
      node->first_individual = particle;
    }
    else if (node->num_particles > 1)
    {
      // Internal node, then update mass center and insert recursively
      Vector particle_position = particle.center;
      int ind = node->quad_index(particle_position);
      // Check the quadrant that the new particle belongs to
      if (node->quadrants[ind] == nullptr)
      {
        // If the quadrant found above is empty, create a new tree at this quadrant
        Vector quad_center = node->quadrant_center(ind);
        double quad_size = node->box_size / 2;
        node->quadrants[ind] = new TreeNode(quad_center, quad_size);
      }
      else
      {
        // If the quadrant is not empty, recursively insert the particle
        insert(node->quadrants[ind], particle);
      }
    }
    else
    {
      // External node, then subdivide the box into 4 quadrants and arrange the first individual and the new particle to their
      // appropriate quadrants based on their positions
      int first_particle_ind = node->quad_index(node->first_individual.center);
      if (node->quadrants[first_particle_ind] == nullptr)
      {
        Vector first_individual_quad_center = node->quadrant_center(first_particle_ind);
        double first_individual_quad_size = node->box_size / 2;
        node->quadrants[first_particle_ind] = new TreeNode(first_individual_quad_center, first_individual_quad_size);
      }
      else
      {
        insert(node->quadrants[first_particle_ind], node->first_individual);
      }
      // Reallocate the first individual particle to its quadrant

      int particle_ind = node->quad_index(particle.center);
      if (node->quadrants[particle_ind] == nullptr)
      {
        Vector quad_center = node->quadrant_center(particle_ind);
        double quad_size = node->box_size / 2;
        node->quadrants[particle_ind] = new TreeNode(quad_center, quad_size);
      }
      else
      {
        insert(node->quadrants[particle_ind], particle);
      }
      // Reallocate the new particle to its quadrant
    }
    node->num_particles += 1;
    node->mass_center = node->mass_center + particle;
    // Update the mass and the position of the mass center of the box
  }

  void build(std::vector<Body> &particles, TreeNode *root_node)
  {
    for (int i = 0; i < particles.size(); i++)
    {
      insert(root_node, particles[i]);
    }
  }

  void delete_tree(TreeNode *node)
  {
    if (node != nullptr)
    {
      for (int quad = 0; quad < 4; quad++)
      {
        // Delete 4 quadrants children of the node
        delete_tree(node->quadrants[quad]);
      }
      delete node;
    }
  }
};