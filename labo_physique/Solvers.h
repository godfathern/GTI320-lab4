#pragma once

/**
 * @file Solvers.h
 *
 * @brief Implémentation de plusieurs algorihtmes de solveurs pour un système
 *        d'équations linéaires
 *
 * Nom:
 * Code permanent :
 * Email :
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
