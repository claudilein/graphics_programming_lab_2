#ifndef TORUS_H
#define TORUS_H

#include <primitive.h>

class Torus : public Primitive
{
public:
    Torus(std::string name, int id, int tesselation, float3 color,
          float innerRadius, float outerRadius, int sides, int rings);
    void draw();

private:
    float innerRadius_;
    float outerRadius_;
    int sides_;
    int rings_;
};

#endif // TORUS_H
