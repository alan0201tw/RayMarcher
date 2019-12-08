#include "entity.hpp"

DistanceInfo Sphere::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = ApplyInverseTransform(m_transform, point);

    DistanceInfo info;
    info.color = m_color;
    info.distance = linalg::length(pos) - m_radius;
	info.distance *= m_transform.scale;

    return info;
}

DistanceInfo Cube::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = ApplyInverseTransform(m_transform, point);

    Vector3 o = abs(pos) - m_extent;
    float ud = length(max(o, 0.0f));
    float n = std::max(std::max(
        std::min(o[0], 0.0f), std::min(o[1], 0.0f)), std::min(o[2], 0.0f));

    DistanceInfo info;
    info.color = m_color;
    info.distance = ud + n;
	info.distance *= m_transform.scale;

    return info;
}
