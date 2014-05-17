#include "sphere.h"
#include <math.h>

Sphere::Sphere(std::string name, int id, int tesselation,
               float radius, int lats, int longs) :
    Primitive(0, name, id, tesselation),
    radius_(radius),
    lats_(lats),
    longs_(longs)
{
    for (int i = 0; i < lats_-1; i++)
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
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glBufferData(GL_ARRAY_BUFFER, vertexNormals_.size() * 3 * sizeof(float3), &vertexNormals_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glBufferData(GL_ARRAY_BUFFER, vertexColors_.size() * 3 * sizeof(float3), &vertexColors_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);



}



void Sphere::draw() {
    // 1rst attribute buffer : vertices
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
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glVertexAttribPointer(
        2,                  // attribute 2
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}
