#pragma once

#include "transform.hpp"
#include "entity.hpp"
#include "meshOperation.hpp"

class IDistanceList final : public IDistance
{
private:
	std::vector< IDistanceRef > m_idistList;

public:
	explicit IDistanceList(const std::vector< IDistanceRef >& elements)
		: m_idistList(elements)
		{}
	explicit IDistanceList(std::vector< IDistanceRef >&& elements)
		: m_idistList(std::move(elements))
		{}
	~IDistanceList() {}

	IDistanceList(const IDistanceList& other) = default;
	IDistanceList(IDistanceList&& other) = default;
	IDistanceList& operator=(IDistanceList const &) = default;
	IDistanceList& operator=(IDistanceList &&) = default;

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};

class Scene final: public IDistance, public IUpdate
{
private:
	IDistanceRef m_distanceFuncProvider;

public:
	explicit Scene();
	~Scene() {}

	// NonCopyable: Restrict copy constructor and assign operator
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(Scene const &) = delete;
	Scene& operator=(Scene &&) = delete;

	virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
	virtual void Update(float currentTime) final override;

	Vector3 EvaluateNormal(Vector3 point, float time, float epsilon = 0.0001f) const;
};