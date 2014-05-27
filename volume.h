#ifndef VOLUME_H
#define VOLUME_H

#include <primitive.h>

class Volume : public Primitive
{
public:
    Volume(std::string name, int id, int tesselation, float3 color);
    //void draw();
};

#endif // VOLUME_H
