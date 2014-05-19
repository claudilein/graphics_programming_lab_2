#include "cube.h"
#include <iostream>

Cube::Cube(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{

    // set cube vertices
    float x = 0.5f;

    float position0[3] = {-x, -x, +x};
    float position1[3] = {+x, -x, +x};
    float position2[3] = {+x, -x, -x};
    float position3[3] = {-x, -x, -x};
    float position4[3] = {-x, +x, +x};
    float position5[3] = {+x, +x, +x};
    float position6[3] = {+x, +x, -x};
    float position7[3] = {-x, +x, -x};

    // front
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position4));

    // right
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position1));

    // back
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position7));

    // left
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position7));

    // bottom
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position2));

    // top
    vertexPositions_.push_back(float3(position7));
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position6));


    // set indices list
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }


    // set cube vertexNormals_
    float normal0[3] = {0, 0, 1};
    float normal1[3] = {1, 0, 0};
    float normal2[3] = {0, 0, -1};
    float normal3[3] = {-1, 0, 0};
    float normal4[3] = {0, -1, 0};
    float normal5[3] = {0, 1, 0};

    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal0));
    }
    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal1));
    }
    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal2));
    }
    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal3));
    }
    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal4));
    }
    for (int i = 0; i < 4; i++) {
        vertexNormals_.push_back(float3(normal5));
    }

    float cubeColor[3] = {1, 0, 0};
    for (int i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(cubeColor));
    }




}



void Cube::draw() {

    bindVAOToShader();

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
