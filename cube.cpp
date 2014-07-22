#include "cube.h"
#include <iostream>

Cube::Cube(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{

    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;
    hasVBO_[TEXCOORDS] = true;

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

    // right 2-6-5-1?
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position5));

    // back 3-2-6-7?
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position7));
    vertexPositions_.push_back(float3(position6));

    // left
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position7));

    // bottom 3-0-1-2?
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position0));

    // top 7-4-5-6?
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position7));


    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    // set cube texture coordinates. Each quad will feature the same texture.
    float3 lowerLeftCorner = float3(0,0,0);
    float3 lowerRightCorner = float3(1,0,0);
    float3 upperRightCorner = float3(1,1,0);
    float3 upperLeftCorner = float3(0,1,0);

    for (int i = 0; i < 6; i++) {
        vertexTextureCoordinates_.push_back(lowerLeftCorner);
        vertexTextureCoordinates_.push_back(lowerRightCorner);
        vertexTextureCoordinates_.push_back(upperRightCorner);
        vertexTextureCoordinates_.push_back(upperLeftCorner);
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


    // compute tangents and bitangents
    float3 deltaPos1, deltaPos2, deltaUV1, deltaUV2, tangent, bitangent;
    float r;

    for (int i = 0; i < vertexPositions_.size(); i += 3) {
        deltaPos1 = vertexPositions_[i + 1] - vertexPositions_[i];
        deltaPos2 = vertexPositions_[i + 2] - vertexPositions_[i];
        deltaUV1 = vertexTextureCoordinates_[i + 1] - vertexTextureCoordinates_[i];
        deltaUV2 = vertexTextureCoordinates_[i + 2] - vertexTextureCoordinates_[i];

        r = 1.0f / (deltaUV1.x_ * deltaUV2.y_ - deltaUV1.y_ * deltaUV2.x_);
        tangent = (deltaPos1 * deltaUV2.y_ - deltaPos2 * deltaUV1.y_) * r;
        bitangent = (deltaPos2 * deltaUV1.x_ - deltaPos1 * deltaUV2.x_) * r;

        // make tangent base orthogonal
        //tangent = normalize(tangent - n * glm::dot(n, t));

        vertexTangents_.push_back(tangent);
        vertexTangents_.push_back(tangent);
        vertexTangents_.push_back(tangent);

        vertexBitangents_.push_back(bitangent);
        vertexBitangents_.push_back(bitangent);
        vertexBitangents_.push_back(bitangent);
    }


    float cubeColor[3] = {1, 0, 0};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(cubeColor));
    }

    generateTangents(4);

}



void Cube::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);


    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );


}
