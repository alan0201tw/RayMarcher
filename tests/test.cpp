#include <gtest/gtest.h>

#include <cmath>

#include "transform.hpp"

#define HELLOWORLD "Hello World"
 
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