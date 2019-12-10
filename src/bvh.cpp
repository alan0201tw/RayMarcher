#include "bvh.hpp"

#include <algorithm>
#include <iostream>

float AABB::GetDistance(Vector3 point) const
{
    float dx = std::max(m_min.x - point.x, point.x - m_max.x);
    float dy = std::max(m_min.y - point.y, point.y - m_max.y);
    float dz = std::max(m_min.z - point.z, point.z - m_max.z);

    dx = std::max(0.0f, dx);
    dy = std::max(0.0f, dy);
    dz = std::max(0.0f, dz);

    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

// static method
AABB AABB::MergeAABB(const AABB& a, const AABB& b)
{
    Vector3 minimum( std::min( a.GetMin().x, b.GetMin().x ),
                     std::min( a.GetMin().y, b.GetMin().y ),
                     std::min( a.GetMin().z, b.GetMin().z ));
    Vector3 maximum( std::max( a.GetMax().x, b.GetMax().x ),
                     std::max( a.GetMax().y, b.GetMax().y ),
                     std::max( a.GetMax().z, b.GetMax().z ));
    
    return AABB(minimum, maximum);
}

/////////////////////////////////////////////////////////////////////

bool BVH::box_x_compare::operator()(const IDistanceRef& a, const IDistanceRef& b) const
{
    AABB left = a->GetBoundingBox();
    AABB right = b->GetBoundingBox();

    if(left.GetMin().x - right.GetMin().x <= 0.0f)
        return false;
    else
        return true;
}

bool BVH::box_y_compare::operator()(const IDistanceRef& a, const IDistanceRef& b) const
{
    AABB left = a->GetBoundingBox();
    AABB right = b->GetBoundingBox();

    if(left.GetMin().y - right.GetMin().y <= 0.0f)
        return false;
    else
        return true;
}

bool BVH::box_z_compare::operator()(const IDistanceRef& a, const IDistanceRef& b) const
{
    AABB left = a->GetBoundingBox();
    AABB right = b->GetBoundingBox();

    if(left.GetMin().z - right.GetMin().z <= 0.0f)
        return false;
    else
        return true;
}

BVH::BVH(const std::vector<IDistanceRef>& elements)
    : m_elements(elements)
{
    UpdateBVH();
}

BVH::BVH(std::vector<IDistanceRef>&& elements)
    : m_elements(std::move(elements))
{
    UpdateBVH();
}

void BVH::Update(float currentTime)
{
    UpdateBVH();
}

void BVH::UpdateBVH()
{
    size_t random_axis = rand() % 3;
    if(random_axis == 0)
        std::sort(m_elements.begin(), m_elements.end(), box_x_compare());
    else if(random_axis == 1)
        std::sort(m_elements.begin(), m_elements.end(), box_y_compare());
    else
        std::sort(m_elements.begin(), m_elements.end(), box_z_compare());

    size_t elementSize = (size_t)m_elements.size();

    if(elementSize == 1)
        m_leftBVH = m_rightBVH = m_elements[0];
    else if(elementSize == 2)
    {
        m_leftBVH = m_elements[0];
        m_rightBVH = m_elements[1];
    }
    else
    {
        size_t half_size = elementSize / 2;

        std::vector<IDistanceRef> left_list(
            m_elements.begin(), m_elements.begin() + half_size);
        std::vector<IDistanceRef> right_list(
            m_elements.begin() + half_size, m_elements.end());

        m_leftBVH = std::make_shared<BVH>(std::move(left_list));
        m_rightBVH = std::make_shared<BVH>(std::move(right_list));
    }

    m_boundingBox = AABB::MergeAABB(
        m_leftBVH->GetBoundingBox(),
        m_rightBVH->GetBoundingBox()
    );
}

DistanceInfo BVH::GetDistanceInfo(Vector3 point, float time) const
{
    const Vector3 pos = point;

    const DistanceInfo rightInfo = m_rightBVH->GetDistanceInfo(pos, time);
    if(m_leftBVH->GetBoundingBox().GetDistance(pos) > rightInfo.distance)
    {
        // if the largest distance from right node is lesser than
        // the smallest distance from left node, skip right node and 
        // just return right info
        return rightInfo;
    }
    else
    {
        auto leftInfo = m_leftBVH->GetDistanceInfo(pos, time);
        if(leftInfo.distance < rightInfo.distance)
            return leftInfo;
        return rightInfo;
    }

    // // return the left bvh info
    // if(leftBBDist < rightBBDist)
    //     return m_leftBVH->GetDistanceInfo(point, time);
    // // return the right bvh info
    // return m_rightBVH->GetDistanceInfo(point, time);
}

AABB BVH::GetBoundingBox() const
{
    return m_boundingBox;
}

////////////////////////////////////////////////////////////////

AABB TransformedBVH::GetTransformedAABB(Transform transform, AABB aabb)
{
    Vector3 minimal(1e9, 1e9, 1e9);
    Vector3 maximal(-1e9, -1e9, -1e9);

    const AABB boundingBox = aabb;

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            for(int k = 0; k < 2; k++)
            {
                float x = i * boundingBox.GetMax().x
                    + (1 - i) * boundingBox.GetMin().x;
                float y = j * boundingBox.GetMax().y
                    + (1 - j) * boundingBox.GetMin().y;
                float z = k * boundingBox.GetMax().z
                    + (1 - k) * boundingBox.GetMin().z;

                Vector3 tester(x, y, z);
                tester = ApplyTransform(transform, tester);
                for(int c = 0; c < 3; c++)
                {
                    if(tester[c] > maximal[c])
                        maximal[c] = tester[c];
                    if(tester[c] < minimal[c])
                        minimal[c] = tester[c];
                }
            }
        }
    }

    // std::cout << "minimal = "
    //     << minimal.x << ", "
    //     << minimal.y << ", "
    //     << minimal.z << std::endl;

    // std::cout << "maximal = "
    //     << maximal.x << ", "
    //     << maximal.y << ", "
    //     << maximal.z << std::endl;

    return AABB(minimal, maximal);
}

AABB TransformedBVH::GetBoundingBox() const
{
    // return m_bvh.GetBoundingBox();
    return GetTransformedAABB(m_transform, m_bvh.GetBoundingBox());
}

DistanceInfo TransformedBVH::GetDistanceInfo(Vector3 point, float time) const
{
    Vector3 pos = ApplyInverseTransform(m_transform, point);

    DistanceInfo info = m_bvh.GetDistanceInfo(pos, time);

    info.distance *= m_transform.scale;

    return info;
}

void TransformedBVH::Update(float currentTime)
{
    m_bvh.Update(currentTime);
}
