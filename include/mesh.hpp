#pragma once

#include "entity.hpp"

#include <array>
#include <vector>

class Triangle final : public Entity
{
private:
	std::array<Vector3, 3> m_vertices;
	Vector3 m_normal;

	Vector3 m_color;

	inline float sign(float value) const
	{
		// simulate glsl sign function
		// sign returns -1.0 if x is less than 0.0, 0.0 if x is equal to 0.0, and +1.0 if x is greater than 0.0.
		if (value == 0.0f) return 0.0f;
		return (value < 0.0f) ? -1.0f : 1.0f;
	}

	const Vector3 ba;
	const Vector3 cb;
	const Vector3 ac;
	const Vector3 cross_ba_normal;
	const Vector3 cross_cb_normal;
	const Vector3 cross_ac_normal;

public:
	explicit Triangle(
		const Transform& transform,
		const std::array<Vector3, 3>& vertices,
		const Vector3& color
		)
		: Entity(transform), m_vertices(vertices)
		, m_normal(safe_normalize(linalg::cross(vertices[1] - vertices[0], vertices[2] - vertices[0])))
		, m_color(color)
		, ba(vertices[1] - vertices[0])
		, cb(vertices[2] - vertices[1])
		, ac(vertices[0] - vertices[2])
		, cross_ba_normal(linalg::cross(ba, -m_normal))
		, cross_cb_normal(linalg::cross(cb, -m_normal))
		, cross_ac_normal(linalg::cross(ac, -m_normal))
		{
		}

	~Triangle() {};

	virtual AABB GetBoundingBox() const final override;
	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

	Triangle(const Triangle& other) = default;
	Triangle(Triangle&& other) = default;
	Triangle& operator=(Triangle const &) = default;
	Triangle& operator=(Triangle &&) = default;
};

 class TriangleMesh final : public Entity
 {
 private:
 	// TODO : change to BVH 
 	std::vector<Triangle> m_triangles;
	
 public:
 	explicit TriangleMesh(
 		const Transform& transform,
 		const std::vector<Triangle>& triangles
 		)
 		: Entity(transform), m_triangles(triangles)
 		{}

 	explicit TriangleMesh(
 		const Transform& transform,
 		std::vector<Triangle>&& triangles
 	)
 		: Entity(transform), m_triangles(std::move(triangles))
 	{}

 	~TriangleMesh() {};

	virtual AABB GetBoundingBox() const final override;
 	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

 	TriangleMesh(const TriangleMesh& other) = default;
 	TriangleMesh(TriangleMesh&& other) = default;
 	TriangleMesh& operator=(TriangleMesh const &) = default;
 	TriangleMesh& operator=(TriangleMesh &&) = default;
 };