#include "terrain.h"
#include <iostream>
#include <QFile>
#include <QGLWidget>
#include <QImage>

Terrain::Terrain(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color),
    horizontalScale_(256),
    verticalScale_(300),
    gridSize_(128),
    width_(0),
    height_(0)
{

    hasVBO_[TEXCOORDS] = true;
    isTerrain_ = true;

    vertexPositions_.resize(0);
    indicesList_.resize(0);
    vertexTextureCoordinates_.resize(0);
    int halfGridSize = gridSize_ / 2;
/*
    for (int i = 0; i < gridSize_; i++) {
        for (int j = 0; j > gridSize_; j++) {
            vertexPositions_.push_back(float3(i, 0, j));
            vertexPositions_.push_back(float3(i + 1, 0, j));
            vertexPositions_.push_back(float3(i + 1, 0, j - 1));
            vertexPositions_.push_back(float3(i, 0, j - 1));

            // texture coordinates range from [-0.5, 0.5] here, as they will be added to the global tex coords of the camera
            vertexTextureCoordinates_.push_back(float3((i) / (2.0f * halfGridSize), (-j) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i + 1) / (2.0f * halfGridSize), (-j) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i + 1) / (2.0f * halfGridSize), (-j + 1) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i) / (2.0f * halfGridSize), (-j + 1) / (2.0f * halfGridSize), 0));

        }
    }*/


    for (int i = -halfGridSize; i < halfGridSize; i++) {
        for (int j = halfGridSize; j > -halfGridSize; j--) {
            vertexPositions_.push_back(float3(i, 0, j));
            vertexPositions_.push_back(float3(i + 1, 0, j));
            vertexPositions_.push_back(float3(i + 1, 0, j - 1));
            vertexPositions_.push_back(float3(i, 0, j - 1));

            // texture coordinates range from [-0.5, 0.5] here, as they will be added to the global tex coords of the camera
            vertexTextureCoordinates_.push_back(float3((i) / (2.0f * halfGridSize), (-j) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i + 1) / (2.0f * halfGridSize), (-j) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i + 1) / (2.0f * halfGridSize), (-j + 1) / (2.0f * halfGridSize), 0));
            vertexTextureCoordinates_.push_back(float3((i) / (2.0f * halfGridSize), (-j + 1) / (2.0f * halfGridSize), 0));

        }
    }

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

}

void Terrain::parseHeightMap(QString fileName) {
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
        // up to 64 - 1 = 63 ASCII characteres can be read in this line. One is reserved for the \0 termination.
        QByteArray line = file.readLine(64); // do nothing, throw this line away
        line = file.readLine(64);
        width_ = line.trimmed().toInt();
        line = file.readLine(64);
        height_ = line.trimmed().toInt();
        line = file.readLine(64);
        int maxValue = line.trimmed().toInt();
        std::cout << "width: " << width_ << ", height: " << height_ << ", maxValue: " << maxValue << std::endl;

        int sizeOfTexture = width_ * height_;

        // allocate array for height values
        heightValues_ = (unsigned short*) malloc(sizeOfTexture * sizeof(unsigned short));

        unsigned short currentHeightValue = 0;
        char currentByte;
        unsigned short minHeight = 65535;
        unsigned short maxHeight = 0;
        for (uint i = 0; i < sizeOfTexture && !file.atEnd(); i++) {
            // copy first byte into short
            if (!file.getChar(&currentByte)) std::cout << "getChar() failed at pos " << i << std::endl;
            currentHeightValue = (uchar) currentByte;
            // bit shift by 1 byte
            currentHeightValue = currentHeightValue << 8;

            //std::cout << "byte 1: " << (unsigned short) currentByte;

            // copy second byte into short
            if (!file.getChar(&currentByte)) std::cout << "getChar() failed at pos " << i << std::endl;
            currentHeightValue += (uchar) currentByte;

            heightValues_[i] = currentHeightValue;
            if (heightValues_[i] > maxHeight) maxHeight = heightValues_[i];
            if (heightValues_[i] < minHeight) minHeight = heightValues_[i];

            //std::cout << ", byte 2: " << (unsigned short) currentByte;
            //std::cout << ", height value[" << i << "]: " << currentHeightValue << std::endl;

            //uchar temp = (uchar) ((float) currentHeightValue / 65536.0f * 256.0f);
            //uchar temp = (uchar) currentHeightValue;
//                std::cout << "tmp: " << (uint) temp << std::endl;
            // copy into float array
            //floatData_[i] =  temp; // == 2^16
        }

        std::cout << "minHeight: " << minHeight << ", maxHeight: " << maxHeight << std::endl;

        for (uint i = 0; i < sizeOfTexture; i++) {
            heightValues_[i] -= minHeight;
        }

    } else {
        std::cout << "Failed to open file " << fileName.toStdString() << std::endl;
    }

}

void Terrain::createTextures(GLuint id) {

    checkGLErrors("before height map upload");

//    glGenTextures(1, &heightTexture_);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, width_, height_, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, heightValues_);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::cout << "height map uploaded.." << std::endl;

    checkGLErrors("after height map upload");

    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLErrors("after uploading material textures");
}



void Terrain::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    checkGLErrors("before drawing terrain");

    glDrawElements(
        GL_PATCHES,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

    checkGLErrors("drawing terrain");

}

void Terrain::setHorizontalScale(int horizontalScale) {
    horizontalScale_ = horizontalScale;
    if (horizontalScale_ % 2 != 0) horizontalScale_--;
}

void Terrain::setVerticalScale(int verticalScale) {
    verticalScale_ = verticalScale;
}

void Terrain::uploadMaterial(QString fileName, GLuint textureID) {
    QImage material = QGLWidget::convertToGLFormat(QImage(fileName));

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, material.width(), material.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, material.bits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLErrors("material upload");
}


void Terrain::checkGLErrors(const char *label) {
    GLenum errCode;
    const GLubyte *errStr;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errStr = gluErrorString(errCode);
        printf("OpenGL ERROR: ");
        printf((char*)errStr);
        printf("(Label: ");
        printf(label);
        printf(")\n.");
    }
}

int Terrain::getHorizontalScale() {
    return horizontalScale_;
}

int Terrain::getVerticalScale() {
    return verticalScale_;
}

int Terrain::getGridSize() {
    return gridSize_;
}


unsigned short* Terrain::getHeightValues() {
    return heightValues_;
}

int Terrain::getWidth() {
    return width_;
}
