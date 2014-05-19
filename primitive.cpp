#include "primitive.h"
#include <iostream>
#include <QMatrix4x4>


Primitive::Primitive(QObject *parent, std::string name, int id, int tesselation) :
    QObject(parent),
    name_(name),
    id_(id),
    tesselation_(tesselation)
{
    glGenVertexArrays(1, &vertexArray_);
    glBindVertexArray(vertexArray_);

    glGenBuffers(1, &vertexBufferPositions_);
    glGenBuffers(1, &vertexBufferNormals_);
    glGenBuffers(1, &vertexBufferColors_);

    glGenBuffers(1, &indexBuffer_);

    translation_ = QVector3D();
    rotation_ = QQuaternion();

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



void Primitive::draw() {

}

void Primitive::translate(QVector3D translation)
{
    translation_ += translation;
}

void Primitive::rotate(QQuaternion rotation)
{
    rotation_ = rotation * rotation_;
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
