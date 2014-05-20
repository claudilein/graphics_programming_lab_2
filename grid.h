#ifndef GRID_H
#define GRID_H

#include <primitive.h>

class Grid : public Primitive
{
public:
    Grid(std::string name, int id, int tesselation, float3 color);
    void draw();
    void setGridSize(int i);
    void setStepSize(int i);
private:
    int gridSize_;
    int stepSize_;
};

#endif // GRID_H
