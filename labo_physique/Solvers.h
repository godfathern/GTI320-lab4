#include <vector>
#pragma once

/**
 * @file Solvers.h
 *
 * @brief Implémentation de plusieurs algorihtmes de solveurs pour un système
 *        d'équations linéaires
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Operators.h"
#include "Vector.h"

namespace gti320
{
    // Identification des solveurs
    enum eSolverType { kNone, kGaussSeidel, kColorGaussSeidel, kSparseGaussSeidel, kCholesky };

    // Paramètres de convergences pour les algorithmes itératifs
    static const float eps = 1e-4f;
    static const float tau = 1e-5f;

    /**
     * Résout Ax = b avec la méthode Gauss-Seidel
     */
    static void gaussSeidel(const Matrix<float, Dynamic, Dynamic>& A,
        const Vector<float, Dynamic>& b,
        Vector<float, Dynamic>& x, int k_max)
    {
        // TODO 
        //
        // Implémenter la méthode de Gauss-Seidel
        const int n = A.rows();

        if (x.rows() != n) {
            x.resize(n);
            x.setZero();
        }

        float normB = b.norm();

        for (int k = 0; k < k_max; ++k) {
            for (int i = 0; i < n; ++i)
            {
                float sum = b(i);
                for (int j = 0; j < i; ++j)
                {
                    sum = sum - A(i, j) * x(j);
                }

                for (int j = i + 1; j < n; ++j)
                {
                    sum = sum -  A(i, j) * x(j);
                }

                if (A(i, i) > 0) {
                    x(i) = sum / A(i, i);
                }
            }
            //Test de convergence :
            Vector<float, Dynamic> r = (A * x) - b;
            float normR = r.norm();
            if (normR < eps || (normB > 0 && (normR / normB) < tau)) {
                break;
            }
        }
    }

    /**
     * Résout Ax = b avec la méthode Gauss-Seidel (matrice creuse)
     */
    static void gaussSeidelSparse(const SparseMatrix<float, Dynamic, Dynamic>& A, const Vector<float, Dynamic>& b, Vector<float, Dynamic>& x, int maxIter)
    {    
        // TODO 
        //
        // Implémenter la méthode Gauss-Seidel utilisant la structure de données CRS. 
        // Note: votre implémentation ne doit pas utiliser l'opérateur operator(int, int).
        const int n = A.rows();
        if (x.rows() != n) {
            x.resize(n);
            x.setZero();
        }

        float normB = b.norm();

        for (int k = 0; k < maxIter; ++k)
        {
            for (int i = 0; i < n; ++i)
            {
                float sum = b(i);
                float diag = 0;

                const unsigned int begin = A.outer()[i];
                const unsigned int end = (i + 1 < n) ? A.outer()[i + 1] : A.getInnerSize();

                for (unsigned int k_ptr = begin; k_ptr < end; ++k_ptr)
                {
                    int j = A.inner()[k_ptr];
                    float val = A.values()[k_ptr];

                    if (i == j) {
                        diag = val;
                    } else {
                        sum = sum - val * x(j);
                    }
                }

                if (diag > 0) {
                    x(i) = sum / diag;
                }
            }

            // Test de convergence
            Vector<float, Dynamic> r = (A * x) - b;
            float normR = r.norm();
            if (normR < eps || (normB > 0 && (normR / normB) < tau)) {
                break;
            }
        }
    }


    /**

     * Résout Ax = b avec la méthode Gauss-Seidel (coloration de graphe)
     */
    static void gaussSeidelColor(const Matrix<float, Dynamic, Dynamic>& A, const Vector<float, Dynamic>& b, Vector<float, Dynamic>& x, const Partitions& P, const int maxIter)
    {
        // TODO 
        //
        // Implémenter la méthode de Gauss-Seidel avec coloration de graphe.
        // Les partitions avec l'index de chaque particule sont stockées dans la table des tables, P.
        const int n = A.rows();
        if (x.rows() != n) {
            x.resize(n);
            x.setZero();
        }

        float normB = b.norm();

        for (int k = 0; k < maxIter; ++k)
        {
            for (const auto& partition : P)
            {
                #pragma omp parallel for
                for (int i_part = 0; i_part < (int)partition.size(); ++i_part)
                {
                    int p_idx = partition[i_part];

                    for (int d = 0; d < 2; ++d)
                    {
                        int i = 2 * p_idx + d;
                        float sum = b(i);

                        for (int j = 0; j < n; ++j)
                        {
                            if (i != j) {
                                sum -= A(i, j) * x(j);
                            }
                        }

                        if (A(i, i) > 0) {
                            x(i) = sum / A(i, i);
                        }
                    }
                }
                // Test de convergence
                Vector<float, Dynamic> r = (A * x) - b;
                float normR = r.norm();
                if (normR < eps || (normB > 0 && (normR / normB) < tau)) {
                    break;
                }
            }
        }
    }

    /**
     * Résout Ax = b avec la méthode de Cholesky
     * Implementation de reference fournie.
     */
    static void cholesky(const Matrix<float, Dynamic, Dynamic>& A,
        const Vector<float, Dynamic>& b,
        Vector<float, Dynamic>& x)
    {
        const int n = A.rows();
        Matrix<float, Dynamic, Dynamic> L(n, n);
        L.setZero();

        for (int i = 0; i < n; ++i)
        {
            for (int k = 0; k <= i; ++k)
            {
                float s = 0;
                for (int j = 0; j < k; ++j)
                {
                    s += L(i, j) * L(k, j);
                }

                if (i == k)
                    L(i, k) = std::sqrt(A(i, i) - s);
                else
                    L(i, k) = (A(i, k) - s) / L(k, k);
            }
        }

        auto y = b;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < i; ++j)
                y(i) -= L(i, j) * y(j);
            y(i) = y(i) / L(i, i);
        }

        x = y;
        for (int i = n - 1; i >= 0; --i)
        {
            for (int j = i + 1; j < n; ++j)
                x(i) -= L(j, i) * x(j);
            x(i) = x(i) / L(i, i);
        }
    }

}
