#ifndef CONE_H
#define CONE_H

#include <primitive.h>

class Cone : public Primitive
{
public:
    Cone(std::string name, int id, int tesselation, float3 color,
         float radius, float height);
    void draw();

private:
    float radius_;
    float height_;
};

#endif // CONE_H
