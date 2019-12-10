#pragma once

#include <vector>

#include "common.hpp"
#include "entity.hpp"

struct AABB
{
private:
    Vector3 m_min, m_max;

public:
    explicit AABB() {}
    explicit AABB(Vector3 minPosition, Vector3 maxPosition)
        : m_min(minPosition), m_max(maxPosition)
        {}

    inline Vector3 GetMin() const { return m_min; }
    inline Vector3 GetMax() const { return m_max; }

    // do not inherit from IDistance
    // simply use a distance function
    float GetDistance(Vector3 point) const;

    static AABB MergeAABB(const AABB& a, const AABB& b);
};

class BVH : public IDistance, public IUpdate
{
protected:
    std::vector<IDistanceRef> m_elements;
    IDistanceRef m_leftBVH, m_rightBVH;
    AABB m_boundingBox;

public:
    BVH(const std::vector<IDistanceRef>& elements);
    BVH(std::vector<IDistanceRef>&& elements);

    virtual ~BVH() {};

    virtual void Update(float currentTime) final override;
    virtual AABB GetBoundingBox() const final override;
    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

// Functors for comparing / sorting entities
private:

    void UpdateBVH();

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

class TransformedBVH final : public Entity, public IUpdate
{
private:
    BVH m_bvh;

    static AABB GetTransformedAABB(Transform transform, AABB aabb);

public:

    TransformedBVH(
        const Transform& transform,
        const std::vector<IDistanceRef>& elements
        )
        : Entity(transform), m_bvh(elements)
        {}

    TransformedBVH(
        const Transform& transform,
        std::vector<IDistanceRef>&& elements
        )
        : Entity(transform), m_bvh(std::move(elements))
        {}

    ~TransformedBVH() {}

    virtual void Update(float currentTime) final override;
    virtual AABB GetBoundingBox() const final override;
    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};