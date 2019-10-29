#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vector3.hpp"
#include "camera.hpp"

namespace
{
    const size_t image_width = 800;
    const size_t image_height = 800;
    const float width_to_height_ratio = (float)image_width / (float)image_height;

    unsigned char image[3 * image_width * image_height];

    typedef std::array<float, 3> rgb01;

    // just tmp values
    float tmp = 0.0f;
}

inline void write_to_image(size_t u, size_t v, rgb01 value)
{
    image[v * 3 * image_width + u * 3 + 0] = (unsigned char)255 * value[0];
    image[v * 3 * image_width + u * 3 + 1] = (unsigned char)255 * value[1];
    image[v * 3 * image_width + u * 3 + 2] = (unsigned char)255 * value[2];
}

float sphere(Vector3 pos, float radius)
{
	return length(pos) - radius;
}

float box(Vector3 pos, Vector3 size)
{
    return length(max(abs(pos) - size, 0.0));
}

float distfunc(Vector3 pos)
{
    float d0 = box(pos, Vector3(1.0f));
    float d1 = sphere(pos - Vector3(1.5f - tmp), 1.0f);

    const float k = 3.0f;
    float h = std::max( k - abs(d0 - d1), 0.0f ) / k;

    return std::min(d0, d1) - h*h*h*k*(1.0f/6.0f);
}

rgb01 GetColor(float u, float v)
{
    static Camera camera(
        Vector3(3.0f, 3.0f, 10.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        60.0f,
        width_to_height_ratio
    );

    const int MAX_ITER = 100; // 100 is a safe number to use, it won't produce too many artifacts and still be quite fast
    const float MAX_DIST = 20.0; // Make sure you change this if you have objects farther than 20 units away from the camera
    const float EPSILON = 0.001; // At this distance we are close enough to the object that we have essentially hit it

    Ray ray = camera.get_ray(u, v);

    float totalDist = 0.0;
    Vector3 pos = ray.GetOrigin();
    float dist = EPSILON;

    for (int i = 0; i < MAX_ITER; i++)
    {
        // std::cout << "pos = " << pos.x << 
        //     ", " << pos.y <<
        //     ", " << pos.z << std::endl;
        // Either we've hit the object or hit nothing at all, either way we should break out of the loop
        if (dist < EPSILON || totalDist > MAX_DIST)
            break; // If you use windows and the shader isn't working properly, change this to continue;

        dist = distfunc(pos); // Evalulate the distance at the current point
        totalDist += dist;
        pos += dist * ray.GetDirection(); // Advance the point forwards in the ray direction by the distance
    
        if(dist < EPSILON)
        {
            return {1, 1, 1};
        }
    }
    return {0, 0, 0};
}

static void Render()
{
    for(size_t i = 0; i < image_width; i++)
    {
        for(size_t j = 0; j < image_height; j++)
        {
            rgb01 color = GetColor((float)i / (float)image_width, 
                  (float)j / (float)image_height);

            write_to_image(i, j, color);
        }
    }
}

static void Output(std::string fileName)
{
    // std::cout << "fileName = " << fileName << std::endl;
    stbi_flip_vertically_on_write(true);
    if(stbi_write_png(fileName.c_str(), image_width, image_height, 3, image, 0) == 0)
    {
        std::cerr << "Error when saving image\n";
    }
}

int main(int argc, char* argv[])
{
    int image_index = 0;
    std::stringstream ss;
    tmp = -5;
    while(tmp <= 5.0f)
    {
        ss << "output/output_";
        ss << image_index;
        ss << ".png";

        Render();
        Output(ss.str());

        tmp += 0.1f;
        image_index++;
        ss.str(std::string());
    }
}