#include "grid.h"

Grid::Grid(std::string name, int id, int tesselation) :
    Primitive(0, name, id, tesselation)
{
    int size = 20;
    for (int i = -size; i < size; i++) {
        vertexPositions_.push_back(float3(i, 0, -size));
        vertexPositions_.push_back(float3(i, 0, +size));
    }
    for (int i = -size; i < size; i++) {
        vertexPositions_.push_back(float3(-size, 0, i));
        vertexPositions_.push_back(float3(size, 0, i));
    }

    // set indices list
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glBufferData(GL_ARRAY_BUFFER, vertexColors_.size() * 3 * sizeof(float3), &vertexColors_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);



}



void Grid::draw() {

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
        GL_LINES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );


}
