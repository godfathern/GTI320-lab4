/**
 * @file IKSolver.cpp
 *
 * @brief Class du solveur cinématique inverse
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */


#include "IKSolver.h"

#include <unistd.h>

#include "Armature.h"
#include "SVD.h"


using namespace gti320;

namespace
{
}

IKSolver::IKSolver(Armature* _armature, Vector3f& _targetPos) : m_armature(_armature), m_targetPos(_targetPos), m_J()
{
}

namespace
{
    // NOUVELLE
    // Helper pour extraire la position globale d'un link a partir de sa matrice transformation M
    Vector3f getPosition(const Link* link)
    {
        Vector3f position;
        position(0) = link->M(0, 3);
        position(1) = link->M(1, 3);
        position(2) = link->M(2, 3);
        return position;
    }
}

namespace
{
    // NOUVELLE
    // Helper utilitaire pour calculer tmp = S^+ * U^T * dx.
    Vector<float, Dynamic> calculeTMP(
        const Matrix<float, 3>& U,
        const Vector<float, Dynamic>& sigma,
        const Vector3f& dx)
    {
        Vector<float, Dynamic> tmp(3);
        tmp.setZero();

        for (int i = 0; i < 3; ++i) {
            float uiTdx = 0;

            for (int k = 0; k < 3; ++k) {
                uiTdx += U(k, i) * dx(k);
            }

            if (std::abs(sigma(i)) > 0) {
                tmp(i) = uiTdx / sigma(i);
            } else {
                tmp(i) = 0;
            }
        }

        return tmp;
    }
}

namespace
{
    // NOUVELLE
    // Helper utilitaire pour calculer deltaTheta = V * tmp.
    Vector<float, Dynamic> calculeDeltaTheta(
        const Matrix<float, Dynamic, Dynamic, ColumnStorage>& V,
        const Vector<float, Dynamic>& tmp,
        int dim)
    {
        Vector<float, Dynamic> deltaTheta(dim);
        deltaTheta.setZero();

        for (int i = 0; i < dim; ++i) {
            float sum = 0;
            for (int j = 0; j < 3; ++j) {
                sum += V(i, j) * tmp(j);
            }
             deltaTheta(i) = sum;
        }
        return deltaTheta;
    }
}



float IKSolver::getError(Vector3f& dx) const
{
    // TODO Compute the error between the current end effector 
    //      position and the target position
    Link* endEffector = m_armature->links[m_armature->links.size() - 1];

    Vector3f position = getPosition(endEffector);

    dx = m_targetPos - position;
    return dx.norm();
}

void IKSolver::solve()
{
    //Dernier diapo du cours 8 : m = 3 x n(n nombre de links - link de l effecteur terminal)
    const int numLinks = m_armature->links.size();
    const int dim = 3 * (numLinks);
    m_J.resize(3, dim);

    //Assurer que tous sont a jour
    m_armature->updateKinematics();

    // Find the end effector.  Assume only one.
    Link* endEffector = nullptr;

    endEffector = m_armature->links[numLinks - 1];

    //Extraire la position de effector terminal :
    Vector3f position = getPosition(endEffector);

    // TODO Build the Jacobian matrix m_J.
    //      Each column corresponds to a separate
    //Slide 29 du cours 8 : jacob = wi x ri
    m_J.setZero();

    std::vector<Link*> links = m_armature->links;
    for (int i = 0; i < numLinks; ++i) {
        Link* link = links[i];

        Vector3f t;
        t(0) = link->M(0,3);
        t(1) = link->M(1,3);
        t(2) = link->M(2,3);

        //Slide 27 du cours 8 :
        Vector3f r = position - t;

        Vector3f wx,wy, wz;
        wx(0) = link->M(0, 0);
        wx(1) = link->M(1, 0);
        wx(2) = link->M(2, 0);

        wy(0) = link->M(0, 1);
        wy(1) = link->M(1, 1);
        wy(2) = link->M(2, 1);

        wz(0) = link->M(0, 2);
        wz(1) = link->M(1, 2);
        wz(2) = link->M(2, 2);

        //Slide 29 du cours 8 :
        Vector3f cx = crossP(wx, r);
        Vector3f cy = crossP(wy, r);
        Vector3f cz = crossP(wz, r);

        // m_J.setColumn3(3 * i + 0, cx(0), cx(1), cx(2));
        // m_J.setColumn3(3 * i + 1, cy(0), cy(1), cy(2));
        // m_J.setColumn3(3 * i + 2, cz(0), cz(1), cz(2));

        auto Ji = m_J.block(0, 3 * i, 3, 3);

        Ji(0, 0) = cx(0);  Ji(1, 0) = cx(1);  Ji(2, 0) = cx(2);
        Ji(0, 1) = cy(0);  Ji(1, 1) = cy(1);  Ji(2, 1) = cy(2);
        Ji(0, 2) = cz(0);  Ji(1, 2) = cz(1);  Ji(2, 2) = cz(2);
    }


    // TODO Compute the error between the current end effector 
    //      position and the target position by calling getError()
    //
    //dx : deltaX
    Vector3f dx;
    float err0 = getError(dx);

    // TODO Compute the change in the joint angles by solving:
    //    df/dtheta * delta_theta = delta_x
    //  where df/dtheta is the Jacobian matrix.
    //    
    //

    //J = U * S * V^T
    SVD<float, 3, Dynamic, ColumnStorage> svd(m_J);
    svd.decompose();

    const auto& U = svd.getU();
    const auto& V = svd.getV();
    const auto& sigma = svd.getSigma();


    // Slide 30 cours 8 :
    // J * deltaTheta = dx et J = U * S * V^T
    // => deltaTheta = V * S^+ * U^T * dx  = V * tmp

    // tmp = S^+ * U^T * dx
    Vector<float, Dynamic> tmp = calculeTMP(U, sigma, dx);

    // deltaTheta = V * tmp
    Vector<float, Dynamic> deltaTheta = calculeDeltaTheta(V, tmp, dim);

    // TODO Perform gradient descent method with line search
    //      to move the end effector toward the target position.
    //
    //   Hint: use the Armature::unpack() and Armature::pack() functions
    //   to set and get the joint angles of the armature.
    // 
    //   Hint: whenever you change the joint angles, you must also call
    //   armature->updateKinematics() to compute the global position.
    //
    Vector<int, Dynamic> ind(numLinks);
    for (int i = 0; i < numLinks; ++i) {
        ind(i) = i;
    }

    Vector<float, Dynamic> theta(dim);
    m_armature->pack(ind, theta);

    float alpha = 1;
    bool accepted = false;
    Vector<float, Dynamic> thetaNew(dim);

    //50 iterations
    for (int iter = 0; iter < 50; ++iter) {
        for (int i = 0; i < dim; ++i) {
            thetaNew(i) = theta(i) + alpha * deltaTheta(i);
        }

        m_armature->unpack(ind, thetaNew);
        m_armature->updateKinematics();

        Vector3f dxNew;
        float errNew = getError(dxNew);

        if (errNew < err0) {
            accepted = true;
            break;
        }
        alpha *= 0.5;
    }

    if (!accepted) {
        m_armature->unpack(ind, theta);
        m_armature->updateKinematics();
    }

}
