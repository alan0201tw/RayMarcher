#include "mesh.hpp"

DistanceInfo Triangle::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = ApplyInverseTransform(m_transform, point);

	const Vector3 a = m_vertices[0];
	const Vector3 b = m_vertices[1];
	const Vector3 c = m_vertices[2];
	const Vector3 p = pos;

	const Vector3 ba = b - a; const Vector3 pa = p - a;
	const Vector3 cb = c - b; const Vector3 pb = p - b;
	const Vector3 ac = a - c; const Vector3 pc = p - c;
	const Vector3 nor = -m_normal;

	DistanceInfo info;
	// TEST : add std::abs to avoid culling, attempt to improve lighting quality
	info.distance = std::sqrt(
		(std::abs(sign(dot(cross(ba, nor), pa)) + 
		 sign(dot(cross(cb, nor), pb)) +
		 sign(dot(cross(ac, nor), pc))) < 2.0f)
		?
		std::min(std::min(
			linalg::length2(ba*std::clamp(dot(ba, pa) / linalg::length2(ba), 0.0f, 1.0f) - pa),
			linalg::length2(cb*std::clamp(dot(cb, pb) / linalg::length2(cb), 0.0f, 1.0f) - pb)),
			linalg::length2(ac*std::clamp(dot(ac, pc) / linalg::length2(ac), 0.0f, 1.0f) - pc))
		:
		dot(nor, pa)*dot(nor, pa) / linalg::length2(nor));

	info.distance *= m_transform.scale;

	// TMP
	info.color = m_color;

	return info;
}

DistanceInfo TriangleMesh::GetDistanceInfo(Vector3 point, float time) const
{
	const Vector3 pos = ApplyInverseTransform(m_transform, point);

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

	info.distance *= m_transform.scale;

	return info;
}