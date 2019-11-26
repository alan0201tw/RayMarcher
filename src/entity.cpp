#include "entity.hpp"

#include <cmath>

float Sphere::Distance(Vector3 point) const
{
    return length(m_position - point) - m_radius;
}

float Cube::Distance(Vector3 point) const
{
    Vector3 o = abs(m_position - point) - m_extent;
    float ud = length(max(o, 0.0f));
    float n = std::max(std::max(
        std::min(o[0], 0.0f), std::min(o[1], 0.0f)), std::min(o[2], 0.0f));

    return ud + n;
}