#pragma once

#include "transform.hpp"
#include "common.h"

#include <functional>

class Entity : public IDistance
{
protected:
	Transform m_transform;

public:
	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const override = 0;

    // getter and setter for transform properties
    Vector3   const & position   () const { return m_transform.position; }
    Vector3         & position   ()       { return m_transform.position; }
    Matrix3x3 const & orientation() const { return m_transform.orientation; }
    Matrix3x3       & orientation()       { return m_transform.orientation; }
    Vector3   const & scale      () const { return m_transform.scale; }
    Vector3         & scale      ()       { return m_transform.scale; }

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
    explicit Sphere(
		Transform transform, 
		float radius
		)
        : Entity(transform), m_radius(radius)
        {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class Cube final : public Entity
{
private:
    Vector3 m_extent;
public:
    explicit Cube(
		Transform transform, 
		Vector3 extent
		)
        : Entity(transform), m_extent(extent)
        {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};