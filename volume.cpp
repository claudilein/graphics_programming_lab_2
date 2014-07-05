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

    hasVBO_[TEXCOORDS] = true;

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

    for (int i = 0; i < MAX_SCALAR_VALUE_; i++) {
        histogram_[i] = 0;
    }

}



void Volume::parseFile(QString fileName) {
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
        // up to 64 - 1 = 63 ASCII characteres can be read in this line. One is reserved for the \0 termination.
        QByteArray line = file.readLine(64);

        QList<QByteArray> numbers = line.split(' ');

        if (numbers.size() == 3) {
            /*resolution_ = float3(numbers[0].trimmed().toInt(),
                                 numbers[1].trimmed().toInt(),
                                 numbers[2].trimmed().toInt());*/
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

            aspectRatio_ = float3(numbers[0].trimmed().toFloat(),
                                 numbers[1].trimmed().toFloat(),
                                 numbers[2].trimmed().toFloat());
            /*for (int i = 0; i < 3; i++) {
                // trimming removes whitespaces or \n, \t etc.
                aspectRatio_[i] = numbers[i].trimmed().toFloat();
                cout << "number " << i << ": " << numbers[i].data() << ", " << aspectRatio_[i] << endl;
            }*/
        } else {
            cout << numbers.size() << " numbers were read. The aspectRatio should be defined by exactly 3 numbers." << endl;
        }

        uint sizeOfTexture = resolution_[0] * resolution_[1] * resolution_[2];

        cout << "before allocating floatData" << endl;
        floatData_ = (uchar*) malloc(sizeOfTexture * sizeof(uchar));
        cout << "after allocating floatData" << endl;

        // read 16 bits for tooth texture
        if (resolution_[2] == 161) {
            short scalarValue = 0;
            char currentByte;
            for (uint i = 0; i < sizeOfTexture && !file.atEnd(); i++) {
                // copy first byte into short
                if (!file.getChar(&currentByte)) cout << "getChar() failed at pos " << i << endl;
                scalarValue = (uchar) currentByte;
                // bit shift by 1 byte
                scalarValue = scalarValue << 8;

                // copy second byte into short
                if (!file.getChar(&currentByte)) cout << "getChar() failed at pos " << i << endl;
                scalarValue += (uchar) currentByte;

                uchar temp = (uchar) ((float) scalarValue / 65536.0f * 256.0f);
                //uchar temp = (uchar) scalarValue;
//                std::cout << "tmp: " << (uint) temp << std::endl;
                // copy into float array
                floatData_[i] =  temp; // == 2^16
                histogram_[temp]++;
            }
        // read 8 bits for other textures
        } else {
            char scalarValue;
            uchar temp;
            for (uint i = 0; i < sizeOfTexture && !file.atEnd(); i++) {
                if (!file.getChar(&scalarValue)) cout << "getChar() failed at pos " << i << endl;
                temp = (uchar) scalarValue;
                floatData_[i] = temp;
                int valueInt = temp;
                histogram_[temp]++;
            }
        }



        cout << "upload of file " << fileName.toStdString() << " done" << endl;
        file.close();

        // normalize histogram
        int maxHist = 0;
        uint maxData = 0;
        for (int i = 0; i < MAX_SCALAR_VALUE_; i++) {
            if (floatData_[i] > maxData) maxData = (uint) floatData_[i];
            if (histogram_[i] > maxHist) maxHist = histogram_[i];
        }
        std::cout << "max: " << (uint) maxData<< std::endl;

        for (int i = 0; i < MAX_SCALAR_VALUE_; i++) {
            histogram_[i] = histogram_[i] * 256 / maxHist ;
            std::cout << "histogram[" << i << "]: " << histogram_[i] << std::endl;
        }



        // normalize dimensions
        int width = resolution_[0] * aspectRatio_.x_;
        int height = resolution_[1] * aspectRatio_.y_;
        int depth = resolution_[2] * aspectRatio_.z_;

        int maxDimension = max(max(width, height), depth);

        normalizedAspectRatio_.x_ = width / (float) maxDimension;
        normalizedAspectRatio_.y_ = height / (float) maxDimension;
        normalizedAspectRatio_.z_ = depth / (float) maxDimension;

        createCuboid(normalizedAspectRatio_.x_,
                     normalizedAspectRatio_.y_,
                     normalizedAspectRatio_.z_);

        
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


void Volume::copyVAOToCurrentContext(bufferIDs buffIDs) {

    Primitive::copyVAOToCurrentContext(buffIDs);

    std::cout << "before entering data into texture" << std::endl;

    glGenTextures(1, &volumeTexture_);
    glBindTexture(GL_TEXTURE_3D, volumeTexture_);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    //glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, resolution_[0], resolution_[1], resolution_[2], 0, GL_RED, GL_FLOAT, floatData_);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, resolution_[0], resolution_[1], resolution_[2], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, floatData_);
    glBindTexture(GL_TEXTURE_3D, 0);

    std::cout << "after entering data into texture" << std::endl;

    glGenTextures(1, &transferTexture_);
    glBindTexture(GL_TEXTURE_1D, transferTexture_);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_1D, 0);

    resetTransferFunction(changeTransferScalar());

}

