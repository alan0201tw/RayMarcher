#pragma once

#include "vec_math.hpp"
#include "ray.hpp"

class Camera final
{
public:

    explicit Camera(
        const Vector3& lookfrom, 
        const Vector3& lookat, 
        const Vector3& up,
        float vfov,
        float aspect)
    {
        float theta = static_cast<float>(vfov * M_PI / 180.0f);
        float half_height = std::tan(theta / 2.0f);
        float half_width = aspect * half_height;

        m_origin = lookfrom;
        m_camera_forward = normalize(lookfrom - lookat);
        m_camera_right = normalize( cross(up, m_camera_forward) );
        m_camera_up = normalize( cross(m_camera_forward, m_camera_right) );

        m_lower_left_corner = m_origin - half_width * m_camera_right 
            - half_height * m_camera_up - m_camera_forward;
        
        m_horizontal = 2.0f * half_width * m_camera_right;
        m_vertical = 2.0f * half_height * m_camera_up;
    }

    ~Camera() {}

	Camera(const Camera& other) = default;
	Camera(Camera&& other) = default;
	Camera& operator=(Camera const &) = default;
	Camera& operator=(Camera &&) = default;
    
    inline Ray get_ray(float u, float v) const
    {
        return Ray(m_origin, 
            normalize(m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin));
    }

    Vector3 m_lower_left_corner; 
    Vector3 m_horizontal;
    Vector3 m_vertical;
    Vector3 m_origin;

    Vector3 m_camera_forward;
    Vector3 m_camera_right;
    Vector3 m_camera_up;
};