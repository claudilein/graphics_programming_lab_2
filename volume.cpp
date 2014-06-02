#include "volume.h"
#include <iostream>
#include <QFile>
#include <QIODevice>
#include <QDataStream>

using namespace std;

Volume::Volume(std::string name, int id, int tesselation, float3 color) :
    Primitive(0, name, id, tesselation, color)
{
}

void Volume::parseFile(QString fileName) {
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);    // read the data serialized from the file
        //while (!file.atEnd()) {
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            qint8 x;
            in >> x;
            sum += (int) x;
            cout << x << endl;
        }
        cout << "sum: " << sum << endl;
    } else {
        std::cout << "File " << fileName.toStdString() << " could not be opened." << std::endl;
    }
}
