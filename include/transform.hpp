#pragma once

#include "vec_math.hpp"

struct Transform
{
    Vector3   position;
    Matrix3x3 orientation;
    //Vector3   scale;

	// currently only support uniform scale
	// non-uniform scale is too complicated to implement
	float scale;
};

Vector3 ApplyInverseTransform(Transform transform, Vector3 point);

Vector3 safe_normalize(Vector3 a);
float safe_distance(Vector3 a, Vector3 b);