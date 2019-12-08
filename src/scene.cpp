#include "scene.hpp"

#include <iostream>

#include "mesh.hpp"
#include "bvh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace
{
	std::shared_ptr<Entity> sphere;
	std::shared_ptr<Entity> plane0, plane1;
	std::shared_ptr<Entity> bunnyMesh;

	const float M_PI_MUL2 = 2.0f * static_cast<float>(M_PI);
}

AABB IDistanceList::GetBoundingBox() const
{
    AABB box = m_idistList[0]->GetBoundingBox();
    for(size_t i = 1; i < m_idistList.size(); ++i)
    {
        box = AABB::MergeAABB(
            box,
            m_idistList[i]->GetBoundingBox()
            );
    }
    return box;
}

DistanceInfo IDistanceList::GetDistanceInfo(Vector3 point, float time) const
{
	if(m_idistList.empty())
    {
        throw std::runtime_error("Empty IDistanceList!");
    }

    DistanceInfo currentInfo = m_idistList[0]->GetDistanceInfo(point, time);
    for(size_t i = 1; i < m_idistList.size(); ++i)
    {
		auto tmp = m_idistList[i]->GetDistanceInfo(point, time);
		if(tmp.distance < currentInfo.distance)
        	currentInfo = tmp;
    }
    return currentInfo;
}

AABB Scene::GetBoundingBox() const
{
	// scene acts as the root of BVH tree, this should never be called
	return m_distanceFuncProvider->GetBoundingBox();
}

DistanceInfo Scene::GetDistanceInfo(Vector3 point, float time) const
{
    // TODO : modify to use BVH for speed up
    return m_distanceFuncProvider->GetDistanceInfo(point, time);
}

