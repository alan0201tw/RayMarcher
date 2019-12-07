#include "bvh.hpp"

#include <algorithm>

float AABB::GetDistance(Vector3 point) const
{
    const Vector3 extent = m_max - m_min;
    const Vector3 o = abs(point) - extent;
    const float ud = length(max(o, 0.0f));
    const float n = std::max(std::max(
        std::min(o[0], 0.0f), std::min(o[1], 0.0f)), std::min(o[2], 0.0f));

    return ud + n;
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

BVH::BVH(std::vector<IDistanceRef>&& elements)
    : m_elements(std::move(elements))
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

    DistanceInfo leftInfo = m_leftBVH->GetDistanceInfo(pos, time);
    DistanceInfo rightInfo = m_rightBVH->GetDistanceInfo(pos, time);

    // return the left bvh info
    if(leftInfo.distance < rightInfo.distance)
        return leftInfo;
    // return the right bvh info
    return rightInfo;
}

AABB BVH::GetBoundingBox() const
{
    return m_boundingBox;
}

AABB TransformedBVH::GetBoundingBox() const
{
    Vector3 minimal(1e9, 1e9, 1e9);
    Vector3 maximal(-1e9, -1e9, -1e9);

    const AABB boundingBox = m_bvh.GetBoundingBox();

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
                tester = ApplyTransform(m_transform, tester);
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

    return AABB(minimal, maximal);
}

DistanceInfo TransformedBVH::GetDistanceInfo(Vector3 point, float time) const
{
    Vector3 pos = ApplyInverseTransform(m_transform, point);
    DistanceInfo info = m_bvh.GetDistanceInfo(pos, time);
    // do inverse transform on distance,
    // a scalar needs only scaling
    info.distance *= m_transform.scale;

    return info;
}