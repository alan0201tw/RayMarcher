#pragma once

#include "vec_math.hpp"

struct Transform
{
    Vector3   position;
    Matrix3x3 orientation;
    Vector3   scale;
};