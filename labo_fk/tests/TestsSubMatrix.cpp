/**
 * @file TestsSubMatrix.cpp
 *
 * @brief Tests unitaires pour la classe SubMatrix
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "SubMatrix.h"

#include <gtest/gtest.h>

using namespace gti320;


TEST(TestLabo, AffectationBySubMatrix)
{
    {
        Matrix<double, 4, 4, ColumnStorage> m;
        m.setIdentity();
        EXPECT_DOUBLE_EQ(m(0, 0), 1);
        EXPECT_DOUBLE_EQ(m(0, 1), 0);
        EXPECT_DOUBLE_EQ(m(0, 2), 0);
        EXPECT_DOUBLE_EQ(m(0, 3), 0);
        EXPECT_DOUBLE_EQ(m(1, 0), 0);
        EXPECT_DOUBLE_EQ(m(1, 1), 1);
        EXPECT_DOUBLE_EQ(m(1, 2), 0);
        EXPECT_DOUBLE_EQ(m(1, 3), 0);
        EXPECT_DOUBLE_EQ(m(2, 0), 0);
        EXPECT_DOUBLE_EQ(m(2, 1), 0);
        EXPECT_DOUBLE_EQ(m(2, 2), 1);
        EXPECT_DOUBLE_EQ(m(2, 3), 0);
        EXPECT_DOUBLE_EQ(m(3, 0), 0);
        EXPECT_DOUBLE_EQ(m(3, 1), 0);
        EXPECT_DOUBLE_EQ(m(3, 2), 0);
        EXPECT_DOUBLE_EQ(m(3, 3), 1);
        Matrix<double, 4, 1, RowStorage> c;
        c(0, 0) = 1.0;
        c(1, 0) = 2.0;
        c(2, 0) = 3.0;
        c(3, 0) = 4.0;
        m.block(0, 0, 4, 1) = c;
        m.block(0, 1, 4, 1) = c;
        m.block(0, 2, 4, 1) = c;
        m.block(0, 3, 4, 1) = c;
        EXPECT_DOUBLE_EQ(m(0, 0), 1);
        EXPECT_DOUBLE_EQ(m(0, 1), 1);
        EXPECT_DOUBLE_EQ(m(0, 2), 1);
        EXPECT_DOUBLE_EQ(m(0, 3), 1);
        EXPECT_DOUBLE_EQ(m(1, 0), 2);
        EXPECT_DOUBLE_EQ(m(1, 1), 2);
        EXPECT_DOUBLE_EQ(m(1, 2), 2);
        EXPECT_DOUBLE_EQ(m(1, 3), 2);
        EXPECT_DOUBLE_EQ(m(2, 0), 3);
        EXPECT_DOUBLE_EQ(m(2, 1), 3);
        EXPECT_DOUBLE_EQ(m(2, 2), 3);
        EXPECT_DOUBLE_EQ(m(2, 3), 3);
        EXPECT_DOUBLE_EQ(m(3, 0), 4);
        EXPECT_DOUBLE_EQ(m(3, 1), 4);
        EXPECT_DOUBLE_EQ(m(3, 2), 4);
        EXPECT_DOUBLE_EQ(m(3, 3), 4);
        Matrix<double, 2, 2, ColumnStorage> b;
        b(0, 0) = 0.0;
        b(1, 0) = 1.0;
        b(0, 1) = 2.0;
        b(1, 1) = 3.0;
        m.block(0, 0, 2, 2) = b;
        m.block(2, 0, 2, 2) = b;
        m.block(0, 2, 2, 2) = b;
        m.block(2, 2, 2, 2) = b;
        EXPECT_DOUBLE_EQ(m(0, 0), 0);
        EXPECT_DOUBLE_EQ(m(0, 1), 2);
        EXPECT_DOUBLE_EQ(m(0, 2), 0);
        EXPECT_DOUBLE_EQ(m(0, 3), 2);
        EXPECT_DOUBLE_EQ(m(1, 0), 1);
        EXPECT_DOUBLE_EQ(m(1, 1), 3);
        EXPECT_DOUBLE_EQ(m(1, 2), 1);
        EXPECT_DOUBLE_EQ(m(1, 3), 3);
        EXPECT_DOUBLE_EQ(m(2, 0), 0);
        EXPECT_DOUBLE_EQ(m(2, 1), 2);
        EXPECT_DOUBLE_EQ(m(2, 2), 0);
        EXPECT_DOUBLE_EQ(m(2, 3), 2);
        EXPECT_DOUBLE_EQ(m(3, 0), 1);
        EXPECT_DOUBLE_EQ(m(3, 1), 3);
        EXPECT_DOUBLE_EQ(m(3, 2), 1);
        EXPECT_DOUBLE_EQ(m(3, 3), 3);
    }
    {
        Matrix<double, 4, 4, RowStorage> m;
        m.setIdentity();
        EXPECT_DOUBLE_EQ(m(0, 0), 1);
        EXPECT_DOUBLE_EQ(m(0, 1), 0);
        EXPECT_DOUBLE_EQ(m(0, 2), 0);
        EXPECT_DOUBLE_EQ(m(0, 3), 0);
        EXPECT_DOUBLE_EQ(m(1, 0), 0);
        EXPECT_DOUBLE_EQ(m(1, 1), 1);
        EXPECT_DOUBLE_EQ(m(1, 2), 0);
        EXPECT_DOUBLE_EQ(m(1, 3), 0);
        EXPECT_DOUBLE_EQ(m(2, 0), 0);
        EXPECT_DOUBLE_EQ(m(2, 1), 0);
        EXPECT_DOUBLE_EQ(m(2, 2), 1);
        EXPECT_DOUBLE_EQ(m(2, 3), 0);
        EXPECT_DOUBLE_EQ(m(3, 0), 0);
        EXPECT_DOUBLE_EQ(m(3, 1), 0);
        EXPECT_DOUBLE_EQ(m(3, 2), 0);
        EXPECT_DOUBLE_EQ(m(3, 3), 1);
        Matrix<double, 4, 1, RowStorage> c;
        c(0, 0) = 1.0;
        c(1, 0) = 2.0;
        c(2, 0) = 3.0;
        c(3, 0) = 4.0;
        m.block(0, 0, 4, 1) = c;
        m.block(0, 1, 4, 1) = c;
        m.block(0, 2, 4, 1) = c;
        m.block(0, 3, 4, 1) = c;
        EXPECT_DOUBLE_EQ(m(0, 0), 1);
        EXPECT_DOUBLE_EQ(m(0, 1), 1);
        EXPECT_DOUBLE_EQ(m(0, 2), 1);
        EXPECT_DOUBLE_EQ(m(0, 3), 1);
        EXPECT_DOUBLE_EQ(m(1, 0), 2);
        EXPECT_DOUBLE_EQ(m(1, 1), 2);
        EXPECT_DOUBLE_EQ(m(1, 2), 2);
        EXPECT_DOUBLE_EQ(m(1, 3), 2);
        EXPECT_DOUBLE_EQ(m(2, 0), 3);
        EXPECT_DOUBLE_EQ(m(2, 1), 3);
        EXPECT_DOUBLE_EQ(m(2, 2), 3);
        EXPECT_DOUBLE_EQ(m(2, 3), 3);
        EXPECT_DOUBLE_EQ(m(3, 0), 4);
        EXPECT_DOUBLE_EQ(m(3, 1), 4);
        EXPECT_DOUBLE_EQ(m(3, 2), 4);
        EXPECT_DOUBLE_EQ(m(3, 3), 4);
        Matrix<double, 2, 2, ColumnStorage> b;
        b(0, 0) = 0.0;
        b(1, 0) = 1.0;
        b(0, 1) = 2.0;
        b(1, 1) = 3.0;
        m.block(0, 0, 2, 2) = b;
        m.block(2, 0, 2, 2) = b;
        m.block(0, 2, 2, 2) = b;
        m.block(2, 2, 2, 2) = b;
        EXPECT_DOUBLE_EQ(m(0, 0), 0);
        EXPECT_DOUBLE_EQ(m(0, 1), 2);
        EXPECT_DOUBLE_EQ(m(0, 2), 0);
        EXPECT_DOUBLE_EQ(m(0, 3), 2);
        EXPECT_DOUBLE_EQ(m(1, 0), 1);
        EXPECT_DOUBLE_EQ(m(1, 1), 3);
        EXPECT_DOUBLE_EQ(m(1, 2), 1);
        EXPECT_DOUBLE_EQ(m(1, 3), 3);
        EXPECT_DOUBLE_EQ(m(2, 0), 0);
        EXPECT_DOUBLE_EQ(m(2, 1), 2);
        EXPECT_DOUBLE_EQ(m(2, 2), 0);
        EXPECT_DOUBLE_EQ(m(2, 3), 2);
        EXPECT_DOUBLE_EQ(m(3, 0), 1);
        EXPECT_DOUBLE_EQ(m(3, 1), 3);
        EXPECT_DOUBLE_EQ(m(3, 2), 1);
        EXPECT_DOUBLE_EQ(m(3, 3), 3);
    }
}
