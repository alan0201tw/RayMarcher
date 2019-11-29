#pragma once

#include "transform.hpp"
#include "entity.hpp"
#include "meshOperation.hpp"

class Scene : public IDistance, public IUpdate
{
private:
	IDistanceRef m_distanceFuncProvider;

public:
	explicit Scene();

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
	virtual void Update(float currentTime) const final override;

	Vector3 EvaluateNormal(Vector3 point, float time, float epsilon = 0.0001f) const;
};