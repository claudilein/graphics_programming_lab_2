#include "primitive.h"
#include <iostream>
#include <QMatrix4x4>


Primitive::Primitive(QObject *parent, std::string name, int id, int tesselation) :
    QObject(parent),
    name_(name),
    id_(id),
    tesselation_(tesselation)
{
    std::cout << "gen vertex array" << std::endl;
    glGenVertexArrays(1, &vertexArray_);
    std::cout << "binding vertex array" << std::endl;
    glBindVertexArray(vertexArray_);

    std::cout << "gen positions" << std::endl;
    glGenBuffers(1, &vertexBufferPositions_);
    glGenBuffers(1, &vertexBufferNormals_);
    glGenBuffers(1, &vertexBufferColors_);

    glGenBuffers(1, &indexBuffer_);
    std::cout << "created all buffers " << std::endl;

}


void Primitive::draw() {

}

void Primitive::translate(QVector3D translation)
{
    translation_ += translation;
}

void Primitive::rotate(QQuaternion rotation)
{
    rotation_ = rotation_ * rotation;
}

QMatrix4x4 Primitive::getModelMatrix()
{
    QMatrix4x4 modelMatrix;
    modelMatrix.rotate(rotation_);
    modelMatrix.translate(translation_);
    return modelMatrix;
}
