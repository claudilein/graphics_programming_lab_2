#include "sphere.h"
#include <math.h>

Sphere::Sphere(std::string name, int id, int tesselation, float3 color,
               float radius, int lats, int longs) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    lats_(lats + tesselation),
    longs_(longs + tesselation)
{

    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;

    for (int i = 0; i < lats_; i++)
    {
        double theta_upper = M_PI * i / static_cast<double>(lats_);
        double theta_lower = M_PI * (i+1) / static_cast<double>(lats_);
        for (int j = 0; j < longs_; j++)
        {
            double phi_left = 2* M_PI * j / static_cast<double>(longs_);
            double phi_right = 2* M_PI * (j+1) / static_cast<double>(longs_);

            vertexPositions_.push_back(float3(radius_*sin(theta_upper)*cos(phi_left),
                                              radius_*sin(theta_upper)*sin(phi_left),
                                              radius_*cos(theta_upper)));
            vertexPositions_.push_back(float3(radius_*sin(theta_lower)*cos(phi_left),
                                              radius_*sin(theta_lower)*sin(phi_left),
                                              radius_*cos(theta_lower)));
            vertexPositions_.push_back(float3(radius_*sin(theta_lower)*cos(phi_right),
                                              radius_*sin(theta_lower)*sin(phi_right),
                                              radius_*cos(theta_lower)));
            vertexPositions_.push_back(float3(radius_*sin(theta_upper)*cos(phi_right),
                                              radius_*sin(theta_upper)*sin(phi_right),
                                              radius_*cos(theta_upper)));

            vertexNormals_.push_back(float3(radius_*sin(theta_upper)*cos(phi_left),
                                              radius_*sin(theta_upper)*sin(phi_left),
                                              radius_*cos(theta_upper)));
            vertexNormals_.push_back(float3(radius_*sin(theta_lower)*cos(phi_left),
                                              radius_*sin(theta_lower)*sin(phi_left),
                                              radius_*cos(theta_lower)));
            vertexNormals_.push_back(float3(radius_*sin(theta_lower)*cos(phi_right),
                                              radius_*sin(theta_lower)*sin(phi_right),
                                              radius_*cos(theta_lower)));
            vertexNormals_.push_back(float3(radius_*sin(theta_upper)*cos(phi_right),
                                              radius_*sin(theta_upper)*sin(phi_right),
                                              radius_*cos(theta_upper)));
        }
    }



    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float sphereColor[3] = {0, 1, 0};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(sphereColor));
    }

    ambientColor_[1] = 0.2;

}



void Sphere::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}
