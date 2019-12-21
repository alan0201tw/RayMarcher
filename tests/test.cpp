#include <gtest/gtest.h>

#include <iostream>
#include <cmath>
#include <random>

#include "transform.hpp"
#include "mesh.hpp"
#include "bvh.hpp"

TEST(TransformTest, inverseTransform)
{
    const float M_PI_MUL2 = 2.0f * static_cast<float>(M_PI);

    Transform testTransform = 
    {
        Vector3(2.0f, 1.5f, -7.1f),
        Matrix3x3(
			{ std::cos(0.13f * M_PI_MUL2), 0, std::sin(0.13f * M_PI_MUL2) },
			{ 0, 1, 0 },
			{ -std::sin(0.13f * M_PI_MUL2), 0, std::cos(0.13f * M_PI_MUL2) }
		),
        1.5f
    };

    Vector3 pos(-2.11f, 3.8851f, 13.13f);

    Vector3 inverseAndBackPos = ApplyInverseTransform(testTransform, ApplyTransform(testTransform, pos));

    // std::cout << std::abs(pos.x - inverseAndBackPos.x) << std::endl;
    // std::cout << std::abs(pos.y - inverseAndBackPos.y) << std::endl;
    // std::cout << std::abs(pos.z - inverseAndBackPos.z) << std::endl;

    // take floating point error into account
    EXPECT_NEAR(pos.x, inverseAndBackPos.x, 0.00001f);
    EXPECT_NEAR(pos.y, inverseAndBackPos.y, 0.00001f);
    EXPECT_NEAR(pos.z, inverseAndBackPos.z, 0.00001f);
}

TEST(VectorMathTest, safe_normalize)
{
    Vector3 zero_vec(0.0f);

    ASSERT_EQ(linalg::length(zero_vec), 0.0f);
    ASSERT_EQ(zero_vec, safe_normalize(zero_vec));
    ASSERT_TRUE(std::isnan( linalg::normalize(zero_vec).x ));
}

TEST(BoundingBoxTest, triangleBoundingBox)
{
    {
        Triangle triangle
        (
            identityTransform,
            std::array<Vector3, 3>
            { 
                Vector3(0.0f, 1.0f, 0.0f),
                Vector3(1.0f, 1.0f, 0.0f),
                Vector3(0.0f, 0.0f, 1.0f)
            },
            Vector3(0.0f)
        );

        AABB aabb = triangle.GetBoundingBox();

        ASSERT_EQ(aabb.GetMin(), Vector3(0.0f));
        ASSERT_EQ(aabb.GetMax(), Vector3(1.0f));
    }

    // use scope to avoid name conflicts
    {
        Triangle triangle
        (
            identityTransform,
            std::array<Vector3, 3>
            { 
                Vector3(-0.4f, 1.0f, 2.0f),
                Vector3(1.0f, 1.0f, -1.0f),
                Vector3(-3.0f, -0.5f, 1.7f)
            },
            Vector3(0.0f)
        );

        AABB aabb = triangle.GetBoundingBox();

        ASSERT_EQ(aabb.GetMin(), Vector3(-3.0f, -0.5f, -1.0f));
        ASSERT_EQ(aabb.GetMax(), Vector3(1.0f, 1.0f, 2.0f));
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}