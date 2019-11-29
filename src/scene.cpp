#include "scene.hpp"

#include <iostream>

namespace
{
	std::shared_ptr<Entity> sphere;
	std::shared_ptr<Entity> cube;
}

DistanceInfo Scene::GetDistanceInfo(Vector3 point, float time) const
{
    // TODO : modify to use BVH for speed up
    return m_distanceFuncProvider->GetDistanceInfo(point, time);
}

void Scene::Update(float currentTime) const
{
	sphere->position() = Vector3(8.0f * (0.5f - currentTime));

    cube->orientation() = Matrix3x3(
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
		Vector3(1.0f)
	};

	const Transform cubeTransform =
	{
		Vector3(0.0f),
		Matrix3x3({1,0,0}, {0,1,0}, {0,0,1}),
		Vector3(1.0f)
	};

	sphere = std::make_shared<Sphere>(sphereTransform, 1.0f);
	cube = std::make_shared<Cube>(cubeTransform, Vector3(1.0f));

	std::vector<IDistanceRef> entityList
		= { sphere, cube };
	IDistanceRef blendingEntity
		= std::make_shared<MeshBlender>(entityList, 3.0f);

    m_distanceFuncProvider = blendingEntity;
}

Vector3 Scene::EvaluateNormal(Vector3 point, float time, float epsilon) const
{
	const Vector3 eps_x = Vector3(epsilon, 0.0f, 0.0f);
	const Vector3 eps_y = Vector3(0.0f, epsilon, 0.0f);
	const Vector3 eps_z = Vector3(0.0f, 0.0f, epsilon);

	Vector3 normal = linalg::normalize(Vector3(
		GetDistanceInfo(point + eps_x, time).distance - GetDistanceInfo(point - eps_x, time).distance,
		GetDistanceInfo(point + eps_y, time).distance - GetDistanceInfo(point - eps_y, time).distance,
		GetDistanceInfo(point + eps_z, time).distance - GetDistanceInfo(point - eps_z, time).distance));

	return normal;
}