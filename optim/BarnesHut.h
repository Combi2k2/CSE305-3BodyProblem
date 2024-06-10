#include "../include/struct_body.h"
#include "../include/struct_vector.h"
#include "../include/constant.h"
#include "../include/utils.h"
#include <mutex>
#include <math.h>
#include <vector>
#include <thread>

struct TreeNode {
    Particle COM;
    Vector box_center;
    double box_size;
    TreeNode *children[4];
    int nParticles;
    std::mutex lock;

    TreeNode(double cx = 0, double cy = 0, double size = 1.0);
    TreeNode(const Vector &center = Vector(0, 0), double size = 1.0);
    ~TreeNode();

    inline Vector quad_center(const int &i) const;
    inline int quad_index(const Vector &P) const;
};

namespace Optimizer_BarnesHut {
    void setRoot(double x = 0, double y = 0, double size = 0);
    void insert(const Particle &p);
    void query(const Particle &p, Vector &acc, const double &theta = 0.5);
};