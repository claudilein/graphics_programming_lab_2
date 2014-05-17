#ifndef SPHERE_H
#define SPHERE_H

#include <primitive.h>

class Sphere : public Primitive
{
public:
    Sphere(std::string name, int id, int tesselation,
          float radius, int lats, int longs);
    void draw();

private:
    float radius_;
    int lats_;
    int longs_;
};

#endif // SPHERE_H
