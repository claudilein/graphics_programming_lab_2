#ifndef TORUS_H
#define TORUS_H

#include <primitive.h>

class Torus : public Primitive
{
public:
    Torus(std::string name, int id, int tesselation, float3 color,
          float innerRadius, float outerRadius, int sides, int rings);
    void draw(bufferIDs buffIDs);
    //void generateTangents(int verticesPerPrimitive, int startIndex = 0);

private:
    float innerRadius_;
    float outerRadius_;
    int sides_;
    int rings_;
};

#endif // TORUS_H
