/**
 * @file TestsDenseStorage.cpp
 *
 * @brief Tests unitaires 
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "DenseStorage.h"

#include <gtest/gtest.h>

using namespace gti320;

TEST(TestsDenseStorage, DenseStorage)
{
    // Test: tampon fixe size() et resize()
    {
        DenseStorage<float, 4> buf;
        EXPECT_EQ(buf.size(), 4);

        buf.resize(22);
        EXPECT_EQ(buf.size(), 4);
    }
    // Test: tampon fixe size == 0
    {
        DenseStorage<float, 0> buf;
        EXPECT_EQ(buf.size(), 0);
    }
    // Test: tampon fixe lecture/modification et constructeur de copie
    {
        DenseStorage<float, 4> buf;
        buf[0] = 1.0f;
        buf[1] = 2.0f;
        buf[2] = 3.2f;
        buf[3] = -1.0f;

        EXPECT_DOUBLE_EQ(buf[0], 1.0f);
        EXPECT_DOUBLE_EQ(buf[1], 2.0f);
        EXPECT_DOUBLE_EQ(buf[2], 3.2f);
        EXPECT_DOUBLE_EQ(buf[3], -1.0f);

        DenseStorage<float, 4> buf2(buf);
        EXPECT_DOUBLE_EQ(buf2[0], 1.0f);
        EXPECT_DOUBLE_EQ(buf2[1], 2.0f);
        EXPECT_DOUBLE_EQ(buf2[2], 3.2f);
        EXPECT_DOUBLE_EQ(buf2[3], -1.0f);

    }
    // Test: tampon dynamique size() et resize()
    {
        DenseStorage<float, Dynamic> buf(8);
        EXPECT_EQ(buf.size(), 8);

        buf.resize(102);
        EXPECT_EQ(buf.size(), 102);

    }
    // Test: tampon dynamique size == 0
    {
        DenseStorage<float, Dynamic> buf(0);
        EXPECT_EQ(buf.size(), 0);
    }
    // Test: tampon dynamique lecture/modification et constructeur de copie
    {
        DenseStorage<float, Dynamic> buf(4);
        buf[0] = 1.0f;
        buf[1] = 2.0f;
        buf[2] = 3.2f;
        buf[3] = -1.0f;

        EXPECT_DOUBLE_EQ(buf[0], 1.0f);
        EXPECT_DOUBLE_EQ(buf[1], 2.0f);
        EXPECT_DOUBLE_EQ(buf[2], 3.2f);
        EXPECT_DOUBLE_EQ(buf[3], -1.0f);

        DenseStorage<float, Dynamic> buf2(buf);
        EXPECT_DOUBLE_EQ(buf2[0], 1.0f);
        EXPECT_DOUBLE_EQ(buf2[1], 2.0f);
        EXPECT_DOUBLE_EQ(buf2[2], 3.2f);
        EXPECT_DOUBLE_EQ(buf2[3], -1.0f);

    }
}