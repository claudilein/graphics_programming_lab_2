#ifndef GRID_H
#define GRID_H

#include <primitive.h>

class Grid : public Primitive
{
public:
    Grid(float3 color, int gridSize, int stepSize);
    void draw();
    void setGridSize(int i);
    void setStepSize(int i);
private:
    int gridSize_;
    int stepSize_;
};

#endif // GRID_H
