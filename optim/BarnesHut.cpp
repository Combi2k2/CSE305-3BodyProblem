#include "BarnesHut.h"
#include <stack>

TreeNode::TreeNode(double cx, double cy, double size) {
    this->nParticles = 0;
    this->box_center = Vector(cx, cy);
    this->box_size   = size;
    this->COM = Particle();
    children[0] = children[1] = nullptr;
    children[2] = children[3] = nullptr;
}
TreeNode::TreeNode(const Vector &center, double size) {
    this->nParticles = 0;
    this->box_center = center;
    this->box_size   = size;
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

static TreeNode *root;

void Optimizer_BarnesHut::setRoot(double x, double y, double size) {
    root = new TreeNode(x, y, size);
}
void Optimizer_BarnesHut::insert(const Particle &p) {
    TreeNode *node = root;

    while (true) {
        std::lock_guard<std::mutex> lock(node->lock);

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