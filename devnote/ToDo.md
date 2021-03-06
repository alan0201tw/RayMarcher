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

* (O) BVH or Parallel triangle SDF in triangle mesh
    * Current performance is way too poor
    * One image needs a few seconds?
    * (Moved to bottom) Triangle lighting quality is poor, maybe lower epsilon for evaluating normal?
    
    * (X) BVH has to be constructed every frame 
        * since it depends on transform, which updates every frame
    * Each entity type has to consider its own transform in its GetBoundingBox method
        * currently only consider position
    ```
    [Single thread]
    Using move constructor
    bunny_list.size() = 135
    Triangle::GetBoundingBox() is called 406742484 times
    Triangle::GetDistanceInfo() is called 717142022 times

    real    0m57.920s
    user    0m57.912s
    sys     0m0.005s
    ---------------------
    Using move constructor
    bunny_list.size() = 135
    Triangle::GetBoundingBox() is called 0 times
    Triangle::GetDistanceInfo() is called 1904521005 times

    real    2m5.179s
    user    2m5.158s
    sys     0m0.017s
    ---------------------
    [MultiThread with BVH]
    Using move constructor
    bunny_list.size() = 135

    real    0m25.508s
    user    1m25.397s
    sys     0m0.742s
    ---------------------
    [MultiThread without BVH]
    Using move constructor
    bunny_list.size() = 135

    real    0m50.308s
    user    2m42.952s
    sys     0m0.290s
    ---------------------
    [MultiThread without BVH, on my Windows machine]
    bunny.obj , 4968 triangles
    real    14m16.287s
    user    0m0.000s
    sys     0m0.000s
    ---------------------
    [MultiThread with BVH, on Ubuntu, my laptop]
    Using move constructor
    bunny_list.size() = 4968

    real    8m34.897s
    user    33m35.018s
    sys     0m0.519s
    --------------------
    [MultiThread without BVH, on Ubuntu, my laptop]
    Using move constructor
    bunny_list.size() = 4968

    real    24m28.826s
    user    96m47.536s
    sys     0m0.628s
    --------------------
    [MultiThread without BVH, on Ubuntu, my laptop]
    [bunnyLow 100 images]
    real	82m4.230s
    user	271m59.557s
    sys	    0m35.908s
    --------------------
    [MultiThread with BVH, on Ubuntu, my laptop]
    [bunnyLow 100 images]
    real    42m16.707s
    user    162m54.638s
    sys     0m3.479s
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