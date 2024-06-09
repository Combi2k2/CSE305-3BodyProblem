#include "BarnesHut.h"
#include <iostream>
#include <stack>

static void computeAcceleration(const Particle &a, const Particle &b, Vector &acc) {
    double dx = b.pos[0] - a.pos[0];
    double dy = b.pos[1] - a.pos[1];
    double dist = sqrt(dx * dx + dy * dy + 0.01);

    double acceleration = G * b.mass / (dist * dist * dist);

    acc[0] += acceleration * dx;
    acc[1] += acceleration * dy;
}

TreeNode::TreeNode(const Vector &box_center, const double &box_size) {
    this->nParticles = 0;
    this->box_center = box_center;
    this->box_size   = box_size;
    this->COM = Particle();
    children[0] = children[1] = nullptr;
    children[2] = children[3] = nullptr;
}
TreeNode::~TreeNode() {
    delete children[0];
    delete children[1];
    delete children[2];
    delete children[3];
}

Vector TreeNode::quad_center(const int &i) const {
    switch (i) {
        case 0:     return Vector(box_center[0] - box_size/2, box_center[1] - box_size/2);
        case 1:     return Vector(box_center[0] - box_size/2, box_center[1] + box_size/2);
        case 2:     return Vector(box_center[0] + box_size/2, box_center[1] - box_size/2);
        default:    return Vector(box_center[0] + box_size/2, box_center[1] + box_size/2);
    }
}
int TreeNode::quad_index(const Vector &P) const {
    if (P[0] <= box_center[0] && P[1] <= box_center[1])    return 0;
    if (P[0] <= box_center[0] && P[1] >= box_center[1])    return 1;
    if (P[0] >= box_center[0] && P[1] <= box_center[1])    return 2;
    
    return 3;
}

void Optimizer_BarnesHut::insert(const Particle &p) {
    TreeNode *node = root;

    while (true) {
        if (node->nParticles == 0) {
            node->nParticles = 1;
            node->COM = p;
            break;
        }
        if (node->nParticles == 1) {
            int idx = node->quad_index((node->COM).pos);

            node->children[idx] = new TreeNode(node->quad_center(idx), node->box_size / 2);
            node->children[idx]->COM = node->COM;
            node->children[idx]->nParticles = 1;
        }
        node->nParticles++;
        (node->COM).pos = ((node->COM).pos * (node->COM).mass + p.pos * p.mass) / ((node->COM).mass + p.mass);
        (node->COM).mass += p.mass;

        int idx = node->quad_index(p.pos);

        if (!node->children[idx])
            node->children[idx] = new TreeNode(node->quad_center(idx), node->box_size / 2);
        
        node = node->children[idx];
    }
}
void Optimizer_BarnesHut::query(const Particle &p, Vector &acc, const double &theta) {
    std::stack<TreeNode*> st;
    st.push(root);

    while (st.size()) {
        TreeNode *node = st.top();
        st.pop();

        if (node->nParticles == 1) {
            computeAcceleration(p, node->COM, acc);
            continue;
        }
        double s = node->box_size;
        double d = ((node->COM).pos - p.pos).norm();

        if (s < (theta - 1e-6) * d) {
            computeAcceleration(p, node->COM, acc);
        } else {
            if (node->children[0])  st.push(node->children[0]);
            if (node->children[1])  st.push(node->children[1]);
            if (node->children[2])  st.push(node->children[2]);
            if (node->children[3])  st.push(node->children[3]);
        }
    }
}
void Optimizer_BarnesHut::build(const std::vector<Particle> &particles) {
    double xMin = particles[0].pos[0], xMax = particles[0].pos[0];
    double yMin = particles[0].pos[1], yMax = particles[0].pos[1];

    for (const Particle &p : particles) {
        xMin = std::min(xMin, p.pos[0]);
        xMax = std::max(xMax, p.pos[0]);
        yMin = std::min(yMin, p.pos[1]);
        yMax = std::max(yMax, p.pos[1]);
    }
    double box_size = std::max({-xMin, xMax, -yMin, yMax}) * 2;
    root = new TreeNode(Vector(0, 0), box_size);

    for (const Particle &p : particles)
        insert(p);
}

int main() {
    int nPoints = 100;
    int nFrames = 500;

    std::vector<Body> bodies(nPoints);
    initialize_random(bodies, nPoints);

    for (int t = 0 ; t < nFrames ; ++t) {
        std::vector<Particle> vec;
        std::vector<Vector> acc(nPoints, Vector());

        for (const Body &b : bodies)
            vec.push_back(Particle(b.center, b.mass));
        
        Optimizer_BarnesHut optim;
        optim.build(vec);

        for (int i = 0 ; i < nPoints ; ++i) {
            optim.query(vec[i], acc[i]);

            bodies[i].speed  += DELTAT * acc[i];
            bodies[i].center += DELTAT * bodies[i].speed;
        }
        for (int i = 0 ; i < nPoints ; ++i)
            std::cout << bodies[i].center[0] << " " << bodies[i].center[1] << "\n";
    }
}