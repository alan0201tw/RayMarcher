#include <iostream>

#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace
{
    const size_t image_width = 800;
    const size_t image_height = 800;

    unsigned char image[3 * image_width * image_height];

    typedef std::array<unsigned char, 3> rgb255;
    typedef std::array<float, 3> rgb01;
}

inline void write_to_image(size_t u, size_t v, rgb01 value)
{
    image[v * 3 * image_width + u * 3 + 0] = (unsigned char)255 * value[0];
    image[v * 3 * image_width + u * 3 + 1] = (unsigned char)255 * value[1];
    image[v * 3 * image_width + u * 3 + 2] = (unsigned char)255 * value[2];
}

int main(int argc, char* argv[])
{
    for(size_t i = 0; i < image_width; i++)
    {
        for(size_t j = 0; j < image_height; j++)
        {
            write_to_image(i, j, 
                { (float)i / (float)image_width, 
                  (float)j / (float)image_height, 
                  0 } );
        }
    }

    // stbi_flip_vertically_on_write(true);
    if(stbi_write_png("output.png", image_width, image_height, 3, image, 0) == 0)
    {
        std::cerr << "Error when saving image\n";
    }
}