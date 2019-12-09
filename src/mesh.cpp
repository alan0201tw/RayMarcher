#include "mesh.hpp"

#include "bvh.hpp"

size_t Triangle::s_getBBCount = 0;
size_t Triangle::s_getDistCount = 0;

AABB Triangle::GetBoundingBox() const
{
	s_getBBCount++;

	const Vector3 v0 = m_vertices[0];
	const Vector3 v1 = m_vertices[1];
	const Vector3 v2 = m_vertices[2];

	const Vector3 midpoint = (v0 + v1 + v2) / 3.0f;
	return AABB(midpoint, midpoint);

	// float minX = std::min(v0.x, std::min(v1.x, v2.x));
    // float minY = std::min(v0.y, std::min(v1.y, v2.y));
    // float minZ = std::min(v0.z, std::min(v1.z, v2.z));

    // float maxX = std::max(v0.x, std::max(v1.x, v2.x));
    // float maxY = std::max(v0.y, std::max(v1.y, v2.y));
    // float maxZ = std::max(v0.z, std::max(v1.z, v2.z));

    // Vector3 minPoint = Vector3(minX, minY, minZ);
    // Vector3 maxPoint = Vector3(maxX, maxY, maxZ);

    // return AABB(minPoint, maxPoint);
}

DistanceInfo Triangle::GetDistanceInfo(Vector3 point, float time) const
{
	s_getDistCount++;

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

	info.distance *= m_transform.scale;

	info.color = m_color;

	return info;
}

 AABB TriangleMesh::GetBoundingBox() const
 {
	 AABB box = m_triangles[0].GetBoundingBox();
	 for (size_t i = 1; i < m_triangles.size(); ++i)
	 {
		 box = AABB::MergeAABB(
			 box,
			 m_triangles[i].GetBoundingBox()
		 );
	 }
	 return box;
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