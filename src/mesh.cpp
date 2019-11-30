#include "mesh.hpp"

DistanceInfo Triangle::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = linalg::mul(linalg::inverse(m_transform.orientation),
		(m_transform.position - point)) * (1.0f / m_transform.scale);

	const Vector3 a = m_vertices[0];
	const Vector3 b = m_vertices[1];
	const Vector3 c = m_vertices[2];
	const Vector3 p = pos;

	Vector3 ba = b - a; Vector3 pa = p - a;
	Vector3 cb = c - b; Vector3 pb = p - b;
	Vector3 ac = a - c; Vector3 pc = p - c;
	Vector3 nor = cross(ba, ac);

	DistanceInfo info;

	info.distance = std::sqrt(
		(sign(dot(cross(ba, nor), pa)) +
			sign(dot(cross(cb, nor), pb)) +
			sign(dot(cross(ac, nor), pc)) < 2.0f)
		?
		std::min(std::min(
			linalg::length2(ba*std::clamp(dot(ba, pa) / linalg::length2(ba), 0.0f, 1.0f) - pa),
			linalg::length2(cb*std::clamp(dot(cb, pb) / linalg::length2(cb), 0.0f, 1.0f) - pb)),
			linalg::length2(ac*std::clamp(dot(ac, pc) / linalg::length2(ac), 0.0f, 1.0f) - pc))
		:
		dot(nor, pa)*dot(nor, pa) / linalg::length2(nor));

	// TMP
	info.color = Vector3(1, 1, 1);

	return info;
}

DistanceInfo TriangleMesh::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = linalg::mul(linalg::inverse(m_transform.orientation),
		(m_transform.position - point)) * (1.0f / m_transform.scale);

	DistanceInfo info;
	info.distance = 1e9;

	for (auto& tri : m_triangles)
	{
		auto currentInfo = tri.GetDistanceInfo(pos, time);
		if (currentInfo.distance < info.distance)
		{
			info = currentInfo;
		}
	}

	return info;
}