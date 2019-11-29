```cpp

// defines color ( and other things? )
struct Material { ... }

// or maybe just use this?
interface IColor
{
    virtual Color GetColor( ... ) const = 0;
}

// or... this?
// in the get color method, consider alpha, light pos and other parameters
// get the reference of it in distance function
// after obtaining normals and other things, <P>
// use the reference and <P> to obtain the color of the pixel
interface IMaterial { ... }

struct DistanceInfo
{
    float distance;
    
    Vector3 color;
    // returns color directly
    
    // size_t entityID; 
    // alt : use entityID to access a specific entity and do texture mapping or other stuff
    // cons : sampling the color or texture might require duplicated computation
    
    // IMaterial* material;
    // described in interface IMaterial
}

interface IDistance
{
    // Time parameter can help some easy animation,
    // distance functions can just take in an normalized 
    // time and scale it or add offset as needed.
    virtual DistanceInfo GetDistanceInfo ( Vector3 point, float time  ) const = 0;
}

class Entity : IDistance
{
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    
    // or maybe use a transform struct?
    // Transform transform;
}

// Blend, Mask, Lerp ... etc
class MeshOperation : IDistance
{
public:
    reference_to_Entity e0, e1;
}
```

```cpp

// Obsolete
// Since I use a scene class for encapsulating everything, I have the information
// about every entity in the scene class.
// I can easily control every entities' behavior in scene's Update function,
// so entity itself does not need to inherit the IUpdate class
//
// The major drawback for entity to inherit IUpdate is that, when I call make_shared to
// init the entity, it has not been constructed. Thus it's not possible to pass itself to
// its update function call.
//
//
// cube = std::make_shared<Cube>( ... , [&](){ cube->fun(); } )
// 
// Above code cannot work because when constructing the cube, we need its reference or instance.
// But it's not constructed yet, so it's a dead loop.

class IUpdate
{
    virtual void Update(float current_time) const = 0;
}

class Entity
{
    std::function<void (float)> update_function;
    
    void Update(float current_time) const
    {
        update_function();
    }
}

std::vector< IUpdate* > updatable;

int main()
{
    while( ... )
    {
        foreach(auto a in updatable)
        {
            a->Update(t);
        }
    }
}
```