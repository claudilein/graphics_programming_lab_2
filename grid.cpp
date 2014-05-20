#include "grid.h"
#include <iostream>

Grid::Grid(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{
    gridSize_ = 10;
    stepSize_ = 4;

    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            vertexPositions_.push_back(float3(i + (j / (float)stepSize_), 0, -gridSize_));
            vertexPositions_.push_back(float3(i + (j / (float)stepSize_), 0, +gridSize_));
        }
    }
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            vertexPositions_.push_back(float3(-gridSize_, 0, i + (j / (float)stepSize_)));
            vertexPositions_.push_back(float3(gridSize_, 0, i + (j / (float)stepSize_)));
        }
    }

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
        vertexNormals_.push_back(float3(0,1,0));
    }

    float gridColor[3] = {0.72, 0.72, 0.72};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(gridColor));
    }

}


void Grid::setGridSize(int i) {
    gridSize_ = i;


    attribute newPositions;
    attribute newColors;
    attribute newNormals;
    std::vector<uint> newIndices;


    std::cout << "before accl" << std::endl;
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            newPositions.push_back(float3(i + (j / (float)stepSize_), 0, -gridSize_));
            newPositions.push_back(float3(i + (j / (float)stepSize_), 0, +gridSize_));
        }
    }
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            newPositions.push_back(float3(-gridSize_, 0, i + (j / (float)stepSize_)));
            newPositions.push_back(float3(gridSize_, 0, i + (j / (float)stepSize_)));
        }
    }

    // set indices list
    for (uint i = 0; i < newPositions.size(); i++) {
        newIndices.push_back(i);
        newNormals.push_back(float3(0,1,0));
    }

    float gridColor[3] = {0.72, 0.72, 0.72};
    for (uint i = 0; i < newPositions.size(); i++) {
        newColors.push_back(float3(gridColor));
    }

//    vertexPositions_ = newPositions;
//    vertexNormals_ = newNormals;
//    vertexColors_ = newColors;
//    indicesList_ = newIndices;

    std::cout << "before swap" << std::endl;
    newPositions.swap(vertexPositions_);
    newNormals.swap(vertexNormals_);
    newColors.swap(vertexColors_);
    newIndices.swap(indicesList_);
    std::cout << "after swap" << std::endl;

//    vertexPositions_ = newPositions;
//    vertexNormals_ = newNormals;
//    vertexColors_ = std::vector<float3>(newColors);
//    indicesList_ = std::vector<uint>(newIndices);

    copyVAOToCurrentContext();

    std::cout << "after VAO" << std::endl;
}

void Grid::setStepSize(int i) {
    std::cout << "setting step size" << std::endl;
    stepSize_ = i;

    attribute newPositions;
    attribute newColors;
    attribute newNormals;
    std::vector<uint> newIndices;

    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            newPositions.push_back(float3(i + (j / (float)stepSize_), 0, -gridSize_));
            newPositions.push_back(float3(i + (j / (float)stepSize_), 0, +gridSize_));
        }
    }
    for (int i = -gridSize_; i < gridSize_; i++) {
        for (int j = 0; j < stepSize_; j++) {
            newPositions.push_back(float3(-gridSize_, 0, i + (j / (float)stepSize_)));
            newPositions.push_back(float3(gridSize_, 0, i + (j / (float)stepSize_)));
        }
    }

    // set indices list
    for (uint i = 0; i < newPositions.size(); i++) {
        newIndices.push_back(i);
        newNormals.push_back(float3(0,1,0));
    }

    float gridColor[3] = {0.72, 0.72, 0.72};
    for (uint i = 0; i < newPositions.size(); i++) {
        newColors.push_back(float3(gridColor));
    }


    vertexPositions_ = newPositions;
    vertexNormals_ = newNormals;
    vertexColors_ = newColors;
    indicesList_ = newIndices;

    copyVAOToCurrentContext();
}


void Grid::draw() {

    bindVAOToShader();


    std::cout << "drawing grid" << std::endl;
    glDrawElements(
        GL_LINES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );


}
