#include "primitive.h"
#include <iostream>


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


Primitive::attribute Primitive::getVertexPositions()
{
    return vertexPositions_;
}

Primitive::attribute Primitive::getVertexColors()
{
    return vertexColors_;
}

Primitive::attribute Primitive::getVertexNormals()
{
    return vertexNormals_;
}

void Primitive::draw() {
    std::cout << "Primitive->draw was called " << std::endl;
}

int Primitive::getID()
{
    return id_;
}
