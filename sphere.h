#ifndef SPHERE_H
#define SPHERE_H

#include <primitive.h>

class Sphere : public Primitive
{
public:
    Sphere(std::string name, int id, int tesselation, float3 color,
          float radius, int lats, int longs);
    void draw(bufferIDs buffIDs);
    void generateTangents(int verticesPerPrimitive, int startIndex = 0);

private:
    float radius_;
    int lats_;
    int longs_;
};

#endif // SPHERE_H
