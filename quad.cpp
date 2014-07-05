#include "quad.h"
#include <iostream>


Quad::Quad() :
  Primitive(0, "Quad", 0, 0, float3(0,0,0))
{

    hasVBO_[TEXCOORDS] = true;

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



void Quad::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
