#include "primitive.h"
#include <iostream>
#include <QMatrix4x4>
#include <QGLWidget>

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
        texturesActive[i] = false;
    }

    ambientColor_ = float3(0, 0, 0);
    diffuseColor_ = float3(0, 0, 0);
    specularColor_ = float3(0, 0, 0);
    roughness_ = 0.50f;
    refractionIndex_ = 2.5f;
    ka_ = 1; kd_ = 1; ks_ = 1;
    diffuseShader_ = 0;
    specularShader_ = 0;


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
    if (buffIDs.hasTangents_) {
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.tangents_);
        glBufferData(GL_ARRAY_BUFFER, vertexTangents_.size() *  sizeof(float3), &vertexTangents_[0], GL_STATIC_DRAW);
    }
    if (buffIDs.hasBitangents_) {
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.bitangents_);
        glBufferData(GL_ARRAY_BUFFER, vertexBitangents_.size() *  sizeof(float3), &vertexBitangents_[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Primitive::generateTangents(int verticesPerPrimitive, int startIndex) {
    hasVBO_[TANGENTS] = true;
    hasVBO_[BITANGENTS] = true;

    // compute tangents and bitangents
    float3 deltaPos1, deltaPos2, deltaUV1, deltaUV2, tangent, bitangent;
    float r;

    for (uint i = startIndex; i < vertexPositions_.size(); i += verticesPerPrimitive) {

        deltaPos1 = vertexPositions_[i + 1] - vertexPositions_[i];
        deltaPos2 = vertexPositions_[i + 2] - vertexPositions_[i];
        deltaUV1 = vertexTextureCoordinates_[i + 1] - vertexTextureCoordinates_[i];
        deltaUV2 = vertexTextureCoordinates_[i + 2] - vertexTextureCoordinates_[i];


       /*std::cout << "deltaPos1: " << deltaPos1.x_ << ", " << deltaPos1.y_ << ", " << deltaPos1.z_
                  << ", deltaPos2: " << deltaPos2.x_ << ", " << deltaPos2.y_ << ", " << deltaPos2.z_
                  << ", deltaUV1: " << deltaUV1.x_ << ", " << deltaUV1.y_ << ", " << deltaUV1.z_
                  << ", deltaUV2: " << deltaUV2.x_ << ", " << deltaUV2.y_ << ", " << deltaUV2.z_
                  << std::endl;
        */

        /*if (deltaPos1.x_ < 0.0001) deltaPos1.x_ = 0;
        if (deltaPos1.y_ < 0.0001) deltaPos1.y_ = 0;
        if (deltaPos1.z_ < 0.0001) deltaPos1.z_ = 0;
        if (deltaPos2.x_ < 0.0001) deltaPos2.x_ = 0;
        if (deltaPos2.y_ < 0.0001) deltaPos2.y_ = 0;
        if (deltaPos2.z_ < 0.0001) deltaPos2.z_ = 0;
        if (deltaUV1.x_ < 0.0001) deltaUV1.x_ = 0;
        if (deltaUV1.y_ < 0.0001) deltaUV1.y_ = 0;
        if (deltaUV1.z_ < 0.0001) deltaUV1.z_ = 0;
        if (deltaUV2.x_ < 0.0001) deltaUV2.x_ = 0;
        if (deltaUV2.y_ < 0.0001) deltaUV2.y_ = 0;
        if (deltaUV2.z_ < 0.0001) deltaUV2.z_ = 0;*/

        if (deltaPos1.x_ < 0.0001 && deltaPos1.x_ > 0.0) std::cout << "vertex " << i << ": deltaPos1.x:" << std::endl;
        if (deltaPos1.y_ < 0.0001 && deltaPos1.y_ > 0.0) std::cout << "vertex " << i << ": deltaPos1.y:" << std::endl;
        if (deltaPos1.z_ < 0.0001 && deltaPos1.z_ > 0.0) std::cout << "vertex " << i << ": deltaPos1.z:" << std::endl;
        if (deltaPos2.x_ < 0.0001 && deltaPos2.x_ > 0.0) std::cout << "vertex " << i << ": deltaPos2.x:" << std::endl;
        if (deltaPos2.y_ < 0.0001 && deltaPos2.y_ > 0.0) std::cout << "vertex " << i << ": deltaPos2.y:" << std::endl;
        if (deltaPos2.z_ < 0.0001 && deltaPos2.z_ > 0.0)  std::cout << "vertex " << i << ": deltaPos2.z:" << std::endl;
        if (deltaUV1.x_ < 0.0001 && deltaUV1.x_ > 0.0) std::cout << "vertex " << i << ": deltaUV1.x:" << std::endl;
        if (deltaUV1.y_ < 0.0001 && deltaUV1.y_ > 0.0) std::cout << "vertex " << i << ": deltaUV1.y:" << std::endl;
        if (deltaUV1.z_ < 0.0001 && deltaUV1.z_ > 0.0) std::cout << "vertex " << i << ": deltaUV1.z:" << std::endl;
        if (deltaUV2.x_ < 0.0001 && deltaUV2.x_ > 0.0) std::cout << "vertex " << i << ": deltaUV2.x:" << std::endl;
        if (deltaUV2.y_ < 0.0001 && deltaUV2.y_ > 0.0) std::cout << "vertex " << i << ": deltaUV2.y:" << std::endl;
        if (deltaUV2.z_ < 0.0001 && deltaUV2.z_ > 0.0) std::cout << "vertex " << i << ": deltaUV2.z:" << std::endl;


        r = 1.0f / (deltaUV1.x_ * deltaUV2.y_ - deltaUV1.y_ * deltaUV2.x_);
        tangent = (deltaPos1 * deltaUV2.y_ - deltaPos2 * deltaUV1.y_) * r;
        bitangent = (deltaPos2 * deltaUV1.x_ - deltaPos1 * deltaUV2.x_) * r;

        /*
        float3 normal = float3();
        for (int j = 0; j < verticesPerPrimitive; j++) {
            normal = normal + vertexNormals_[i + j];
        }
        normal = float3::normalize(normal);


        // make tangent base orthogonal
        tangent = float3::normalize(tangent - normal * float3::dot(normal, tangent));
        bitangent = float3::normalize(bitangent - normal * float3::dot(normal, bitangent));
*/

        for (int j = 0; j < verticesPerPrimitive; j++) {
            float3 normal = vertexNormals_[i + j];
            float3 t = float3::normalize(tangent - normal * float3::dot(normal, tangent));
            if (float3::dot(normal, t) > 0.0001) {
                std::cout << "NdotT(" << i + j << "): " << float3::dot(normal, t) << std::endl;
            }
            vertexTangents_.push_back(t);
            float3 b = float3::normalize((bitangent - normal * float3::dot(normal, bitangent) - tangent * float3::dot(tangent, bitangent)));
            if (float3::dot(t, b) > 0.0001) {
                std::cout << "TdotB(" << i + j << "): " << float3::dot(t, b) << std::endl;
            }
            vertexBitangents_.push_back(b);
        }
/*
        for (int j = 0; j < verticesPerPrimitive; j++) {
            vertexTangents_.push_back(tangent);
            vertexBitangents_.push_back(bitangent);
        }
        */
    }
}




void Primitive::copyTextureToCurrentContext(GLuint textureID, Textures x) {
    QImage transformedTexture = QGLWidget::convertToGLFormat(textures[x]);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, transformedTexture.width(), transformedTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, transformedTexture.bits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (x != NORMALS) glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "texture " << x << " uploaded to OpenGL context" << std::endl;
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
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.texCoords_);
        glVertexAttribPointer(
            3,                  // attribute 2
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    if (buffIDs.hasTangents_) {
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.tangents_);
        glVertexAttribPointer(
            4,                  // attribute 2
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    if (buffIDs.hasBitangents_) {
        glEnableVertexAttribArray(5);
        glBindBuffer(GL_ARRAY_BUFFER, buffIDs.bitangents_);
        glVertexAttribPointer(
            5,                  // attribute 2
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }



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
    float m = std::max(0.2f, (50.0f * (1.0f - roughness) * (1.0f - roughness)));
    std::cout << "phong roughness: " << m << std::endl;


}

void Primitive::setRefractionIndex(float refractionIndex) {
    refractionIndex_ = refractionIndex;
    std::cout << "refractionIndex set to " << refractionIndex_ << std::endl;
}

void Primitive::setTexture(Textures x, QImage texture) {
    textures[x] = texture;
    // upload to OpenGL context
    std::cout << "Texture " << x << " set" << std::endl;
}

void Primitive::setTextureActive(Textures x, bool status) {
    texturesActive[x] = status;
    std::cout << "TextureActive " << x << " set to " << status << std::endl;
}

void Primitive::setDiffuseShader(int i) {
    diffuseShader_ = i;
    std::cout << "Diffuse Shader set to " << i << std::endl;

}

void Primitive::setSpecularShader(int i) {
    specularShader_ = i;
    std::cout << "Specular Shader set to " << i << std::endl;
}

void Primitive::setKa(float ka) {
    ka_ = ka;
    std::cout << "ka set to " << ka << std::endl;
}

void Primitive::setKd(float kd) {
    kd_ = kd;
    std::cout << "kd set to " << kd << std::endl;
}

void Primitive::setKs(float ks) {
    ks_ = ks;
    std::cout << "ks set to " << ks << std::endl;
}




Primitive::float3 Primitive::getAmbientColor() {
    return ambientColor_;
}

Primitive::float3 Primitive::getDiffuseColor() {
    return diffuseColor_;
}

Primitive::float3 Primitive::getSpecularColor() {
    return specularColor_;
}

float Primitive::getRoughness() {
    return roughness_;
}

float Primitive::getRefractionIndex() {
    return refractionIndex_;
}

int Primitive::getDiffuseShader() {
    return diffuseShader_;
}

int Primitive::getSpecularShader() {
    return specularShader_;
}

bool Primitive::isTextureActive(Textures x) {
    return texturesActive[x];
}

QImage* Primitive::getTexture(Textures x) {
    return &textures[x];
}

float Primitive::getKa() {
    return ka_;
}

float Primitive::getKd() {
    return kd_;
}

float Primitive::getKs() {
    return ks_;
}



void Primitive::debugTangentSpace(bool normals, bool tangents, bool bitangents) {

    glBegin(GL_LINES);

    if (normals) {
        glColor3f(0,0,1);
        for (uint i = 0; i < vertexPositions_.size(); i++){
            float3 p = vertexPositions_[i];
            glVertex3f(p.x_, p.y_, p.z_);
            float3 o = float3::normalize(vertexNormals_[i]);
            p = p + o * 0.2f;
            glVertex3f(p.x_, p.y_, p.z_);
        }
    }

    if (tangents) {
        glColor3f(1,0,0);
        for (uint i = 0; i < vertexPositions_.size(); i++){
            float3 p = vertexPositions_[i];
            glVertex3f(p.x_, p.y_, p.z_);

            float3 o = float3::normalize(vertexTangents_[i]);
            p = p + o * 0.2f;
            glVertex3f(p.x_, p.y_, p.z_);
        }
    }

    if (bitangents) {
        glColor3f(0,1,0);
        for (uint i = 0; i < vertexPositions_.size(); i++){
            float3 p = vertexPositions_[i];
            glVertex3f(p.x_, p.y_, p.z_);
            float3 o = float3::normalize(vertexBitangents_[i]);
            p = p + o * 0.2f;
            glVertex3f(p.x_, p.y_, p.z_);
        }
    }

    glEnd();
}
