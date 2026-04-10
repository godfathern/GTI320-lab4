/**
 * @file TestsMath3D.cpp
 *
 * @brief Tests unitaires pour les rotations et les transformations 3D.
 *
 */

#include "Math3D.h"

#include <gtest/gtest.h>

using namespace gti320;

// Test les matrices à taille fixe
TEST(TestsMath3D, TransformPoints)
{
    // Test: Identité
    {
        Matrix4f M;
        M.setIdentity();

        Vector3f p;
        p(0) = 1.0f;
        p(1) = 27.222f;
        p(2) = -3.5f;

        Vector3f p2 = M * p;
        EXPECT_DOUBLE_EQ(p(0), p2(0));
        EXPECT_DOUBLE_EQ(p(1), p2(1));
        EXPECT_DOUBLE_EQ(p(2), p2(2));
    }

    // Test : rotation 
    {
        Matrix3f R = makeRotation(0.2f, -1.3f, 2.1f);

        Vector3f p;
        p(0) = 1.0f;
        p(1) = 6.3f;
        p(2) = -4.1f;

        Vector3f p2 = R * p;
        EXPECT_NEAR(-7.51382065f, p2(0), 1e-6f);
        EXPECT_NEAR(-0.996269703f, p2(1), 1e-6f);
        EXPECT_NEAR(0.223480821f, p2(2), 1e-6f);
    }

    // Test : translation
    {
        Matrix4f M;
        M.setIdentity();
        M(0, 3) = 1.0f;
        M(1, 3) = -7.6f;
        M(2, 3) = -0.4f;

        Vector3f p;
        p.setZero();

        Vector3f p2 = M * p;
        EXPECT_DOUBLE_EQ(1.0f, p2(0));
        EXPECT_DOUBLE_EQ(-7.6f, p2(1));
        EXPECT_DOUBLE_EQ(-0.4f, p2(2));
    }
    // Test : rotation+translation
    {
        Matrix4f M;
        M.setIdentity();

        const Matrix3f R = makeRotation(-0.32f, 0.5f, 1.1f);
        M(0, 0) = R(0, 0); M(0, 1) = R(0, 1); M(0, 2) = R(0, 2);
        M(1, 0) = R(1, 0); M(1, 1) = R(1, 1); M(1, 2) = R(1, 2);
        M(2, 0) = R(2, 0); M(2, 1) = R(2, 1); M(2, 2) = R(2, 2);
        M(0, 3) = 0.3f;
        M(1, 3) = -22.6f;
        M(2, 3) = 5.4f;

        Vector3f p;
        p(0) = 1.0f;
        p(1) = -1.0f;
        p(2) = 2.0f;

        Vector3f p2 = M * p;
        EXPECT_NEAR(1.46460509f, p2(0), 1e-6f);
        EXPECT_NEAR(-21.0175304f, p2(1), 1e-6f);
        EXPECT_NEAR(6.86269760f, p2(2), 1e-6f);
    }


}

TEST(TestsMath3D, TransformInverse)
{
    Matrix4f M;
    M.setIdentity();
    const Matrix3f R = makeRotation(-0.32f, 0.5f, 1.1f);
    M(0, 0) = R(0, 0); M(0, 1) = R(0, 1); M(0, 2) = R(0, 2);
    M(1, 0) = R(1, 0); M(1, 1) = R(1, 1); M(1, 2) = R(1, 2);
    M(2, 0) = R(2, 0); M(2, 1) = R(2, 1); M(2, 2) = R(2, 2);
    M(0, 3) = 0.3f;
    M(1, 3) = -22.6f;
    M(2, 3) = 5.4f;

    Matrix4f Minv = M.inverse();

    EXPECT_NEAR(0.398068011f, Minv(0, 0), 1e-6f);
    EXPECT_NEAR(-0.914373040f, Minv(1, 0), 1e-6f);
    EXPECT_NEAR(-0.0739180297f, Minv(2, 0), 1e-6f);
    EXPECT_NEAR(0.0f, Minv(3, 0), 1e-6f);
    EXPECT_NEAR(0.782108068f, Minv(0, 1), 1e-6f);
    EXPECT_NEAR(0.296165377f, Minv(1, 1), 1e-6f);
    EXPECT_NEAR(0.548263669f, Minv(2, 1), 1e-6f);
    EXPECT_NEAR(0.0f, Minv(3, 1), 1e-6f);
    EXPECT_NEAR(-0.479425550f, Minv(0, 2), 1e-6f);
    EXPECT_NEAR(-0.276058108f, Minv(1, 2), 1e-6f);
    EXPECT_NEAR(0.833032429f, Minv(2, 2), 1e-6f);
    EXPECT_NEAR(0.0f, Minv(3, 2), 1e-6f);
    EXPECT_NEAR(20.1451187f, Minv(0, 3), 1e-6f);
    EXPECT_NEAR(8.45836353f, Minv(1, 3), 1e-6f);
    EXPECT_NEAR(7.91455984f, Minv(2, 3), 1e-6f);
    EXPECT_NEAR(1.0f, Minv(3, 3), 1e-6f);
}

TEST(TestsMath3D, RotationInverse)
{
    {
        Matrix3f R = makeRotation(0.0f, 0.0f, 0.0f);
        Matrix3f R2 = R.inverse();
        EXPECT_NEAR(1.0f, R(0, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R(1, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R(2, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R(0, 1), 1e-6f);
        EXPECT_NEAR(1.0f, R(1, 1), 1e-6f);
        EXPECT_NEAR(0.0f, R(2, 1), 1e-6f);
        EXPECT_NEAR(0.0f, R(0, 2), 1e-6f);
        EXPECT_NEAR(0.0f, R(1, 2), 1e-6f);
        EXPECT_NEAR(1.0f, R(2, 2), 1e-6f);
        EXPECT_NEAR(1.0f, R2(0, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R2(1, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R2(2, 0), 1e-6f);
        EXPECT_NEAR(0.0f, R2(0, 1), 1e-6f);
        EXPECT_NEAR(1.0f, R2(1, 1), 1e-6f);
        EXPECT_NEAR(0.0f, R2(2, 1), 1e-6f);
        EXPECT_NEAR(0.0f, R2(0, 2), 1e-6f);
        EXPECT_NEAR(0.0f, R2(1, 2), 1e-6f);
        EXPECT_NEAR(1.0f, R2(2, 2), 1e-6f);
    }
    {
        Matrix3f R = makeRotation(0.2f, 0.3f, 0.5f);
        Matrix3f R2 = R.inverse();
        EXPECT_NEAR(0.838386655f, R2(0, 0), 1e-6f);
        EXPECT_NEAR(-0.418345392f, R2(1, 0), 1e-6f);
        EXPECT_NEAR(0.349420935f, R2(2, 0), 1e-6f);
        EXPECT_NEAR(0.458012730f, R2(0, 1), 1e-6f);
        EXPECT_NEAR(0.888236821f, R2(1, 1), 1e-6f);
        EXPECT_NEAR(-0.0354929715f, R2(2, 1), 1e-6f);
        EXPECT_NEAR(-0.295520216f, R2(0, 2), 1e-6f);
        EXPECT_NEAR(0.189796060f, R2(1, 2), 1e-6f);
        EXPECT_NEAR(0.936293423f, R2(2, 2), 1e-6f);
    }
}
