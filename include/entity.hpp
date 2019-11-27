#pragma once

#include "transform.hpp"

struct DistanceInfo
{
    float distance;
    Vector3 color;
};

class IDistance
{
public:
    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const = 0;
};

class Entity : public IDistance
{
public:
    Transform m_transform;

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const = 0;

protected:
    explicit Entity(
        Transform transform
        )
        : m_transform(transform)
          {};
};

class Sphere final : public Entity
{
private:
    float m_radius;
public:
    explicit Sphere(Transform transform, float radius)
        : Entity(transform), m_radius(radius)
        {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class Cube final : public Entity
{
private:
    Vector3 m_extent;
public:
    explicit Cube(Transform transform, Vector3 extent)
        : Entity(transform), m_extent(extent)
        {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class Prism final : public Entity
{
private:
    Vector2 m_h;

public:
    explicit Prism(Transform transform, Vector2 _h)
        : Entity(transform), m_h(_h)
        {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};