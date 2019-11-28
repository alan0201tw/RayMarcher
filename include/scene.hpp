#pragma once

#include "transform.hpp"
#include "entity.hpp"
#include "meshOperation.hpp"

class Scene
{
public:

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

	std::shared_ptr<Entity> sphere
		= std::make_shared<Sphere>(sphereTransform, 1.0f, [&](float current_time)
	{
		sphere->m_transform.position = Vector3(8.0f * (0.5f - current_time));
	});

	std::shared_ptr<Entity> cube
		= std::make_shared<Cube>(cubeTransform, Vector3(1.0f), [&](float current_time)
	{
		cube->m_transform.orientation = Matrix3x3(
			{ std::cos(current_time * 5.0f), 0, std::sin(current_time * 5.0f) },
			{ 0, 1, 0 },
			{ -std::sin(current_time * 5.0f), 0, std::cos(current_time * 5.0f) }
		);
	});

	std::vector<std::shared_ptr<IDistance>> entityList
		= { sphere, cube };
	std::shared_ptr<IDistance> blendingEntity
		= std::make_shared<MeshBlender>(entityList, 3.0f);
};