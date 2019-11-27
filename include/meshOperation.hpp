#pragma once

#include "entity.hpp"

#include <memory>
#include <vector>

class MeshBlender final : public IDistance
{
private:
    std::vector< std::shared_ptr<IDistance> > m_entities;

    const float m_k;

    static DistanceInfo SmoothDistance(
        DistanceInfo d0, DistanceInfo d1, const float k);

public:
    explicit MeshBlender(
        std::vector< std::shared_ptr<IDistance> > entities,
        float k
        )
        : m_entities(entities), m_k(k) {}

    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const final override;
};