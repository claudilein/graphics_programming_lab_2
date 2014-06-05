#include "volume.h"
#include <iostream>
#include <QFile>
#include <QIODevice>
#include <QDataStream>
#include <QByteArray>

using namespace std;

Volume::Volume(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{
    isVolume_ = true;

    glGenTextures(1, &volumeTexture_);
    glBindTexture(GL_TEXTURE_3D, volumeTexture_);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_3D, 0);

    // set texture coordinates for the cuboid

    float position0[3] = {0, 0, 1};
    float position1[3] = {1, 0, 1};
    float position2[3] = {1, 0, 0};
    float position3[3] = {0, 0, 0};
    float position4[3] = {0, 1, 1};
    float position5[3] = {1, 1, 1};
    float position6[3] = {1, 1, 0};
    float position7[3] = {0, 1, 0};

    // front
    vertexTextureCoordinates_.push_back(float3(position0));
    vertexTextureCoordinates_.push_back(float3(position1));
    vertexTextureCoordinates_.push_back(float3(position5));
    vertexTextureCoordinates_.push_back(float3(position4));

    // right
    vertexTextureCoordinates_.push_back(float3(position2));
    vertexTextureCoordinates_.push_back(float3(position6));
    vertexTextureCoordinates_.push_back(float3(position5));
    vertexTextureCoordinates_.push_back(float3(position1));

    // back
    vertexTextureCoordinates_.push_back(float3(position3));
    vertexTextureCoordinates_.push_back(float3(position2));
    vertexTextureCoordinates_.push_back(float3(position6));
    vertexTextureCoordinates_.push_back(float3(position7));

    // left
    vertexTextureCoordinates_.push_back(float3(position3));
    vertexTextureCoordinates_.push_back(float3(position0));
    vertexTextureCoordinates_.push_back(float3(position4));
    vertexTextureCoordinates_.push_back(float3(position7));

    // bottom
    vertexTextureCoordinates_.push_back(float3(position3));
    vertexTextureCoordinates_.push_back(float3(position0));
    vertexTextureCoordinates_.push_back(float3(position1));
    vertexTextureCoordinates_.push_back(float3(position2));

    // top
    vertexTextureCoordinates_.push_back(float3(position7));
    vertexTextureCoordinates_.push_back(float3(position4));
    vertexTextureCoordinates_.push_back(float3(position5));
    vertexTextureCoordinates_.push_back(float3(position6));


}

Volume::~Volume() {
    delete[] data_;
}

