#pragma once

#include "vector3.hpp"

class Entity
{
public:
    Vector3 m_position;

    explicit Entity(Vector3 position)
        : m_position(position)
        {};

public:
    virtual float Distance(Vector3 point) const = 0;
};

class Sphere final : public Entity
{
public:
    float m_radius;
public:
    explicit Sphere(Vector3 position, float radius)
        : Entity(position), m_radius(radius)
        {}

    virtual float Distance(Vector3 point) const final override;
};

class Cube final : public Entity
{
public:
    Vector3 m_extent;
public:
    explicit Cube(Vector3 position, Vector3 extent)
        : Entity(position), m_extent(extent)
        {}

    virtual float Distance(Vector3 point) const final override;
};