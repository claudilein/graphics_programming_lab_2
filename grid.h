#ifndef GRID_H
#define GRID_H

#include <primitive.h>

class Grid : public Primitive
{
public:
    Grid(std::string name, int id, int tesselation, float3 color);
    void draw();
};

#endif // GRID_H
