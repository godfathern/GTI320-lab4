/**
 * @file Armature.cpp
 *
 * @brief Class de l'armature.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */
#include "Armature.h"

#include <unistd.h>


using namespace gti320;

namespace {
    // Map euler angles to the range [-pi,pi]
    //
    template<typename Scalar>
    static Scalar wrapToPi(Scalar a) {
        while (a > M_PI) a -= 2.0 * M_PI;
        while (a < -M_PI) a += 2.0 * M_PI;
        return a;
    }

    template<typename Scalar>
    static Scalar makeClose(Scalar a, Scalar ref) {
        a = wrapToPi(a);
        Scalar d = a - ref;
        a -= std::round(d / (2.0 * M_PI)) * (2.0 * M_PI);
        return a;
    }

    // Map each euler angle close to the reference angles @a prev.
    //
    template<typename Scalar>
    static Vector<Scalar, 3> makeEulerCloseTo(const Vector<Scalar, 3> &e, const Vector<Scalar, 3> &prev) {
        Vector<Scalar, 3> ee;
        ee(0) = makeClose(e(0), prev(0));
        ee(1) = makeClose(e(1), prev(1));
        ee(2) = makeClose(e(2), prev(2));
        return ee;
    }

    // Returns Euler angles based on rotation matrix @a R.
    // Code is based on Ken Shoemake's Graphics Gems IV article.
    //
    template<typename Scalar>
    Vector<Scalar, 3> rotationMatrixToEulerXYZ(const Matrix<Scalar, 3, 3> &R) {
        Vector<Scalar, 3> euler;

        Scalar sy = std::min(1.0f, std::max(-1.0f, -R(2, 0)));
        euler(1) = std::asin(sy);
        const Scalar cy = std::cos(euler(1));
        if (std::abs(cy) > 1e-3) {
            euler(0) = std::atan2(R(2, 1), R(2, 2));
            euler(2) = std::atan2(R(1, 0), R(0, 0));
        } else {
            euler(2) = 0.0;
            euler(0) = std::atan2(-R(0, 1), R(1, 1));
        }

        return euler;
    }
}

// Constructor.
Link::Link(const std::string &_name, Link *_parent, const Vector3f &_eulerAng, const Vector3f &_trans) : name(_name),
    parent(_parent), eulerAng(_eulerAng), trans(_trans), endEffector(false) {
    if (parent != nullptr) {
        parent->enfants.push_back(this);
    }
    M.setIdentity();
}

void Link::forward() {
    // TODO Create a rotation matrix from the Euler angles
    //      of the current link.
    Matrix3f rotM = makeRotation(eulerAng(0), eulerAng(1), eulerAng(2));

    // TODO Create a local 4D rigid transformation matrix from the
    //      3D rotation matrix and translation of the current link.
    Matrix4f T;
    T.setZero();
    T(3, 3) = 1;
    T.block(0,0,3,3) = rotM;

    for (int i = 0; i < 3; ++i) {
        T(i, 3) = trans(i);
    }

    // TODO Update the global transformation for the link using the
    //      parent's rigid transformation matrix and the local transformation.
    //      Hint : the parent matrix should be post-multiplied.
    //      Hint : the root does not have a parent. You must consider this case.
    if (isRoot()) {
        M = T;
    } else {
        M = parent->M * T;
    }

    // TODO Update the transformation of child links
    // by recursion.
    if (!isEndEffector()) {
        for (Link* child : enfants) {
            child->forward();
        }
    }

}


Armature::Armature() : links(), root(nullptr) {
}

Armature::~Armature() {
    for (Link *l: links) {
        delete l;
    }
}

void Armature::updateKinematics() {
    assert(root != nullptr);

    root->forward();
}

void Armature::pack(const Vector<int, Dynamic> &ind, Vector<float, Dynamic> &theta) {
    // TODO Collect the Euler angles of each link and put them
    //      into the dense vector @a theta
    int p = 0;
    for (int k = 0; k < ind.size(); ++k) {
        Link* l = links[ind(k)];
        for (int i = 0; i < 3; ++i) {
            theta(3 * p + i) = l->eulerAng(i);
        }
        p++;
    }
}

void Armature::unpack(const Vector<int, Dynamic> &ind, const Vector<float, Dynamic> &theta) {
    // TODO Extract the Euler angles contained in the
    //      dense vector @a theta and update the angles
    //      for each link in the armature.
    //
    int p = 0;
    for (int k = 0; k < ind.size(); ++k) {
        Link* l = links[ind(k)];
        for (int i = 0; i < 3; ++i) {
            l->eulerAng(i) = theta(3 * p + i);
        }
        p++;
    }
}

void Armature::removeChild(Link* parent, Link* child)
{
    auto& children = parent->enfants;
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}

std::vector<Link*> Armature::buildPathToRoot(Link* start, Link* stop)
{
    std::vector<Link*> path;
    Link* current = start;

    while (current != nullptr) {
        path.push_back(current);
        if (current == stop) break;
        current = current->parent;
    }

    return path;
}

void Armature::reverseLinksDuPath(const std::vector<Link*>& path)
{
    for (int i = (int)path.size() - 2; i >= 0; --i) {
        Link* child = path[i];
        Link* par   = path[i + 1];

        assert(child->parent == par);
        removeChild(par, child);

        par->parent = child;
        child->enfants.push_back(par);
    }
}


void Armature::reroot(Link *newRoot) {
    // TODO update the graph of links between @a newRoot and
    // the previous @a root. Edges should be swapped, where
    // children become parents and parents become children.
    //
    // A reviserr
    if (newRoot == nullptr || newRoot == root) {
        return;
    }

    Link* oldRoot = root;

    std::vector<Link*> path = buildPathToRoot(newRoot, oldRoot);

    assert(path.back() == oldRoot);

    // TODO revisit all links, starting at @a newRoot, and
    // recompute their translation and eulerAng relative to the parent.
    // This can be done by computing the local transform matrix from
    // parent to child using their global transforms stored in @a M.
    //
    // Then, extract the relative translation and euler angles from
    // the 4x4 matrix.  Note that may use the rotationMatrixToEulerXYZ
    // function to help.
    //
    //
    reverseLinksDuPath(path);

    newRoot->parent = nullptr;
    root = newRoot;

    std::vector<Link*> stack;
    stack.push_back(newRoot);

    while (!stack.empty()) {
        Link* node = stack.back();
        stack.pop_back();

        Matrix4f Tlocal;
        if (node->parent == nullptr) {
            Tlocal = node->M;
        } else {
            Tlocal = node->parent->M.inverse() * node->M;
        }

        for (int i = 0; i < 3; ++i) {
            node->trans(i) = Tlocal(i, 3);
        }

        Matrix3f R = Tlocal.block(0, 0, 3, 3);
        Vector3f e = rotationMatrixToEulerXYZ(R);

        node->eulerAng = makeEulerCloseTo<float>(e, node->eulerAng);

        for (Link* c : node->enfants) {
            stack.push_back(c);
        }
    }



    // Finally, update all the global transforms.
    updateKinematics();
}