void Volume::parseFile(QString fileName) {
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
        // up to 64 - 1 = 63 ASCII characteres can be read in this line. One is reserved for the \0 termination.
        QByteArray line = file.readLine(64);
        cout << "size: " << line.size() << endl;

        QList<QByteArray> numbers = line.split(' ');

        if (numbers.size() == 3) {
            for (int i = 0; i < 3; i++) {
                // trimming removes whitespaces or \n, \t etc.
                resolution_[i] = numbers[i].trimmed().toInt();
                cout << "number " << i << ": " << numbers[i].data() << ", " << resolution_[i] << endl;
            }
        } else {
            cout << numbers.size() << " numbers were read. The resolution should be defined by exactly 3 numbers." << endl;
        }

        line = file.readLine(64);
        numbers = line.split(' ');

        if (numbers.size() == 3) {
            for (int i = 0; i < 3; i++) {
                // trimming removes whitespaces or \n, \t etc.
                aspectRatio_[i] = numbers[i].trimmed().toFloat();
                cout << "number " << i << ": " << numbers[i].data() << ", " << aspectRatio_[i] << endl;
            }
        } else {
            cout << numbers.size() << " numbers were read. The aspectRatio should be defined by exactly 3 numbers." << endl;
        }

        uint sizeOfTexture = resolution_[0] * resolution_[1] * resolution_[2];
        cout << "allocating data array: " << sizeOfTexture << endl;
        data_ = new uchar[sizeOfTexture];

        char scalarValue;
        for (uint i = 0; i < sizeOfTexture && !file.atEnd(); i++) {
            if (file.getChar(&scalarValue)) data_[i] = (uchar) scalarValue;
            else cout << "file.getChar() failed on character " << i << endl;
        }

        cout << "upload of file " << fileName.toStdString() << " done" << endl;
        file.close();

//        uchar transferredData[sizeOfTexture * 4];
//        transfer(data_, &transferredData[0], sizeOfTexture);

        cout << "before allocating testData" << endl;
        float *testData = (float*) malloc(sizeOfTexture * sizeof(float));


        cout << "after allocating testData" << endl;
        for (uint i = 0; i < sizeOfTexture; i++) {
            testData[i] = (uint) data_[i] / 256.0f;
//            cout << "testData[" << i << "]: " << testData[i] << endl;
        }
        cout << "before texture binding" << endl;

        glBindTexture(GL_TEXTURE_3D, volumeTexture_);
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, resolution_[0], resolution_[1], resolution_[2], 0, GL_RED, GL_UNSIGNED_INT, data_);
        cout << "before data transfer" << endl;
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE8, resolution_[0], resolution_[1], resolution_[2], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data_);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, resolution_[0], resolution_[1], resolution_[2], 0, GL_RED, GL_FLOAT, testData);
        cout << "before texture unbinding" << endl;
        glBindTexture(GL_TEXTURE_3D, 0);

        cout << "entered data into texture" << endl;

        // normalize dimensions
        int width = resolution_[0] * aspectRatio_[0];
        int height = resolution_[1] * aspectRatio_[1];
        int depth = resolution_[2] * aspectRatio_[2];

        int maxDimension = max(max(width, height), depth);
        normalizedAspectRatio_[0] = width / (float) maxDimension;
        normalizedAspectRatio_[1] = height / (float) maxDimension;
        normalizedAspectRatio_[2] = depth / (float) maxDimension;

        createCuboid(normalizedAspectRatio_[0],
                     normalizedAspectRatio_[1],
                     normalizedAspectRatio_[2]);

    } else {
        std::cout << "File " << fileName.toStdString() << " could not be opened." << std::endl;
    }
}

void Volume::createCuboid(float width, float height, float depth) {
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float halfDepth = depth / 2;

    float position0[3] = {-halfWidth, -halfHeight, +halfDepth};
    float position1[3] = {+halfWidth, -halfHeight, +halfDepth};
    float position2[3] = {+halfWidth, -halfHeight, -halfDepth};
    float position3[3] = {-halfWidth, -halfHeight, -halfDepth};
    float position4[3] = {-halfWidth, +halfHeight, +halfDepth};
    float position5[3] = {+halfWidth, +halfHeight, +halfDepth};
    float position6[3] = {+halfWidth, +halfHeight, -halfDepth};
    float position7[3] = {-halfWidth, +halfHeight, -halfDepth};

    // front
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position4));

    // right
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position1));

    // back
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position2));
    vertexPositions_.push_back(float3(position6));
    vertexPositions_.push_back(float3(position7));

    // left
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position7));

    // bottom
    vertexPositions_.push_back(float3(position3));
    vertexPositions_.push_back(float3(position0));
    vertexPositions_.push_back(float3(position1));
    vertexPositions_.push_back(float3(position2));

    // top
    vertexPositions_.push_back(float3(position7));
    vertexPositions_.push_back(float3(position4));
    vertexPositions_.push_back(float3(position5));
    vertexPositions_.push_back(float3(position6));

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }
}


void Volume::copyVAOToCurrentContext() {

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoordinates_);
    glBufferData(GL_ARRAY_BUFFER, vertexTextureCoordinates_.size() *  sizeof(float3), &vertexTextureCoordinates_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);

}

void Volume::bindVAOToShader() {

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
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoordinates_);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

    // bind texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, volumeTexture_);
    glActiveTexture(GL_TEXTURE0);

}



void Volume::draw() {


    bindVAOToShader();


    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}

void Volume::transfer(uchar *data, uchar *transferredData, uint size) {
    for (uint i = 0; i < size; i++) {
        transferredData[i * 4 + 0] = data[i];
        transferredData[i * 4 + 1] = data[i];
        transferredData[i * 4 + 2] = data[i];
        transferredData[i * 4 + 3] = data[i];
    }
}
