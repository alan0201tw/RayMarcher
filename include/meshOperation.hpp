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
        std::vector<IDistanceRef> entities,
        float k
        )
        : m_entities(entities), m_k(k) {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};