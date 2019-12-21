#include <gtest/gtest.h>

#include <cmath>

#include "transform.hpp"

#define HELLOWORLD "Hello World"

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

TEST(GoogleTest, HelloWorld) { 
    ASSERT_EQ("Hello World", HELLOWORLD);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}