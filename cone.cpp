#include "cone.h"
#include <math.h>

Cone::Cone(std::string name, int id, int tesselation,
           float radius, float height) :
    Primitive(0, name, id, tesselation),
    radius_(radius),
    height_(height)
{

    //int steps = 1 << tesselation;
    int steps = 10;
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


void Cone::draw() {
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
        GL_TRIANGLES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}