void Volume::bindVAOToShader(bufferIDs buffIDs) {

    Primitive::bindVAOToShader(buffIDs);

    // bind volume and transfer textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, volumeTexture_);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D, transferTexture_);
    glActiveTexture(GL_TEXTURE0);


}



void Volume::draw(bufferIDs buffIDs) {


    bindVAOToShader(buffIDs);


    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

}

void Volume::setTransferFunction(int index, changeTransferScalar change, transferScalar scalar) {
    if (index < MAX_SCALAR_VALUE_) {
        std::cout << "setting red: " << change.r_ << ", blue: " << change.b_ << ", green: " << change.g_ << ", alpha: " << change.a_ << std::endl;
        if (change.r_) transferFunction_[index].r_ = scalar.r_;
        if (change.g_) transferFunction_[index].g_ = scalar.g_;
        if (change.b_) transferFunction_[index].b_ = scalar.b_;
        if (change.a_) transferFunction_[index].a_ = scalar.a_;

        uploadTransferFunction();

    } else {
        cout << "setTransferFunction: index " << index << " exceeds MAX_SCALAR_VALUE of transfer function" << endl;
    }
}

void Volume::setTransferFunction(transferScalar *function) {

    for (int i = 0; i < MAX_SCALAR_VALUE_; i++) {
        transferFunction_[i] = function[i];
    }

    uploadTransferFunction();
}

void Volume::resetTransferFunction(changeTransferScalar change) {
    if (change.r_) {
        for (int i = 0; i < MAX_SCALAR_VALUE_; i++)   transferFunction_[i].r_ = i;
    }
    if (change.g_) {
        for (int i = 0; i < MAX_SCALAR_VALUE_; i++)   transferFunction_[i].g_ = i;
    }
    if (change.b_) {
        for (int i = 0; i < MAX_SCALAR_VALUE_; i++)   transferFunction_[i].b_ = i;
    }
    if (change.a_) {
        for (int i = 0; i < MAX_SCALAR_VALUE_; i++)   transferFunction_[i].a_ = i;
    }

    uploadTransferFunction();
}

void Volume::uploadTransferFunction() {
    uchar* temp = (uchar*) malloc(sizeof(uchar) * MAX_SCALAR_VALUE_ * 4);
    for (int i = 0; i < MAX_SCALAR_VALUE_; i++) {
        temp[i * 4 + 0] = (uchar) transferFunction_[i].r_;
        temp[i * 4 + 1] = (uchar) transferFunction_[i].g_;
        temp[i * 4 + 2] = (uchar) transferFunction_[i].b_;
        temp[i * 4 + 3] = (uchar) transferFunction_[i].a_;
    }


    glBindTexture(GL_TEXTURE_1D, transferTexture_);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, MAX_SCALAR_VALUE_, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp);
    glBindTexture(GL_TEXTURE_1D, 0);


    free(temp);
}

Primitive::float3 Volume::getAspectRatio() {
    return normalizedAspectRatio_;
}

Volume::transferScalar* Volume::getTransferFunction() {
    return transferFunction_;
}

int* Volume::getHistogram() {
    return histogram_;
}

int Volume::getMaxResolution() {
    return MAX_SCALAR_VALUE_;
}

Volume::~Volume() {
    free(floatData_);
}