void Scene::Update(float currentTime) const
{
	if (sphere)
		sphere->position() = Vector3(8.0f * (0.5f - currentTime));

	if (bunnyMesh)
		bunnyMesh->orientation() = Matrix3x3(
			{ std::cos(currentTime * M_PI_MUL2), 0, std::sin(currentTime * M_PI_MUL2) },
			{ 0, 1, 0 },
			{ -std::sin(currentTime * M_PI_MUL2), 0, std::cos(currentTime * M_PI_MUL2) }
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

	// const Transform bunnyTransform =
	// {
	// 	Vector3(1.0f, -2.0f, 0.0f),
	// 	Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
	// 	35.0f
	// };
	// bunnyLow
	const Transform bunnyTransform =
	{
		Vector3(0.0f, 0.0f, 0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		5.0f
	};

	const Transform plane0Transform = 
	{
		Vector3(0.0f, -3.0f, 0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		100.0f
	};
	
	// So basically I use two planes to cover the edge visual artifact
	const Transform plane1Transform =
	{
		Vector3(0.0f, 0.0f, -30.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		100.0f
	};

	sphere = std::make_shared<Sphere>(
		sphereTransform, 
		1.0f,
		Vector3(36,144,135) / 255.0f
		);

	// Using a large sphere as plane in a distance based renderer might not be
	// a good idea

	// plane = std::make_shared<Sphere>(
	// 	spherePlaneTransform,
	// 	1.0f,
	// 	Vector3(240, 136, 83) / 255.0f
	// 	);
	plane0 = std::make_shared<Cube>(
		plane0Transform,
		Vector3(1.0f, 0.01f, 1.0f),
		Vector3(240, 136, 83) / 255.0f
	);

	plane1 = std::make_shared<Cube>(
		plane1Transform,
		Vector3(1.0f, 1.0f, 0.1f),
		Vector3(83, 136, 240) / 255.0f
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
	// std::vector<IDistanceRef> bunny_list;
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
			// bunny_list.push_back(
			// 	std::make_shared<Triangle>(identityTransform, vertices, Vector3(230, 67, 83) / 255.0f )
			// );

			{
				auto rightTransform = identityTransform;
				rightTransform.position += Vector3(0.8f, 0.5f, -1.0f);
				rightTransform.scale *= 0.5f;
				
				bunny_list.emplace_back(rightTransform, vertices, Vector3(230, 67, 83) / 255.0f );
				// bunny_list.push_back(
				// 	std::make_shared<Triangle>(rightTransform, vertices, Vector3(230, 67, 83) / 255.0f )
				// );
			}
			{
				auto leftTransform = identityTransform;
				leftTransform.position += Vector3(-0.8f, 0.5f, -1.0f);
				leftTransform.scale *= 0.5f;

				bunny_list.emplace_back(leftTransform, vertices, Vector3(230, 67, 83) / 255.0f );
				// bunny_list.push_back(
				// 	std::make_shared<Triangle>(leftTransform, vertices, Vector3(230, 67, 83) / 255.0f )
				// );
			}

			index_offset += fv;
		}
	}

	// Update : The house-keeping code here does not cost a lot of time, and the difference
	// between copy and move constructor is little.
	//
	// Reference : https://stackoverflow.com/questions/40193269/does-the-use-of-stdmove-have-any-performance-benefits
	// This post states that the preformance gain from move constructor is from dynamic allocation being ruled out.
	//
	// Update : Remove incorrect comments. Use move semantics to construct std::vector does yield performance gain.
	// However, it needs about 1e7 integer elements to actually sees the difference.
	// 
	// using the "bunnyLow" mesh, which has 135 triangles. And duplicate the emplace_back process by 1e5
	//
	// for(int i=0;i<1e5;++i)
	//     bunny_list.emplace_back(identityTransform, vertices, Vector3(230, 67, 83) / 255.0f);
	//
	// this yields a significant performance difference. About 0~1 ms for move and 600~700 ms for copy

#if 1

	std::cout << "Using move constructor" << std::endl;

	// bunnyLow : 135, bunny : 4968
	std::cout << "bunny_list.size() = " << bunny_list.size() << std::endl;

	bunnyMesh = std::make_shared<TriangleMesh>(bunnyTransform, std::move(bunny_list));
	// bunnyMesh = std::make_shared<TransformedBVH>(bunnyTransform, std::move(bunny_list));

	std::vector<IDistanceRef> blendingEntityList
		= { sphere, bunnyMesh };
	IDistanceRef blendingEntity
		= std::make_shared<MeshBlender>(std::move(blendingEntityList), 3.0f);

	std::vector<IDistanceRef> sceneList
		= { blendingEntity, plane0, plane1 };
	m_distanceFuncProvider = std::make_shared<IDistanceList>(std::move(sceneList));

#else

	std::cout << "Using copy constructor" << std::endl;

	bunnyMesh = std::make_shared<TriangleMesh>(bunnyTransform, bunny_list);

	std::vector<IDistanceRef> blendingEntityList
		= { sphere, bunnyMesh };
	IDistanceRef blendingEntity
		= std::make_shared<MeshBlender>(blendingEntityList, 3.0f);

	std::vector<IDistanceRef> sceneList
		= { blendingEntity, plane0, plane1 };
	m_distanceFuncProvider = std::make_shared<IDistanceList>(sceneList);

#endif
}

Vector3 Scene::EvaluateNormal(Vector3 point, float time, float epsilon) const
{
	const Vector3 eps_x = Vector3(epsilon, 0.0f, 0.0f);
	const Vector3 eps_y = Vector3(0.0f, epsilon, 0.0f);
	const Vector3 eps_z = Vector3(0.0f, 0.0f, epsilon);

	Vector3 normal = Vector3(
		GetDistanceInfo(point + eps_x, time).distance - GetDistanceInfo(point - eps_x, time).distance,
		GetDistanceInfo(point + eps_y, time).distance - GetDistanceInfo(point - eps_y, time).distance,
		GetDistanceInfo(point + eps_z, time).distance - GetDistanceInfo(point - eps_z, time).distance);

	return safe_normalize(normal);
}