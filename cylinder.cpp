#include "cylinder.h"
#include <math.h>

Cylinder::Cylinder(std::string name, int id, int tesselation, float3 color,
                   float radius, float height) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    height_(height),
    steps_(4 + tesselation * tesselation)

{
    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;

    for (int i = 0; i < steps_; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps_);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps_);

        vertexPositions_.push_back(float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left)));
        vertexPositions_.push_back(float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right)));
        vertexPositions_.push_back(float3(0,0,0));

        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));
    }

    for (int i = 0; i < steps_; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps_);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps_);

        vertexPositions_.push_back(float3(radius_ * sin(phi_left), height_, radius_ * cos(phi_left)));
        vertexPositions_.push_back(float3(radius_ * sin(phi_right), height_, radius_ * cos(phi_right)));
        vertexPositions_.push_back(float3(0,0,0));

        vertexNormals_.push_back(float3(0, 1, 0));
        vertexNormals_.push_back(float3(0, 1, 0));
        vertexNormals_.push_back(float3(0, 1, 0));
    }

    // steps_ * 3 * 2 punkte
    for (int i = 0; i < steps_; i++) {

        vertexPositions_.push_back(vertexPositions_[i * 3]);
        vertexPositions_.push_back(vertexPositions_[i * 3 + 1]);
        vertexPositions_.push_back(vertexPositions_[3 * steps_ + i * 3 + 1]);
        vertexPositions_.push_back(vertexPositions_[3 * steps_ + i * 3]);

        vertexNormals_.push_back(float3(vertexPositions_[i * 3].x_, 0, vertexPositions_[i * 3].z_));
        vertexNormals_.push_back(float3(vertexPositions_[i * 3 + 1].x_, 0, vertexPositions_[i * 3 + 1].z_));
        vertexNormals_.push_back(float3(vertexPositions_[3 * steps_ + i * 3 + 1].x_, 0, vertexPositions_[3 * steps_ + i * 3 + 1].z_));
        vertexNormals_.push_back(float3(vertexPositions_[3 * steps_ + i * 3 ].x_, 0, vertexPositions_[3 * steps_ + i * 3 ].z_));
    }



    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float cylinderColor[3] = {0, 0, 1};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(cylinderColor));
    }


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
