#include "meshOperation.hpp"

#include <stdexcept>

float MeshBlender::SmoothDistance(float d0, float d1, const float k)
{
    // !!! IMPORTANT !!!
    // use std::abs so g++ do not link to integer version
    float h = std::max( k - std::abs(d0 - d1), 0.0f ) / k;
    return (std::min(d0, d1) - h*h*h*k*(1.0f/6.0f));
}

float MeshBlender::Distance(Vector3 point) const
{
    if(m_entities.empty())
    {
        throw std::runtime_error("Empty Blender!");
    }

    float currentDistance = m_entities[0]->Distance(point);
    for(size_t i = 0; i < m_entities.size(); i++)
    {
        currentDistance = SmoothDistance(currentDistance, m_entities[i]->Distance(point), m_k);
    }
    return currentDistance;
}