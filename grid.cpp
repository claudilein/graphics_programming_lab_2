#include "grid.h"
#include <iostream>

Grid::Grid(float3 color, int gridSize, int stepSize) :
    Primitive(0, "Grid", 0, 0, color),
    gridSize_(gridSize),
    stepSize_(stepSize)
{
    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;

    createGrid();

}


void Grid::setGridSize(int i) {
    gridSize_ = i;
    createGrid();
}

void Grid::setStepSize(int i) {
    stepSize_ = i;
    createGrid();
}


void Grid::createGrid() {

    vertexPositions_.resize(0, float3(0,0,0));
    vertexColors_.resize(0, float3(0,0,0));
    vertexNormals_.resize(0, float3(0,0,0));
    indicesList_.resize(0, 0);

    // draw vertical lines (in z-direction)
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j <= stepSize_; j++) {
            vertexPositions_.push_back(float3(i + (j / (float)stepSize_), 0, -gridSize_));
            vertexPositions_.push_back(float3(i + (j / (float)stepSize_), 0, +gridSize_));
        }
    }

    // draw horizontal lines (in x-direction)
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j <= stepSize_; j++) {
            vertexPositions_.push_back(float3(-gridSize_, 0, i + (j / (float)stepSize_)));
            vertexPositions_.push_back(float3(gridSize_, 0, i + (j / (float)stepSize_)));
        }
    }

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
        vertexNormals_.push_back(float3(0,1,0));
    }

    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(color_));
    }
}

void Grid::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_LINES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );


}
