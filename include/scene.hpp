#pragma once

#include "transform.hpp"
#include "entity.hpp"
#include "meshOperation.hpp"

class IDistanceList final : public IDistance
{
private:
	std::vector< IDistanceRef > m_idistList;

public:
	explicit IDistanceList(std::vector< IDistanceRef > elements)
		: m_idistList(elements)
		{}
	~IDistanceList() {}

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class Scene final: public IDistance, public IUpdate
{
private:
	IDistanceRef m_distanceFuncProvider;

public:
	explicit Scene();
	~Scene() {}

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
	virtual void Update(float currentTime) const final override;

	Vector3 EvaluateNormal(Vector3 point, float time, float epsilon = 0.0001f) const;
};