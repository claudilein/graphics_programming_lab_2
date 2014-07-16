#include "primitive.h"
#include <iostream>
#include <QMatrix4x4>


Primitive::Primitive(QObject *parent, std::string name, int id, int tesselation, float3 color) :
    QObject(parent),
    name_(name),
    id_(id),
    tesselation_(tesselation),
    color_(color),
    isVolume_(false),
    isTerrain_(false)
{
    for (int i = 0; i < NR_VBOS; i++) {
        hasVBO_[i] = false;
    }

    for (int i = 0; i < NR_TEXTURES; i++) {
        textures[i] = QImage();
    }

    ambientColor_ = float3(0, 0, 0);
    diffuseColor_ = float3(0, 0, 0);
    specularColor_ = float3(0, 0, 0);
    roughness_ = 1;
    refractionIndex_ = 1;


    translation_ = QVector3D();
    rotation_ = QQuaternion();
    scalingFactor_ = QVector3D(1,1,1);


}

void Primitive::copyVAOToCurrentContext(bufferIDs buffIDs) {

    // every primitive has a VAO, positions and indices

    glBindVertexArray(buffIDs.VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, buffIDs.positions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIDs.indices_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);

    // bind normals, colors and texCoords if needed

    if (buffIDs.hasNormals_) {
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.normals_);
        glBufferData(GL_ARRAY_BUFFER, vertexNormals_.size() * sizeof(float3), &vertexNormals_[0], GL_STATIC_DRAW);
    }
    if (buffIDs.hasColors_) {
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.colors_);
        glBufferData(GL_ARRAY_BUFFER, vertexColors_.size()  * sizeof(float3), &vertexColors_[0], GL_STATIC_DRAW);
    }
    if (buffIDs.hasTexCoords_) {
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.texCoords_);
        glBufferData(GL_ARRAY_BUFFER, vertexTextureCoordinates_.size() *  sizeof(float3), &vertexTextureCoordinates_[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Primitive::copyTextureToCurrentContext(GLuint textureID, Textures x) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[x].width(), textures[x].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[x].bits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Primitive::bindVAOToShader(bufferIDs buffIDs) {

    glBindVertexArray(buffIDs.VAO_);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffIDs.positions_);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIDs.indices_);


    if (buffIDs.hasNormals_) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.normals_);
        glVertexAttribPointer(
            1,                  // attribute 1
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    if (buffIDs.hasColors_) {
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.colors_);
        glVertexAttribPointer(
            2,                  // attribute 2
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    if (buffIDs.hasTexCoords_) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.texCoords_);
        glVertexAttribPointer(
            1,                  // attribute 2
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    for (int i = 0; i < NR_TEXTURES; i++) {
        if (buffIDs.hasTextures_[i]) {
            glActiveTexture(GL_TEXTURE10 + i);
            glBindTexture(GL_TEXTURE_2D, buffIDs.textures_[i]);
            std::cout << "texture " << i << " bound" << std::endl;
        }
    }
    glActiveTexture(GL_TEXTURE0);

}

void Primitive::draw(bufferIDs buffIDs) {
    std::cout << "The super draw method of Primitive was called, but should never be called" << std::endl;
}

bool Primitive::hasVBO(VBO vbo) {
    return hasVBO_[vbo];
}

Primitive::float3* Primitive::getColor() {
    return &color_;
}

void Primitive::translate(QVector3D translation)
{
    translation_ += translation;
    rbtNode_.translation = rbtNode_.translation;
}

void Primitive::rotate(QQuaternion rotation)
{
    rotation_ = rotation * rotation_;
    rbtNode_.rotation = rbtNode_.rotation * rotation;
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

    //modelMatrix.translate(rbtNode_.translation);
    //modelMatrix.rotate(rbtNode_.rotation);
    modelMatrix.scale(scalingFactor_);

    return modelMatrix;
}


int Primitive::getID()
{
    return id_;
}

bool Primitive::isVolume() {
    return isVolume_;
}


bool Primitive::isTerrain() {
    return isTerrain_;
}

std::string Primitive::getName()
{
    return name_;
}

void Primitive::setAmbientColor(float3 color) {
    ambientColor_ = color;
    std::cout << "ambient color changed to " << ambientColor_.x_  << ", " << ambientColor_.y_ << ", " << ambientColor_.z_ << std::endl;
}

void Primitive::setDiffuseColor(float3 color) {
    diffuseColor_ = color;
    std::cout << "diffuse color changed to " << diffuseColor_.x_  << ", " << diffuseColor_.y_ << ", " << diffuseColor_.z_ << std::endl;
}

void Primitive::setSpecularColor(float3 color) {
    specularColor_ = color;
    std::cout << "specular color changed to " << specularColor_.x_  << ", " << specularColor_.y_ << ", " << specularColor_.z_ << std::endl;
}

void Primitive::setRoughness(float roughness) {
    roughness_ = roughness;
    std::cout << "roughness set to " << roughness_ << std::endl;
}

void Primitive::setRefractionIndex(float refractionIndex) {
    refractionIndex_ = refractionIndex;
    std::cout << "refractionIndex set to " << refractionIndex_ << std::endl;
}

void Primitive::setTexture(Textures x, QImage texture) {
    textures[x] = texture;
    // upload to OpenGL context
    std::cout << "Texture " << x << "reset" << std::endl;
}

void Primitive::setTextureActive(Textures x, bool status) {
    texturesActive[x] = status;
    std::cout << "Texture " << x << "set to " << status << std::endl;
}
