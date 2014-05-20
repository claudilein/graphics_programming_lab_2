#include "cone.h"
#include <math.h>
#include <iostream>

Cone::Cone(std::string name, int id, int tesselation, float3 color,
           float radius, float height) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    height_(height)
{

    int steps = 4 + tesselation_ * tesselation_;
    for (int i = 0; i < steps; i++) {
        double phi_left = 2 * M_PI * i / static_cast<double>(steps);
        double phi_right = 2 * M_PI * (i + 1) / static_cast<double>(steps);

        vertexPositions_.push_back(float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left)));
        vertexPositions_.push_back(float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right)));
        vertexPositions_.push_back(float3(0,0,0));

        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));
        vertexNormals_.push_back(float3(0, -1, 0));


        vertexPositions_.push_back(float3(radius_ * sin(phi_left), 0, radius_ * cos(phi_left)));
        vertexPositions_.push_back(float3(radius_ * sin(phi_right), 0, radius_ * cos(phi_right)));
        vertexPositions_.push_back(float3(0, height_, 0));



        float degree = atan(height_ / radius_);
        vertexNormals_.push_back(float3(cos(degree) * 0 + (1 - cos(degree)) * radius_ * sin(phi_left),
                                        cos(degree) * 1 + (1 - cos(degree)) * 0,
                                        cos(degree) * 0 + (1 - cos(degree)) * radius_ * cos(phi_left)));
        vertexNormals_.push_back(float3(cos(degree) * 0 + (1 - cos(degree)) * radius_ * sin(phi_right),
                                        cos(degree) * 1 + (1 - cos(degree)) * 0,
                                        cos(degree) * 0 + (1 - cos(degree)) * radius_ * cos(phi_right)));
        vertexNormals_.push_back(float3(cos(degree) * 0 + (1 - cos(degree)) * (radius_ * sin(phi_left) + radius_ * sin(phi_right)) / 2,
                                        cos(degree) * 1 + (1 - cos(degree)) * 0,
                                        cos(degree) * 0 + (1 - cos(degree)) * (radius_ * cos(phi_left) + radius_ * cos(phi_right)) / 2));
    }


    // set indices list
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float coneColor[3] = {1, 1, 0};
    for (int i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(coneColor));
    }

    ambientColor_[0] = 0.2;
    ambientColor_[1] = 0.2;

}


void Cone::draw() {

    bindVAOToShader();    


    glDrawElements(
        GL_TRIANGLES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
