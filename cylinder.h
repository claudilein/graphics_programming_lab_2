#ifndef CYLINDER_H
#define CYLINDER_H

#include <primitive.h>

class Cylinder : public Primitive
{
public:
    Cylinder(std::string name, int id, int tesselation, float3 color,
             float radius, float height);
    void draw();

private:
    float radius_;
    float height_;
    int steps_;
};

#endif // CYLINDER_H
