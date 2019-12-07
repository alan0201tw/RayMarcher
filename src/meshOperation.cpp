#include "meshOperation.hpp"

#include <stdexcept>

#include "bvh.hpp"

DistanceInfo MeshBlender::SmoothDistance(
    DistanceInfo info0, 
    DistanceInfo info1, 
    const float k)
{
    const float d0 = info0.distance;
    const float d1 = info1.distance;
    const Vector3 col0 = info0.color;
    const Vector3 col1 = info1.color;

    // !!! IMPORTANT !!!
    // use std::abs so g++ do not link to integer version
    const float h = std::max( k - std::abs(d0 - d1), 0.0f ) / k;

    DistanceInfo info;
    info.distance = (std::min(d0, d1) - h*h*h*k*(1.0f/6.0f));
    
    const float dd0 = std::abs(info.distance - d0);
    const float dd1 = std::abs(info.distance - d1);
    const float r0 = dd0 / (dd0 + dd1);
    const float r1 = dd1 / (dd0 + dd1);

    info.color = 
        col0 * std::abs(r1) + 
        col1 * std::abs(r0);

    return info;
}

AABB MeshBlender::GetBoundingBox() const
{
    AABB box = m_entities[0]->GetBoundingBox();
    for(size_t i = 1; i < m_entities.size(); ++i)
    {
        box = AABB::MergeAABB(
            box,
            m_entities[i]->GetBoundingBox()
            );
    }
    return box;
}

DistanceInfo MeshBlender::GetDistanceInfo(Vector3 point, float time) const
{
    if(m_entities.empty())
    {
        throw std::runtime_error("Empty Blender!");
    }

    DistanceInfo currentInfo = m_entities[0]->GetDistanceInfo(point, time);
    for(size_t i = 1; i < m_entities.size(); ++i)
    {
        currentInfo = SmoothDistance(
            currentInfo,
            m_entities[i]->GetDistanceInfo(point, time),
            m_k);
    }
    return currentInfo;
}