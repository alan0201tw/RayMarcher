# (Ray Marcher) Ray Marching Renderer

## Build

* On Linux, just use the makefile

* On Windows, please use [premake](https://github.com/premake/premake-core) for generating Visual Studio solution
    * Only `premake vs2017` is used in development

## Features

* Basic geometries : sphere, cube, triangle
* Triangle mesh support (.obj files)
* Shadow ray
* Transformable entities
    * Translate, Rotation and Uniform Scaling
* Mesh Blending and Masking
* Bounding Volume Hierarchy using Axis-Aligned Bounding Box
    * This is pretty interesting. Instead of the traditional BVH in a ray-tracer, I used a technique similar to tree pruning, basically I traverse to the bottom of the right subtree and the top node of the left subtree, if the minimum distance found in the right subtree is larger than the top node of the left subtree, the minimum distanced geometry is guaranteed to be in the right subtree.

## Demo :

* This costs 33m8.356s to render on my desktop computer
![result](./exports/ray_marcher.gif)
