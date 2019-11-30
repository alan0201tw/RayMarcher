#pragma once

#include "entity.hpp"

#include <array>
#include <vector>

class Triangle final : public Entity
{
private:
	std::array<Vector3, 3> m_vertices;
	Vector3 m_normal;

	inline float sign(float value) const
	{
		// simulate glsl sign function
		// sign returns -1.0 if x is less than 0.0, 0.0 if x is equal to 0.0, and +1.0 if x is greater than 0.0.
		return (value == 0.0f) ? 0.0f : ((value < 0.0f) ? -1.0f : 1.0f);
	}

public:
	explicit Triangle(
		Transform transform,
		std::array<Vector3, 3> vertices
		)
		: Entity(transform), m_vertices(vertices)
		{
			
			m_normal = linalg::normalize(linalg::cross(m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]));
		}

	~Triangle() {};

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class TriangleMesh final : public Entity
{
private:
	std::vector<Triangle> m_triangles;

public:
	explicit TriangleMesh(
		Transform transform,
		std::vector<Triangle> triangles
		)
		: Entity(transform), m_triangles(triangles)
		{}

	~TriangleMesh() {};

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};