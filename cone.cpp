#include "cone.h"
#include <math.h>
#include <iostream>

Cone::Cone(std::string name, int id, int tesselation, float3 color,
           float radius, float height) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    height_(height)
{

    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;
    hasVBO_[TEXCOORDS] = true;

    int steps = 6 + tesselation_ * tesselation_;
    for (int i = 0; i < steps; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps);

        // cap
        float3 position0 = float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left));
        float3 position1 = float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right));
        float3 position2 = float3(0, 0, 0);

        vertexPositions_.push_back(position0);
        vertexPositions_.push_back(position1);
        vertexPositions_.push_back(position2);

        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));

        float3 texCoord0 = float3(position0.x_ / 2 + 0.5, position0.z_ / 2 + 0.5, 0.0f);
        float3 texCoord1 = float3(position1.x_ / 2 + 0.5, position1.z_ / 2 + 0.5, 0.0f);
        float3 texCoord2 = float3(position2.x_ / 2 + 0.5, position2.z_ / 2 + 0.5, 0.0f);

        vertexTextureCoordinates_.push_back(texCoord0);
        vertexTextureCoordinates_.push_back(texCoord1);
        vertexTextureCoordinates_.push_back(texCoord2);



        // body

        position0 = float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left));
        position1 = float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right));
        position2 = float3(0, height_, 0);

        vertexPositions_.push_back(position0);
        vertexPositions_.push_back(position1);
        vertexPositions_.push_back(position2);



        float degree = atan(height_ / radius_);
        float3 normal0 = float3(cos(degree) * 0 + (1 - cos(degree)) * radius_ * sin(phi_left),
                                cos(degree) * 1 + (1 - cos(degree)) * 0,
                                cos(degree) * 0 + (1 - cos(degree)) * radius_ * cos(phi_left));
        vertexNormals_.push_back(normal0);

        float3 normal1 = float3(cos(degree) * 0 + (1 - cos(degree)) * radius_ * sin(phi_right),
                                cos(degree) * 1 + (1 - cos(degree)) * 0,
                                cos(degree) * 0 + (1 - cos(degree)) * radius_ * cos(phi_right));
        vertexNormals_.push_back(normal1);

        float3 normal2 = float3(cos(degree) * 0 + (1 - cos(degree)) * (radius_ * sin(phi_left) + radius_ * sin(phi_right)) / 2,
                                cos(degree) * 1 + (1 - cos(degree)) * 0,
                                cos(degree) * 0 + (1 - cos(degree)) * (radius_ * cos(phi_left) + radius_ * cos(phi_right)) / 2);

        //normal2 = float3(0, 1, 0);
        vertexNormals_.push_back(normal2);


        vertexTextureCoordinates_.push_back(float3(i / (float) steps, 0, 0));
        vertexTextureCoordinates_.push_back(float3((i + 1) / (float) steps, 0, 0));
        vertexTextureCoordinates_.push_back(float3(((i + 0.5) / (float) steps) , 1, 0));


    }


    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float coneColor[3] = {1, 1, 0};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(coneColor));
    }


    generateTangents(3);
}


void Cone::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_TRIANGLES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
