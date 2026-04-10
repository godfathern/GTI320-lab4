/**
 * @file TestsSparseMatrix.cpp
 *
 * @brief Tests unitaires 
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "SparseMatrix.h"
#include "Vector.h"
#include "Operators.h"

#include <gtest/gtest.h>

using namespace gti320;


TEST(TestsSparseMatrix, SparseMatrix)
{
    // Initialisation par tuples
    SparseMatrix<double> A(3, 3);
    TripletType<double> triplets[] = { { 1.0, 1, 1 }, {2.5, 2, 1}, {-0.1, 2, 2}, {6.0, 0, 0} };
    A.setFromTriplets(triplets, 4);

    Vector<double> v(3);
    v(0) = 1.0;
    v(1) = 2.0;
    v(2) = 3.0;

    // Test : vérifier les coefficients dans la matrice creuse A 
    {
        double a20 = A(2, 0);
        EXPECT_DOUBLE_EQ(a20, 0.0);

        double a00 = A(0, 0);
        EXPECT_DOUBLE_EQ(a00, 6.0);

        double a21 = A(2, 1);
        EXPECT_DOUBLE_EQ(a21, 2.5);
    }

    // Test : SparseMatrix * Vector dense 
    {
        const Vector<double> vout = A * v;

        EXPECT_DOUBLE_EQ(vout(0), 6.0);
        EXPECT_DOUBLE_EQ(vout(1), 2.0);
        EXPECT_DOUBLE_EQ(vout(2), 4.7);
    }

    // Test : SparseMatrix::setIdentity()
    {
        A.setIdentity();

        double a00 = A(0, 0);
        EXPECT_DOUBLE_EQ(a00, 1.0);

        double a21 = A(2, 1);
        EXPECT_DOUBLE_EQ(a21, 0.0);

        const Vector<double> vout = A * v;
        EXPECT_DOUBLE_EQ(vout(0), v(0));
        EXPECT_DOUBLE_EQ(vout(1), v(1));
        EXPECT_DOUBLE_EQ(vout(2), v(2));

    }

}
