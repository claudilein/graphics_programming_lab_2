#include "primitive.h"
#include <iostream>
#include <QMatrix4x4>


Primitive::Primitive(QObject *parent, std::string name, int id, int tesselation, float3 color) :
    QObject(parent),
    name_(name),
    id_(id),
    tesselation_(tesselation),
    color_(color)
{
    glGenVertexArrays(1, &vertexArray_);
    glBindVertexArray(vertexArray_);

    glGenBuffers(1, &vertexBufferPositions_);
    glGenBuffers(1, &vertexBufferNormals_);
    glGenBuffers(1, &vertexBufferColors_);

    glGenBuffers(1, &indexBuffer_);

    translation_ = QVector3D();
    rotation_ = QQuaternion();
    scalingFactor_ = QVector3D(1,1,1);

}

void Primitive::copyVAOToCurrentContext() {

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glBufferData(GL_ARRAY_BUFFER, vertexNormals_.size() * sizeof(float3), &vertexNormals_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glBufferData(GL_ARRAY_BUFFER, vertexColors_.size()  * sizeof(float3), &vertexColors_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);

}

void Primitive::bindVAOToShader() {


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );


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
}

void Primitive::draw() {

}

Primitive::float3* Primitive::getColor() {
    return &color_;
}

void Primitive::translate(QVector3D translation)
{
    translation_ += translation;
}

void Primitive::rotate(QQuaternion rotation)
{
    rotation_ = rotation * rotation_;
}

void Primitive::scale(QVector3D scalingFactor)
{
    scalingFactor_ += scalingFactor;
    if (scalingFactor_.x() <= 0.1) scalingFactor_.setX(0.1);
    if (scalingFactor_.y() <= 0.1) scalingFactor_.setY(0.1);
    if (scalingFactor_.z() <= 0.1) scalingFactor_.setZ(0.1);
}

void Primitive::setName(std::string name)
{
    name_ = name;
}

QMatrix4x4 Primitive::getModelMatrix()
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(translation_);
    modelMatrix.rotate(rotation_);
    modelMatrix.scale(scalingFactor_);
    return modelMatrix;
}


int Primitive::getID()
{
    return id_;
}

std::string Primitive::getName()
{
    return name_;
}
