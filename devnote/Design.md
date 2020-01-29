# Design notes for the ray marcher

## Mesh Operation as Mesh

```cpp
class Mesh
{
public:
    // defines a distance function (SDF)
    distance() { ... }
}

class MeshOperation : Mesh
{
private:
    Mesh* mesh0, mesh1;
public:
    // defines a distance function using the distance function 
    // of mesh0 and mesh1
    distance() {}
}
```