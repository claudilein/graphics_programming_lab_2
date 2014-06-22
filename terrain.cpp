#include "terrain.h"

Terrain::Terrain(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color),
    horizontalScale_(25),
    verticalScale_(25)
{
    isTerrain_ = true;
    createVBO();
}



void Terrain::createTextures() {

    glGenTextures(1, &heightTexture_);
    glBindTexture(GL_TEXTURE_3D, heightTexture_);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, resolution_[0], resolution_[1], 0, GL_RED, GL_FLOAT, floatData_);
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, resolution_[0], resolution_[1], resolution_[2], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, floatData_);
    glBindTexture(GL_TEXTURE_3D, 0);

}

void Terrain::bindVAOToShader() {


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



    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);



    // bind volume and transfer textures
    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, heightTexture_);
    glActiveTexture(GL_TEXTURE0);
*/

}




void Terrain::draw() {

    bindVAOToShader();

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}

void Terrain::createVBO() {

    vertexPositions_.resize(0);
    indicesList_.resize(0);

    for (int i = -verticalScale_; i < verticalScale_; i++) {
        for (int j = -horizontalScale_; j < horizontalScale_; j++) {
            vertexPositions_.push_back(float3(i, 0, j));
            vertexPositions_.push_back(float3(i, 0, j + 1));
            vertexPositions_.push_back(float3(i + 1, 0, j + 1));
            vertexPositions_.push_back(float3(i + 1, 0, j));
        }
    }

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

}

void Terrain::setHorizontalScale(int horizontalScale) {
    horizontalScale_ = horizontalScale;
}

void Terrain::setVerticalScale(int verticalScale) {
    verticalScale_ = verticalScale;
}
