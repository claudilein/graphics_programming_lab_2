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
    glGenTextures(1, &volumeTexture_);
    glBindTexture(GL_TEXTURE_3D, volumeTexture_);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_3D, 0);

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

        glBindTexture(GL_TEXTURE_3D, volumeTexture_);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, resolution_[0], resolution_[1], resolution_[2], 0, GL_RED, GL_UNSIGNED_INT, data_);
        glBindTexture(GL_TEXTURE_3D, 0);

        cout << "entered data into texture" << endl;

    } else {
        std::cout << "File " << fileName.toStdString() << " could not be opened." << std::endl;
    }
}

void Volume::transfer(uchar *data, uchar *transferredData, uint size) {
    for (int i = 0; i < size; i++) {
        transferredData[i * 4 + 0] = data[i];
        transferredData[i * 4 + 1] = data[i];
        transferredData[i * 4 + 2] = data[i];
        transferredData[i * 4 + 3] = data[i];
    }
}
