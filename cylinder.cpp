#include "cylinder.h"
#include <math.h>

Cylinder::Cylinder(std::string name, int id, int tesselation,
                   float radius, float height) :
    Primitive(0, name, id, tesselation),
    radius_(radius),
    height_(height),
    steps_(4 + tesselation)

{


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



void Cylinder::draw() {

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

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glVertexAttribPointer(
        2,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glVertexAttribPointer(
        3,                  // attribute 2
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
