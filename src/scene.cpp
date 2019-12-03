#include "scene.hpp"

#include <iostream>

#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace
{
	std::shared_ptr<Entity> sphere;
	std::shared_ptr<Entity> spherePlane;
	std::shared_ptr<Entity> bunnyMesh;
}

DistanceInfo IDistanceList::GetDistanceInfo(Vector3 point, float time) const
{
	DistanceInfo info;
	info.distance = 1e9f;
	for(auto& id : m_idistList)
	{
		auto currentInfo = id->GetDistanceInfo(point, time);
		if(currentInfo.distance < info.distance)
		{
			info = currentInfo;
		}
	}

	return info;
}

DistanceInfo Scene::GetDistanceInfo(Vector3 point, float time) const
{
    // TODO : modify to use BVH for speed up
    return m_distanceFuncProvider->GetDistanceInfo(point, time);
}

void Scene::Update(float currentTime) const
{
	sphere->position() = Vector3(8.0f * (0.5f - currentTime));

	bunnyMesh->orientation() = Matrix3x3(
		{ std::cos(currentTime * 5.0f), 0, std::sin(currentTime * 5.0f) },
		{ 0, 1, 0 },
		{-std::sin(currentTime * 5.0f), 0, std::cos(currentTime * 5.0f) }
	);
}

Scene::Scene()
{
    const Transform sphereTransform =
	{
		Vector3(2.5f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		1.0f
	};

	const Transform bunnyTransform =
	{
		Vector3(0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		5.0f
	};

	const Transform spherePlaneTransform = 
	{
		Vector3(0.0f, -255.0f, 0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		250.0f
	};

	const Transform identityTransform =
	{
		Vector3(0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		1.0f
	};

	sphere = std::make_shared<Sphere>(
		sphereTransform, 
		1.0f,
		Vector3(173, 245, 255) / 255.0f
		);

	spherePlane = std::make_shared<Sphere>(
		spherePlaneTransform,
		1.0f,
		Vector3(240, 136, 83) / 255.0f
		);

	// Load Bunny Mesh
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "./resources/bunnyLow.obj");
	if (!warn.empty())
	{
		std::cout << warn << std::endl;
	}
	if (!err.empty())
	{
		std::cerr << err << std::endl;
	}

	std::vector<Triangle> bunny_list;
	bunny_list.reserve(shapes.size());
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[s].mesh.num_face_vertices[f];

			std::array<Vector3, 3> vertices;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				vertices[v] = Vector3(vx, vy, vz);
			}

			// Use emplace_back to reduce the amount of instantiation
			// bunny_list.push_back(Triangle(identityTransform, vertices, Vector3(230, 67, 83) / 255.0f ));
			bunny_list.emplace_back(identityTransform, vertices, Vector3(230, 67, 83) / 255.0f );

			index_offset += fv;
		}
	}

	bunnyMesh = std::make_shared<TriangleMesh>(bunnyTransform, bunny_list);

	std::vector<IDistanceRef> blendingEntityList
		= { sphere, bunnyMesh };
	IDistanceRef blendingEntity
		= std::make_shared<MeshBlender>(blendingEntityList, 3.0f);

    // m_distanceFuncProvider = blendingEntity;
	//m_distanceFuncProvider = bunnyMesh;
	std::vector<IDistanceRef> sceneList
		= { blendingEntity, spherePlane };
	m_distanceFuncProvider = std::make_shared<IDistanceList>(sceneList);
}

Vector3 Scene::EvaluateNormal(Vector3 point, float time, float epsilon) const
{
	// TEST : test to check if this improves the lighting on triangle mesh - bunnyLow
	// TEST Result : does not work
	// epsilon *= 0.01f;

	// TEST : this is even worse
	//epsilon = std::numeric_limits<float>::epsilon();

	const Vector3 eps_x = Vector3(epsilon, 0.0f, 0.0f);
	const Vector3 eps_y = Vector3(0.0f, epsilon, 0.0f);
	const Vector3 eps_z = Vector3(0.0f, 0.0f, epsilon);

	Vector3 normal = Vector3(
		GetDistanceInfo(point + eps_x, time).distance - GetDistanceInfo(point - eps_x, time).distance,
		GetDistanceInfo(point + eps_y, time).distance - GetDistanceInfo(point - eps_y, time).distance,
		GetDistanceInfo(point + eps_z, time).distance - GetDistanceInfo(point - eps_z, time).distance);

	return safe_normalize(normal);
}