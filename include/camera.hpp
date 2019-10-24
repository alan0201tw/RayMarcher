#pragma once

#include "vector3.hpp"
#include "ray.hpp"

class Camera
{
public:

    Camera(const Vector3& _lookfrom, const Vector3& _lookat, 
           const Vector3& _up , float _vfov, float _aspect)
    {
        float theta = _vfov * M_PI / 180.0f;
        float half_height = std::tan(theta / 2.0f);
        float half_width = _aspect * half_height;

        m_origin = _lookfrom;
        m_camera_forward = normalize(_lookfrom - _lookat);
        m_camera_right = normalize( cross(_up, m_camera_forward) );
        m_camera_up = normalize( cross(m_camera_forward, m_camera_right) );

        m_lower_left_corner = m_origin - half_width * m_camera_right 
            - half_height * m_camera_up - m_camera_forward;
        
        m_horizontal = 2.0f * half_width * m_camera_right;
        m_vertical = 2.0f * half_height * m_camera_up;
    }
    
    inline Ray get_ray(float u, float v) const
    {
        return Ray(m_origin, 
            m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
    }

    Vector3 m_lower_left_corner; 
    Vector3 m_horizontal;
    Vector3 m_vertical;
    Vector3 m_origin;

    Vector3 m_camera_forward;
    Vector3 m_camera_right;
    Vector3 m_camera_up;
};