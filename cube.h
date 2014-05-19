#ifndef CUBE_H
#define CUBE_H

#include <primitive.h>

class Cube : public Primitive
{
public:
    Cube(std::string name, int id, int tesselation, float3 color);
    void draw();
};

#endif // CUBE_H
