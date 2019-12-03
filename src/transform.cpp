#include "transform.hpp"

Vector3 ApplyInverseTransform(Transform transform, Vector3 point)
{
	return linalg::mul(linalg::transpose(transform.orientation),
		(point - transform.position)) / transform.scale;
}

Vector3 safe_normalize(Vector3 a)
{
	float length_a = linalg::length(a);
	return (length_a == 0.0f) ? Vector3(0.0f) : a / length_a;
}

float safe_distance(Vector3 a, Vector3 b)
{
	float length_ab = linalg::length(a - b);
	return (length_ab == 0.0f) ? 0.0f : length_ab;
}