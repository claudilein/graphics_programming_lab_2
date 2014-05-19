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
    interactionMode_ = Model::CAMERA;


    cameras_[PERSPECTIVE] = new Camera(0, Camera::PERSPECTIVE, false, QQuaternion::fromAxisAndAngle(QVector3D(1, -1, 0), 45.0f));
    cameras_[FRONT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QQuaternion());
    cameras_[LEFT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 90.0f));
    cameras_[TOP] = new Camera(0, Camera::ORTHOGRAPHIC, true, QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 90.0f));

    connect(scenegraphModel_, SIGNAL(itemNameChanged()), this, SIGNAL(updateStatusBar()));


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
    emit updateGL();
}


void Model::addCube()
{
    std::string name = "Cube ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CUBE];
    name = name_tmp.str();

    Primitive *p = new Cube(name, nrIDs_, tesselation_, Primitive::float3(1, 0, 0));
    idCounters_[CUBE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
    emit updateGL();

    setActivePrimitive(p->getID());
}

void Model::addSphere()
{
    std::string name = "Sphere ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[SPHERE];
    name = name_tmp.str();

    Primitive *p = new Sphere(name, nrIDs_, tesselation_, Primitive::float3(0, 1, 0), 1, 10, 10);
    idCounters_[SPHERE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
    emit updateGL();
    setActivePrimitive(p->getID());
}

void Model::addCylinder()
{
    std::string name = "Cylinder ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CYLINDER];
    name = name_tmp.str();

    Primitive *p = new Cylinder(name, nrIDs_, tesselation_, Primitive::float3(0, 0, 1), 1, 3);
    idCounters_[CYLINDER]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
    emit updateGL();
    setActivePrimitive(p->getID());
}
void Model::addCone()
{
    std::string name = "Cone ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[CONE];
    name = name_tmp.str();

    Primitive *p = new Cone(name, nrIDs_, tesselation_, Primitive::float3(1, 1, 0), 1, 3);
    idCounters_[CONE]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
    emit updateGL();
    setActivePrimitive(p->getID());
}
void Model::addTorus()
{
    std::string name = "Torus ";
    std::stringstream name_tmp;
    name_tmp << name << idCounters_[TORUS];
    name = name_tmp.str();

    Primitive *p = new Torus(name, nrIDs_, tesselation_, Primitive::float3(1, 0, 1), 0.3, 0.7, 10, 10);
    idCounters_[TORUS]++;
    nrIDs_++;

    scenegraphModel_->addPrimitive(p);

    emit copyVAOData(p);
    emit updateGL();
    emit updateGL();
    setActivePrimitive(p->getID());
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
    } else {
        activePrimitive_ = NULL;
    }

    QModelIndex index = scenegraphModel_->index(ID - 1, 0, QModelIndex());

    emit selectItem(index);
    emit updateStatusBar();
    emit updateGL();

}

void Model::setTesselation(int t)
{
    tesselation_ = t;
}

void Model::resetCamera() {
    cameras_[active_]->reset();
    emit updateGL();
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


void Model::setObjectInteractionMode() {
    interactionMode_ = Model::OBJECT;
    std::cout << "InteractionMode: OBJECT" << std::endl;
}

void Model::setCameraInteractionMode() {
    interactionMode_ = Model::CAMERA;
    std::cout << "InteractionMode: CAMERA" << std::endl;
}

Model::InteractionType Model::getInteractionMode() {
    return interactionMode_;
}


void Model::modelItemClicked(QModelIndex index) {
    setActivePrimitive(scenegraphModel_->getPrimitive(index)->getID());
}

void Model::renamePrimitive(QString name) {
    activePrimitive_->setName(name.toStdString());
    emit updateStatusBar();
}

void Model::deleteActivePrimitive() {
    if (activePrimitive_ != NULL) {
        scenegraphModel_->deletePrimitive(activePrimitive_);
        setActivePrimitive(0);
        emit updateStatusBar();
        emit updateGL();
    }
}
