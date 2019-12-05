#pragma once

#include <vector>

#include "common.hpp"

struct AABB
{
private:
    Vector3 m_min, m_max;

public:
    AABB(Vector3 minPosition, Vector3 maxPosition)
        : m_min(minPosition), m_max(maxPosition)
        {}

    Vector3 GetMin() const;
    Vector3 GetMax() const;

    // do not inherit from IDistance
    // simply use a distance function
    float GetDistance(Vector3 point) const;

    static AABB MergeAABB(const AABB& a, const AABB& b);
};

class BVH final : public IDistance
{
private:
    IDistanceRef m_leftBVH, m_rightBVH;
    AABB m_boundingBox;

public:
    BVH(const std::vector<IDistanceRef>& elements);
    BVH(std::vector<IDistanceRef>&& elements);

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

// Functors for comparing / sorting entities
private:
    class box_x_compare
    {
    public:
        bool operator()(const IDistanceRef& a, const IDistanceRef& b) const;
    };
    class box_y_compare
    {
    public:
        bool operator()(const IDistanceRef& a, const IDistanceRef& b) const;
    };
    class box_z_compare
    {
    public:
        bool operator()(const IDistanceRef& a, const IDistanceRef& b) const;
    };
};