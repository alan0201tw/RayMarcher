#include "entity.hpp"

#include "bvh.hpp"

AABB Sphere::GetBoundingBox() const
{
    throw std::runtime_error(
        "Sphere::GetBoundingBox currently does not support transform");

    Vector3 radiusVector(m_radius * m_transform.scale);

    return AABB( m_transform.position - radiusVector, 
        m_transform.position + radiusVector );
}

DistanceInfo Sphere::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = ApplyInverseTransform(m_transform, point);

    DistanceInfo info;
    info.color = m_color;
    info.distance = linalg::length(pos) - m_radius;
	info.distance *= m_transform.scale;

    return info;
}

AABB Cube::GetBoundingBox() const
{
    throw std::runtime_error(
        "Cube::GetBoundingBox currently does not support transform");

    // by doubling the extent vector, we should be able to neglect rotation
    Vector3 halfExtent(m_extent * 0.5f * m_transform.scale);
    halfExtent *= 2.0f;

    return AABB( m_transform.position - halfExtent, 
        m_transform.position + halfExtent );
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

//DistanceInfo Prism::GetDistanceInfo(Vector3 point, float time) const
//{
//    Vector3 pos = linalg::mul( linalg::inverse(m_transform.orientation), 
//        (m_transform.position - point)) * (1.0f / m_transform.scale);
//
//    Vector3 q = abs(pos);
//    
//    DistanceInfo info;
//    info.color = Vector3(1.0f, 1.0f, 1.0f);
//    info.distance = std::max(q.z - m_h.y,
//        std::max(q.x * 0.866025f + pos.y * 0.5f, -pos.y) - m_h.x * 0.5f);
//
//    return info;
//}