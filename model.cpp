#include "model.h"
#include <cube.h>
#include <sphere.h>
#include <cone.h>
#include <cylinder.h>
#include <torus.h>
#include <iostream>

Model::Model(QObject *parent) :
    QObject(parent)
{
    idCounters_ = std::vector<int> (NR_PRIMITIVES, 0);
    nrIDs_ = 0;
    tesselation_ = 0;
    scenegraph_ = new QList<Primitive*>();
    activePrimitiveID_ = 0;

    cameras_[PERSPECTIVE] = new Camera(0, Camera::PERSPECTIVE, false, QVector3D(2,2,3), QVector3D(0,1,0));
    cameras_[FRONT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,0,3), QVector3D(0,1,0));
    cameras_[LEFT] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(-3,0,0), QVector3D(0,1,0));
    cameras_[TOP] = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,3,0), QVector3D(0,0,-1));

    active_ = PERSPECTIVE;
}

Camera* Model::getCamera(ViewportType type) {
    return cameras_[type];
//    switch (type) {
//    case PERSPECTIVE: return perspectiveCamera;
//    case FRONT: return frontCamera;
//    case LEFT: return leftCamera;
//    case TOP: return topCamera;
//    default: return perspectiveCamera;
//    }

}

bool Model::getActive(ViewportType type) {
    return (active_ == type);
//    switch (type) {
//    case PERSPECTIVE: return perspectiveCamera;
//    case FRONT: return frontCamera;
//    case LEFT: return leftCamera;
//    case TOP: return topCamera;
//    default: return perspectiveCamera;
//    }
}

void Model::setActive(ViewportType type)
{
    active_ = type;
}


void Model::addCube()
{
    Primitive *p = new Cube("Cube " + idCounters_[CUBE], nrIDs_, tesselation_);
    idCounters_[CUBE]++;
    nrIDs_++;
    scenegraph_->append(p);
    emit updateGL();
}

void Model::addSphere()
{
    Primitive *p = new Sphere("Sphere " + idCounters_[SPHERE], nrIDs_, tesselation_, 1, 10, 10);
    idCounters_[SPHERE]++;
    nrIDs_++;
    scenegraph_->append(p);
    emit updateGL();
}

void Model::addCylinder()
{
    Primitive *p = new Cylinder("Cylinder " + idCounters_[CYLINDER], nrIDs_, tesselation_, 1, 3);
    idCounters_[CYLINDER]++;
    nrIDs_++;
    scenegraph_->append(p);
    emit updateGL();
}
void Model::addCone()
{
    Primitive *p = new Cone("Cone " + idCounters_[CONE], nrIDs_, tesselation_, 1, 3);
    idCounters_[CONE]++;
    nrIDs_++;
    scenegraph_->append(p);
    emit updateGL();
}
void Model::addTorus()
{
    Primitive *p = new Torus("Torus " + idCounters_[TORUS], nrIDs_, tesselation_, 0.5, 1.5, 10, 10);
    idCounters_[TORUS]++;
    nrIDs_++;
    scenegraph_->append(p);
    emit updateGL();
}

void Model::setActivePrimitive(int ID)
{
    activePrimitiveID_ = ID;
    std::cout << "new active primitive ID: " << activePrimitiveID_ << std::endl;
}

QList<Primitive*>* Model::getScenegraph()
{
    return scenegraph_;
}
