#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ThreadPool.h"

#include "vector3.hpp"
#include "camera.hpp"

#include "entity.hpp"
#include "meshOperation.hpp"

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

void write_to_image(size_t u, size_t v, rgb01 value)
{
    image[v * 3 * image_width + u * 3 + 0] = static_cast<unsigned char>(255 * value[0]);
    image[v * 3 * image_width + u * 3 + 1] = static_cast<unsigned char>(255 * value[1]);
    image[v * 3 * image_width + u * 3 + 2] = static_cast<unsigned char>(255 * value[2]);
}

float distfunc(Vector3 pos)
{
    static std::shared_ptr<Entity> sphere
        = std::make_shared<Sphere>(Vector3(0.0f), 1.0f);
    static std::shared_ptr<Entity> cube
        = std::make_shared<Cube>(Vector3(0.0f), Vector3(1.0f));
    static std::shared_ptr<Entity> sphere1
        = std::make_shared<Sphere>(Vector3(0.0f), 1.0f);
    static std::vector<std::shared_ptr<Entity>> entityList
        = { sphere, cube, sphere1 };
    static std::shared_ptr<Entity> blendingEntity
        = std::make_shared<MeshBlender>(entityList);

    sphere->m_position = Vector3(1.5f - tmp);
    sphere1->m_position = Vector3(0.0f, 0.0f, tmp);

    return blendingEntity->Distance(pos);
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
    const float MAX_DIST = 20.0f; // Make sure you change this if you have objects farther than 20 units away from the camera
    const float EPSILON = 0.001f; // At this distance we are close enough to the object that we have essentially hit it

    Ray ray = camera.get_ray(u, v);

    float totalDist = 0.0;
    Vector3 pos = ray.GetOrigin();
    float dist = EPSILON;

    for (int i = 0; i < MAX_ITER; i++)
    {
        // Either we've hit the object or hit nothing at all, either way we should break out of the loop
        if (dist < EPSILON || totalDist > MAX_DIST)
            break;

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
    // using thread pool for parallelism
#if 1
    ThreadPool pool(image_width);

    for(size_t i = 0; i < image_width; i++)
    {
        auto result = pool.enqueue([](size_t i)
        {
            for(size_t j = 0; j < image_height; j++)
            {
                rgb01 color = GetColor((float)i / (float)image_width, 
                    (float)j / (float)image_height);

                write_to_image(i, j, color);
            }
        }, i);
    }
#else // naive implementation
    for(size_t i = 0; i < image_width; i++)
    {
        for(size_t j = 0; j < image_height; j++)
        {
            rgb01 color = GetColor((float)i / (float)image_width, 
                  (float)j / (float)image_height);

            write_to_image(i, j, color);
        }
    }
#endif
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
    tmp = -5.0;
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