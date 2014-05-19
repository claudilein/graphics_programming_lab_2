#include "grid.h"
#include <iostream>

Grid::Grid(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{
    int size = 20;
    for (int i = -size; i < size; i++) {
        vertexPositions_.push_back(float3(i, 0, -size));
        vertexPositions_.push_back(float3(i, 0, +size));
    }
    for (int i = -size; i < size; i++) {
        vertexPositions_.push_back(float3(-size, 0, i));
        vertexPositions_.push_back(float3(size, 0, i));
    }

    // set indices list
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
        vertexNormals_.push_back(float3(0,1,0));
    }

    float gridColor[3] = {0.72, 0.72, 0.72};
    for (int i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(gridColor));
    }

}



void Grid::draw() {

    bindVAOToShader();

    glDrawElements(
        GL_LINES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );


}
