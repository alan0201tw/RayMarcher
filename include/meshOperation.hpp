#pragma once

#include "entity.hpp"

#include <memory>
#include <vector>

class MeshBlender final : public Entity
{
private:
    std::vector< std::shared_ptr<Entity> > m_entities;

    const float m_k;

    static float SmoothDistance(float d0, float d1, const float k);

public:
    explicit MeshBlender(
        std::vector< std::shared_ptr<Entity> > entities,
        float k
        )
        : Entity(Vector3(0.0f)), m_entities(entities), m_k(k) {}

    virtual float Distance(Vector3 point) const final override;
};