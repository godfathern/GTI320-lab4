/**
 * @file TestsPerformance.cpp
 *
 * @brief Tests unitaires 
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "Matrix.h"
#include "Vector.h"
#include "Operators.h"

#include <gtest/gtest.h>
#include <chrono>

using namespace gti320;

namespace {
    /**
     * Multiplication  matrice * vecteur,  utilisant une implémentation naive
     */
    template<typename _Scalar>
    static inline Vector<_Scalar, Dynamic> naiveMatrixMult(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Vector<_Scalar, Dynamic>& v)
    {
        assert(A.cols() == v.rows());

        Vector<_Scalar, Dynamic> b(A.rows());
        assert(b.rows() == A.rows());

        for (int i = 0; i < A.rows(); ++i) {
            b(i) = 0.0;
            for (int j = 0; j < A.cols(); ++j) {
                b(i) += A(i, j) * v(j);
            }
        }

        return b;
    }

    /**
     * Addition  matrice + matrice,  utilisant une implémentation naive
     */
    template<typename _Scalar>
    static inline Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage> naiveMatrixAddition(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        assert(A.cols() == B.cols() && A.rows() == B.rows());

        Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage> C(A.rows(), A.cols());
        assert(C.rows() == A.rows() && C.cols() == A.cols());
        for (int i = 0; i < C.rows(); ++i) {
            for (int j = 0; j < C.cols(); ++j) {
                C(i, j) = A(i, j) + B(i, j);
            }
        }
        return C;
    }

    /**
     * Multiplication  matrice * matrice,  utilisant une implémentation naive.
     */
    template<typename _Scalar, int _Storage>
    static inline Matrix<_Scalar, Dynamic, Dynamic, _Storage> naiveMatrixMult(const Matrix<_Scalar, Dynamic, Dynamic, _Storage>& A, const Matrix<_Scalar, Dynamic, Dynamic, _Storage>& B)
    {
        assert(A.cols() == B.rows());
        Matrix<_Scalar, Dynamic, Dynamic> product(A.rows(), B.cols());
        for (int i = 0; i < A.rows(); ++i) {
            for (int j = 0; j < B.cols(); ++j) {
                for (int k = 0; k < A.cols(); ++k) {
                    product(i, j) += A(i, k) * B(k, j);
                }
            }
        }
        return product;
    }
} // namespace

/**
 * Test des performance de la multiplication  matrice * vecteur
 * pour de grandes dimensions.
 */
TEST(TestsPerformance, PerformanceMatrixVector)
{
    Matrix<double> A(16384, 16384); // grande matrice avec stockage colonne
    Vector<double> v(16384);        // grand vecteur

    using namespace std::chrono;
    // Test : multiplication avec l'algorithme naif.
    high_resolution_clock::time_point t = high_resolution_clock::now();
    naiveMatrixMult(A, v);
    const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    // Test : multiplication avec l'implémentation spécifique pour les matrices avec
    // stockage par colonnes.
    t = high_resolution_clock::now();
    A * v;
    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    EXPECT_TRUE(optimal_t < 0.4 * naive_t)
        << "Naive time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms, "
        << "optimized time: " << duration_cast<std::chrono::milliseconds>(optimal_t).count() << " ms";
}

/**
 * Test des performances de l'addition  matrice + matrice
 * pour de grandes dimensions.
 */
TEST(TestsPerformance, PerformanceLargeMatrixMatrix)
{
    // deux grandes matrices à stockage par colonnes
    Matrix<double> A(16384, 16384);
    Matrix<double> B(16384, 16384);

    using namespace std::chrono;
    high_resolution_clock::time_point t = high_resolution_clock::now();
    // Test : addition avec l'algorithme naif
    naiveMatrixAddition(A, B);
    const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    // Test : addition avec l'implémentation spécifique pour les matrices à
    // stockage par colonnes.
    t = high_resolution_clock::now();
    A + B;
    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);

    EXPECT_TRUE(optimal_t < 0.4 * naive_t);
}
