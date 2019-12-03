#pragma once

#include "vec_math.hpp"

class Ray
{
public:

    Ray(const Vector3& origin, const Vector3& direction)
    {
        m_origin = origin;
        m_direction = direction;
    }

    ~Ray() {}

	Ray(const Ray& other) = default;
	Ray(Ray&& other) = default;
	Ray& operator=(Ray const &) = default;
	Ray& operator=(Ray &&) = default;

    inline Vector3 GetOrigin() const       { return m_origin; }
    inline Vector3 GetDirection() const    { return m_direction; }

private:
    Vector3 m_origin;
    Vector3 m_direction;
};
