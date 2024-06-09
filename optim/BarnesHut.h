#include "../include/struct_body.h"
#include "../include/struct_vector.h"
#include "../include/constant.h"
#include "../include/utils.h"
#include <math.h>
#include <vector>
#include <thread>

struct Particle {
    Vector pos;
    double mass;

    Particle(const Vector &P = Vector(0, 0), const double &m = 0) : pos(P), mass(m) {}
};

struct TreeNode {
    Particle COM;
    Vector box_center;
    double box_size;
    TreeNode *children[4];
    int nParticles;

    TreeNode(const Vector &box_center = Vector(0, 0), const double &box_size = 1.0);
    ~TreeNode();

    inline Vector quad_center(const int &i) const;
    inline int quad_index(const Vector &P) const;
};

struct Optimizer_BarnesHut {
    TreeNode *root;

    Optimizer_BarnesHut() {}
    ~Optimizer_BarnesHut() {
        delete root;
    }
    void insert(const Particle &p);
    void query(const Particle &p, Vector &force, const double &theta = 0.5);
    void build(const std::vector<Particle> &particles);
};

// double compute_box_size(std::vector<Body> &particles)
// {
//     double xMin = particles[0].center[0], xMax = particles[0].center[0], yMin = particles[0].center[1], yMax = particles[0].center[1];
//     for (size_t i = 0; i < particles.size(); i++)
//     {
//         xMin = std::min(xMin, particles[i].center[0]);
//         xMax = std::max(xMax, particles[i].center[0]);
//         yMin = std::min(yMin, particles[i].center[1]);
//         yMax = std::max(yMax, particles[i].center[1]);
//     }
//     double box_size = std::max(std::max(abs(xMin), abs(xMax)), std::max(abs(yMin), abs(yMax))) * 4;
//     return box_size;
// }

// void simulate_single_thread(std::vector<Body> &particles, double theta)
// {
//     TreeNode *root = new TreeNode;
//     root->box_size = compute_box_size(particles);
//     build(root, particles);
//     std::vector<Vector> forces(particles.size(), Vector(0, 0));
//     for (int i = 0; i < particles.size(); i++)
//     {
//         root->compute_force(particles[i], theta, forces[i]);
//         particles[i].speed += forces[i] / particles[i].mass * DELTAT;
//         particles[i].center += particles[i].speed * DELTAT;
//     }
//     delete_tree(root);
// }

// void simulate_local(int begin, int end, std::vector<Body> &particles, std::vector<Vector> &forces, TreeNode *root, double theta)
// {
//     for (int i = begin; i < end; i++)
//     {
//         root->compute_force(particles[i], theta, forces[i]);
//         // particles[i].speed += forces[i] / particles[i].mass * DELTAT;
//         // particles[i].center += particles[i].speed * DELTAT;
//     }
// }

// void simulate_multi_thread(std::vector<Body> &particles, double theta, int num_thread)
// {
//     TreeNode *root = new TreeNode;
//     root->box_size = compute_box_size(particles);
//     build(root, particles);
//     std::vector<Vector> forces(particles.size(), Vector(0, 0));

//     int block_size = particles.size() / num_thread;
//     std::vector<std::thread> workers(num_thread - 1);
//     int start_block = 0;
//     for (int i = 0; i < num_thread - 1; i++)
//     {
//         int end_block = start_block + block_size;
//         workers[i] = std::thread(&simulate_local, start_block, end_block, std::ref(particles), std::ref(forces), root, theta);
//         start_block = end_block;
//     }
//     simulate_local(start_block, particles.size(), particles, forces, root, theta);
//     for (int i = 0; i < num_thread - 1; ++i)
//     {
//         workers[i].join();
//     }
//     delete_tree(root);
//     for (int i = 0; i < particles.size(); i++)
//     {
//         particles[i].center += DELTAT * particles[i].speed;
//         particles[i].speed += DELTAT / particles[i].mass * forces[i];
//     }
// }

// int getRandomNumber(int64_t min, int64_t max)
// {
//     return min + rand() % (max - min + 1);
// }

// void myinitBodies(std::vector<Body> &objs, size_t N)
// {
//     for (int i = 0; i < N; ++i)
//     {
//         objs[i].mass = getRandomNumber(1e10, 1e11);
//         objs[i].radius = 5;
//         int range_pos = 5;
//         int range_speed = 5;
//         objs[i].center[0] = getRandomNumber(-range_pos, range_pos) / 20;
//         objs[i].center[1] = getRandomNumber(-range_pos, range_pos) / 20;
//         // objs[i].speed[0] = getRandomNumber(-range_speed, range_speed);
//         // objs[i].speed[1] = getRandomNumber(-range_speed, range_speed);
//         objs[i].speed[0] = 0;
//         objs[i].speed[1] = 0;
//     }
// }

// int main(int argc, char **argv)
// {
//     std::vector<Body> objs(N);

//     myinitBodies(objs, N);

//     for (int t = 0; t < step; ++t)
//     {
//         simulate_single_thread(objs, N);

//         for (int i = 0; i < N; ++i)
//         {
//             // std::cerr << objs[i].speed[0] << " " << objs[i].speed[1] << "\n";
//             std::cout << objs[i].center[0] << " " << objs[i].center[1] << "\n";
//         }
//     }
// }