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

    unsigned char r255 = static_cast<unsigned char>(255.99f * value[0]);
    unsigned char g255 = static_cast<unsigned char>(255.99f * value[1]);
    unsigned char b255 = static_cast<unsigned char>(255.99f * value[2]);

    unsigned char lower_bound = 0, upper_bound = 255;

    r255 = std::clamp(r255, lower_bound, upper_bound);
    g255 = std::clamp(g255, lower_bound, upper_bound);
    b255 = std::clamp(b255, lower_bound, upper_bound);

    image[v * 3 * image_width + u * 3 + 0] = r255;
    image[v * 3 * image_width + u * 3 + 1] = g255;
    image[v * 3 * image_width + u * 3 + 2] = b255;
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
    const float MAX_DIST = 200.0f; // Make sure you change this if you have objects farther than 20 units away from the camera
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

        if(dist <= EPSILON)
        {
            // Reference : https://github.com/SebLague/Ray-Marching/blob/master/Assets/Scripts/SDF/Raymarching.compute#L223
            // Use the point slightly away from surface to evaluate normal
            // TEST Result : HOLY SHIT, this works great.
            const Vector3 pointOnSurface = pos + (dist - EPSILON) * ray.GetDirection();
            const Vector3 normal = scene.EvaluateNormal(pointOnSurface, currentTime, EPSILON);
            const Vector3 lightDirection = safe_normalize(-ray.GetDirection());

            const Vector3 I = -lightDirection;
            const Vector3 N = normal;
            const Vector3 reflectLightDir = I - 2.0f * linalg::dot(N, I) * N;

            const Vector3 cameraDirection = safe_normalize(ray.GetOrigin() - pos);

            float diffuse = std::max(0.0f, dot(lightDirection, normal));
			// simulate the glsl saturate function : https://developer.download.nvidia.com/cg/saturate.html
			// diffuse = std::max(0.0f, std::min(1.0f, diffuse));

            float specular = 
                std::pow(std::max(linalg::dot(reflectLightDir, cameraDirection), 0.0f), 32.0f);

            Vector3 color = 
                std::clamp(diffuse, 0.0f, 1.0f) * info.color + specular;

            return linalg::clamp(color, 0.0f, 1.0f);
        }

		// Crutial : update pos after we evaluate normal, or we might get a penetrated normal, which leads to lighting bug

		totalDist += dist;
		pos += dist * ray.GetDirection(); // Advance the point forwards in the ray direction by the distance
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