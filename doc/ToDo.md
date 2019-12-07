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
    * (Moved to bottom) Triangle lighting quality is poor, maybe lower epsilon for evaluating normal?
    
    * BVH has to be constructed every frame 
        * since it depends on transform, which updates every frame
    * Each entity type has to consider its own transform in its GetBoundingBox method
        * currently only consider position

    * bunnyLow.obj without BVH
    ```
    real	82m4.230s
    user	271m59.557s
    sys	    0m35.908s
    ```
    * bunnyLow.obj With BVH
    ```
    real    95m13.382s
    user    312m0.187s
    sys     0m36.864s
    ```
    * bunny.obj with BVH ( WSL ubuntu )
    ```
    real    20m55.495s
    user    0m0.000s
    sys     0m0.000s
    ```
    * bunny.obj without BVH
    ```
    real    14m16.287s
    user    0m0.000s
    sys     0m0.000s 
    ```


* (O) Transform : Use transpost for inversing rotation

* (O) Improve lighting quality
    * Try sphere marching? -> compute every distance and use as ray march step size
    * Use new position for normal evaluation : a point slightly out of the point on surface, referencing SebLague's RayMarching repo

* Use Ray tracer to produce golden ( ground truth )

* (O - currently use default) Apply rule of five ( if the code becomes too spaghetti, perhaps cast some of the operator to delete? )
    * Consider using std::move with std::unique_ptr ?

* (X - Solve be editing scene content) 
    * Handle object edge by handling case when ray iteration exceeds MAX_ITER
    * the return {0, 0, 0} case
    * this should be replaced by the closest point info that still is over EPSILON