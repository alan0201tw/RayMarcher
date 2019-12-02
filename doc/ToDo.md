* NSD_RayMarcher
    * Continue implementation

    * Some more primitives

    * (O) Lighting (Phong or Blinn Phong, entity contain material, material contain texture)
    * (O) Transform (Translate and Rotation, mimic Ray tracer)
    * (?) Precompiled Header - on stb_image and thread_pool ?
    * (O) Triangle Mesh
    * (?) Texture mapping
        * It's hard to come up with a elegant architecture for this.
        * To do texture mapping, multiple rays is required to estimate UVs, like how normals are obtained.

        * Well, it is also possible to do it with the current position of ray-marching, but it will lead to visual artifacts
        * Reference : https://iquilezles.org/www/articles/filteringrm/filteringrm.htm

        * To solve this, we need multiple rays, that means we cannot coordinate mesh operations well.
        * Furthermore, the articles uses many helper functions related to textures defined in glsl.
        * I cannot use those in this project, so it might took a while for me to implement those.

    * Testing, maybe?
        * Google test?

    * Analysis ( with Ray tracer, quality, performance ... etc )
        * Compare shared_ptr and raw pointer performance
        * That I do not call make_shared while multi-threading

    * BVH or Parallel triangle SDF in triangle mesh
        * Current performance is way too poor
        * One image needs a few seconds?
        * Triangle lighting quality is poor, maybe lower epsilon for evaluating normal?

    * (O) Transform : Use transpost for inversing rotation

    * (O) Improve lighting quality
        * Try sphere marching? -> compute every distance and use as ray march step size
        * Use new position for normal evaluation : a point slightly out of the point on surface, referencing SebLague's RayMarching repo