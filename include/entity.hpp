#pragma once

#include "transform.hpp"
#include "common.hpp"

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
    float     const & scale      () const { return m_transform.scale; }
	float           & scale      ()       { return m_transform.scale; }

protected:
    explicit Entity(
        Transform transform
        )
        : m_transform(transform)
          {};
    virtual ~Entity() {};

	// Restrict copy constructor and assign operator

	Entity(const Entity& other) = default;
	Entity(Entity&& other) = default;
	Entity& operator=(Entity const &) = default;
	Entity& operator=(Entity &&) = default;
};

class Sphere final : public Entity
{
private:
    float m_radius;
    Vector3 m_color;

public:
    explicit Sphere(
		Transform transform, 
		float radius, 
        Vector3 color
		)
        : Entity(transform), m_radius(radius), m_color(color)
        {}

    ~Sphere() {};

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

	Sphere(const Sphere& other) = default;
	Sphere(Sphere&& other) = default;
	Sphere& operator=(Sphere const &) = default;
	Sphere& operator=(Sphere &&) = default;
};

class Cube final : public Entity
{
private:
    Vector3 m_extent;
    Vector3 m_color;

public:
    explicit Cube(
		Transform transform,
		Vector3 extent,
        Vector3 color
		)
        : Entity(transform), m_extent(extent), m_color(color)
        {}

    ~Cube() {};

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

	Cube(const Cube& other) = default;
	Cube(Cube&& other) = default;
	Cube& operator=(Cube const &) = default;
	Cube& operator=(Cube &&) = default;
};