#include "model.h"
#include <cube.h>
#include <sphere.h>
#include <cone.h>
#include <cylinder.h>
#include <torus.h>
#include <iostream>
#include <sstream>


Model::Model(QObject *parent) :
    QObject(parent)
{
    idCounters_ = std::vector<int> (NR_PRIMITIVES, 1);
    nrIDs_ = 1;
    tesselation_ = 0;
    scenegraphModel_ = new Scenegraph(this);


    cameras_[PERSPECTIVE] = new Camera(0, Camera::PERSPECTIVE, false, QVector3D(2,2,3), QVector3D(0,1,0));
    cameras_[FRONT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,0,3), QVector3D(0,1,0));
    cameras_[LEFT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(-3,0,0), QVector3D(0,1,0));
    cameras_[TOP] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,3,0), QVector3D(0,0,-1));

    //connect(scenegraphModel_, SIGNAL()

    active_ = PERSPECTIVE;
}

Camera* Model::getCamera(ViewportType type) {
    return cameras_[type];

}

bool Model::isActiveViewport(ViewportType type) {
    return (active_ == type);
}

void Model::setActive(ViewportType type)
{
    active_ = type;
    emit updateGL();
}


void Model::addCube()
{
    std::string name = "Cube ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CUBE];
    name = name_tmp.str();

    Primitive *p = new Cube(name, nrIDs_, tesselation_);
    idCounters_[CUBE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
}

void Model::addSphere()
{
    std::string name = "Sphere ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[SPHERE];
    name = name_tmp.str();

    Primitive *p = new Sphere(name, nrIDs_, tesselation_, 1, 10, 10);
    idCounters_[SPHERE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
}

void Model::addCylinder()
{
    std::string name = "Cylinder ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CYLINDER];
    name = name_tmp.str();

    Primitive *p = new Cylinder(name, nrIDs_, tesselation_, 1, 3);
    idCounters_[CYLINDER]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
}
void Model::addCone()
{
    std::string name = "Cone ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CONE];
    name = name_tmp.str();

    Primitive *p = new Cone(name, nrIDs_, tesselation_, 1, 3);
    idCounters_[CONE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
}
void Model::addTorus()
{
    std::string name = "Torus ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[TORUS];
    name = name_tmp.str();

    Primitive *p = new Torus(name, nrIDs_, tesselation_, 0.5, 1.5, 10, 10);
    idCounters_[TORUS]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
}

void Model::setActivePrimitive(float ID)
{
    if (ID != 0) {
        for (int i = 0; i < scenegraphModel_->getPrimitiveList()->size(); i++) {
            if (scenegraphModel_->getPrimitiveList()->at(i)->getID() == ID) {
                activePrimitive_ = scenegraphModel_->getPrimitiveList()->at(i);
                break;
            }
        }

        std::cout << "new active primitive ID: " << activePrimitive_->getID() << std::endl;
    } else {
        activePrimitive_ = NULL;
        std::cout << "new active primitive ID: 0" << std::endl;
    }

    emit updateStatusBar();
    emit updateGL();
}

void Model::setTesselation(int t)
{
    tesselation_ = t;
}

Primitive* Model::getActivePrimitive()
{
    return activePrimitive_;
}

QList<Primitive*>* Model::getScenegraph()
{
    return scenegraphModel_->getPrimitiveList();
}

Scenegraph* Model::getScenegraphModel() {
    return scenegraphModel_;
}
