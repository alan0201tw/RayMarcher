#ifdef BUILDING_FOR_PYTHON_TESTING
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

namespace py = pybind11;
#endif

// cpp standard library
#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
#include <limits>
#include <vector>

// vendor library

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ThreadPool.h"

// header files included in project
#include "vec_math.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "meshOperation.hpp"

#include "scene.hpp"

namespace
{
    const size_t image_width = 800;
    const size_t image_height = 800;
    const float width_to_height_ratio = (float)image_width / (float)image_height;

    unsigned char image[3 * image_width * image_height];

    typedef Vector3 rgb01;

	Scene scene;
}

void write_to_image(size_t u, size_t v, rgb01 value)
{
    value[0] = std::sqrt(value[0]);
    value[1] = std::sqrt(value[1]);
    value[2] = std::sqrt(value[2]);

    image[v * 3 * image_width + u * 3 + 0] = static_cast<unsigned char>(255 * value[0]);
    image[v * 3 * image_width + u * 3 + 1] = static_cast<unsigned char>(255 * value[1]);
    image[v * 3 * image_width + u * 3 + 2] = static_cast<unsigned char>(255 * value[2]);
}

rgb01 GetColor(float u, float v, float currentTime)
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

    for (int i = 0; i < MAX_ITER; ++i)
    {
        // Either we've hit the object or hit nothing at all, either way we should break out of the loop
        if (dist < EPSILON || totalDist > MAX_DIST)
            break;

        auto info = scene.GetDistanceInfo(pos, currentTime); // Evalulate the distance at the current point
        dist = info.distance;
        totalDist += dist;
        pos += dist * ray.GetDirection(); // Advance the point forwards in the ray direction by the distance
    
        if(dist < EPSILON)
        {
            Vector3 normal = scene.EvaluateNormal(pos, currentTime, EPSILON);
            
            float diffuse = std::max(0.0f, dot(-ray.GetDirection(), normal));
            float specular = pow(diffuse, 32.0f);

            float light_intensity = diffuse + specular;
            Vector3 color = 
                std::clamp(light_intensity, 0.0f, 1.0f) * info.color;

            return color;
        }
    }
    return {0, 0, 0};
}

static void Render(float currentTime)
{
    // using thread pool for parallelism
    // for simple scene, thread overhead actually slows down the process
    // when it becomes more complex, switch to 2D parallel
#if 1
    ThreadPool pool(8);

    for(size_t i = 0; i < image_width; ++i)
    {
        auto result = pool.enqueue([currentTime](size_t i)
        {
            for(size_t j = 0; j < image_height; ++j)
            {
                rgb01 color = GetColor(
                    (float)i / (float)image_width, 
                    (float)j / (float)image_height,
                    currentTime);

                write_to_image(i, j, color);
            }
        }, i);
    }
#else // naive implementation
    for(size_t i = 0; i < image_width; ++i)
    {
        for(size_t j = 0; j < image_height; ++j)
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

#ifndef BUILDING_FOR_PYTHON_TESTING

int main(int argc, char* argv[])
{
    int imageIndex = 0;
    std::stringstream ss;

    // time parameters to support animation output
    float currentTime = 0.0f;

    while(currentTime <= 1.0f)
    {
        ss << "output/output_";
        ss << imageIndex;
        ss << ".png";

        scene.Update(currentTime);
		
        Render(currentTime);
        Output(ss.str());

        currentTime += 0.01f;

        ++imageIndex;
        ss.str(std::string());
    }
}

#else

PYBIND11_MODULE(_rayMarcher, mod)
{
    mod.doc() = "Ray marcher";

    py::class_<Scene>(mod, "Scene")
        .def(py::init<>());

    mod.def("render", &Render, "render the image");
    mod.def("output", &Output, "output the image");
}

#endif