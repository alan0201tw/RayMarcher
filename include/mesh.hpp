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

public:
	explicit Triangle(
		const Transform& transform,
		const std::array<Vector3, 3>& vertices,
		const Vector3& color
		)
		: Entity(transform), m_vertices(vertices), m_color(color)
		{
			m_normal = safe_normalize(linalg::cross(m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]));
		}

	~Triangle() {};

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

 	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

 	TriangleMesh(const TriangleMesh& other) = default;
 	TriangleMesh(TriangleMesh&& other) = default;
 	TriangleMesh& operator=(TriangleMesh const &) = default;
 	TriangleMesh& operator=(TriangleMesh &&) = default;
 };