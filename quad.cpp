#include "quad.h"
#include <iostream>


Quad::Quad() :
  Primitive(0, "Quad", 0, 0, float3(0,0,0))
{
    glGenBuffers(1, &vertexBufferTextureCoordinates_);

    // set vertex positions for a simple quad

    vertexPositions_.push_back(float3(1,1,0));
    vertexPositions_.push_back(float3(-1,1,0));
    vertexPositions_.push_back(float3(-1,-1,0));
    vertexPositions_.push_back(float3(1,-1,0));

    // set texture coordinates for the 4 vertices

    vertexTextureCoordinates_.push_back(float3(1,1,0));
    vertexTextureCoordinates_.push_back(float3(0,1,0));
    vertexTextureCoordinates_.push_back(float3(0,0,0));
    vertexTextureCoordinates_.push_back(float3(1,0,0));

    // push indices into list

    indicesList_.push_back(0);
    indicesList_.push_back(1);
    indicesList_.push_back(2);
    indicesList_.push_back(3);
}

void Quad::copyVAOToCurrentContext()
{


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoordinates_);
    glBufferData(GL_ARRAY_BUFFER, vertexTextureCoordinates_.size() *  sizeof(float3), &vertexTextureCoordinates_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);

}

void Quad::bindVAOToShader() {

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoordinates_);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glDisableVertexAttribArray(2);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

}


void Quad::draw() {

    bindVAOToShader();

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
