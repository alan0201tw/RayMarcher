#pragma once

#include <memory>

#include "vec_math.hpp"

struct AABB;

struct DistanceInfo
{
    float distance = 0.0f;
    Vector3 color;
};

class IDistance
{
public:
    virtual DistanceInfo GetDistanceInfo(Vector3 point, float time) const = 0;
	// for BVH
	virtual AABB GetBoundingBox() const = 0;
};

class IUpdate
{
public:
	virtual void Update(float currentTime) = 0;
};

using IDistanceRef = std::shared_ptr<IDistance>;
using IUpdateRef = std::shared_ptr<IUpdate>;

// using IDistanceRef = IDistance*;
// using IUpdateRef = IUpdate*;