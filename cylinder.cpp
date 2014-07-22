#include "cylinder.h"
#include <math.h>
#include <iostream>

Cylinder::Cylinder(std::string name, int id, int tesselation, float3 color,
                   float radius, float height) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    height_(height),
    steps_(6 + tesselation * tesselation)

{
    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;
    hasVBO_[TEXCOORDS] = true;

    // bottom cap
    for (int i = 0; i < steps_; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps_);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps_);

        float3 position0 = float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left));
        float3 position1 = float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right));
        float3 position2 = float3(0, 0, 0);

        vertexPositions_.push_back(position0);
        vertexPositions_.push_back(position1);
        vertexPositions_.push_back(position2);

        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));

        vertexTextureCoordinates_.push_back(float3(position0.x_ / 2 + 0.5, position0.z_ / 2 + 0.5, 0.0f));
        vertexTextureCoordinates_.push_back(float3(position1.x_ / 2 + 0.5, position1.z_ / 2 + 0.5, 0.0f));
        vertexTextureCoordinates_.push_back(float3(position2.x_ / 2 + 0.5, position2.z_ / 2 + 0.5, 0.0f));
    }

    // top cap
    for (int i = 0; i < steps_; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps_);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps_);


        float3 position0 = float3(radius_ * sin(phi_left), height_, radius_ * cos(phi_left));
        float3 position1 = float3(radius_ * sin(phi_right), height_, radius_ * cos(phi_right));
        float3 position2 = float3(0, height_, 0);

        vertexPositions_.push_back(position0);
        vertexPositions_.push_back(position1);
        vertexPositions_.push_back(position2);


        vertexNormals_.push_back(float3(0, 1, 0));
        vertexNormals_.push_back(float3(0, 1, 0));
        vertexNormals_.push_back(float3(0, 1, 0));

        vertexTextureCoordinates_.push_back(float3(position0.x_ / 2 + 0.5, position0.z_ / 2 + 0.5, 0.0f));
        vertexTextureCoordinates_.push_back(float3(position1.x_ / 2 + 0.5, position1.z_ / 2 + 0.5, 0.0f));
        vertexTextureCoordinates_.push_back(float3(position2.x_ / 2 + 0.5, position2.z_ / 2 + 0.5, 0.0f));

    }

    generateTangents(3);


    // body
    // steps_ * 3 * 2 punkte
    for (int i = 0; i < steps_; i++) {

        float3 position0 = vertexPositions_[i * 3];
        float3 position1 = vertexPositions_[i * 3 + 1];
        float3 position2 = vertexPositions_[3 * steps_ + i * 3 + 1];
        float3 position3 = vertexPositions_[3 * steps_ + i * 3];

        vertexPositions_.push_back(position0);
        vertexPositions_.push_back(position1);
        vertexPositions_.push_back(position2);
        vertexPositions_.push_back(position3);


        float3 normal0 = float3(vertexPositions_[i * 3].x_, 0, vertexPositions_[i * 3].z_);
        float3 normal1 = float3(vertexPositions_[i * 3 + 1].x_, 0, vertexPositions_[i * 3 + 1].z_);
        float3 normal2 = float3(vertexPositions_[3 * steps_ + i * 3 + 1].x_, 0, vertexPositions_[3 * steps_ + i * 3 + 1].z_);
        float3 normal3 = float3(vertexPositions_[3 * steps_ + i * 3 ].x_, 0, vertexPositions_[3 * steps_ + i * 3 ].z_);

        vertexNormals_.push_back(normal0);
        vertexNormals_.push_back(normal1);
        vertexNormals_.push_back(normal2);
        vertexNormals_.push_back(normal3);

        vertexTextureCoordinates_.push_back(float3(i / (float) steps_, 0, 0));
        vertexTextureCoordinates_.push_back(float3((i + 1) / (float) steps_, 0, 0));
        vertexTextureCoordinates_.push_back(float3((i + 1) / (float) steps_, 1, 0));
        vertexTextureCoordinates_.push_back(float3(i / (float) steps_, 1, 0));

/*
        float v = (atan2(normal0.z_, normal0.x_) / 2 * M_PI + 1.0f) * 0.5f;
        vertexTextureCoordinates_.push_back(float3(v, position0.y_ / height_ , 0));

        v = (atan2(normal1.z_, normal1.x_) / 2 * M_PI + 1.0f) * 0.5f;
        vertexTextureCoordinates_.push_back(float3(v, position1.y_ / height_ , 0));

        v = (atan2(normal2.z_, normal2.x_) / 2 * M_PI + 1.0f) * 0.5f;
        vertexTextureCoordinates_.push_back(float3(v, position2.y_ / height_, 0));

        v = (atan2(normal3.z_, normal3.x_) / 2 * M_PI + 1.0f) * 0.5f;
        vertexTextureCoordinates_.push_back(float3(v, position3.y_ / height_, 0));
*/

    }



    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float cylinderColor[3] = {0, 0, 1};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(cylinderColor));
    }

    generateTangents(4, steps_ * 6);

}



void Cylinder::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_TRIANGLES,      // mode
        steps_ * 6,    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
    glDrawElements(
        GL_QUADS,      // mode
        steps_ * 4,    // count
        GL_UNSIGNED_INT,       // type
        (void*) (steps_ * 6 * sizeof(uint))           // element array buffer offset
    );



}
