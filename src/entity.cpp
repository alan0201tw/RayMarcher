#include "entity.hpp"

#include <cmath>

float Sphere::Distance(Vector3 point) const
{
    return length(m_position - point) - m_radius;
}

float Cube::Distance(Vector3 point) const
{
    return length(max(abs(m_position - point) - m_extent, 0.0f));
}