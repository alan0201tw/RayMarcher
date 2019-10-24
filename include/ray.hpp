#pragma once

#include "vector3.hpp"

class Ray
{
public:

    Ray(const Vector3& origin, const Vector3& direction)
    {
        m_origin = origin;
        m_direction = direction;
    }

    inline Vector3 GetOrigin() const       { return m_origin; }
    inline Vector3 GetDirection() const    { return m_direction; }
    inline Vector3 GetPointAtParameter(float t) const { return m_origin + t * m_direction; }

private:
    Vector3 m_origin;
    Vector3 m_direction;
};
