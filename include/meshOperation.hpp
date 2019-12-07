#pragma once

#include <memory>
#include <vector>

#include "entity.hpp"

class MeshBlender final : public IDistance
{
private:
    std::vector<IDistanceRef> m_entities;

    const float m_k;

    static DistanceInfo SmoothDistance(
        DistanceInfo d0, DistanceInfo d1, const float k);

public:
    explicit MeshBlender(
        const std::vector<IDistanceRef>& entities,
        const float& k
        )
        : m_entities(entities), m_k(k) {}

	explicit MeshBlender(
		std::vector<IDistanceRef>&& entities,
		const float& k
	)
		: m_entities(std::move(entities)), m_k(k) {}

    ~MeshBlender() {}

    virtual AABB GetBoundingBox() const final override;
    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;

	MeshBlender(const MeshBlender& other) = default;
	MeshBlender(MeshBlender&& other) = default;
	MeshBlender& operator=(MeshBlender const &) = default;
	MeshBlender& operator=(MeshBlender &&) = default;
};