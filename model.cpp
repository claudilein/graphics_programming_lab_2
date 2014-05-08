#include "model.h"

Model::Model(QObject *parent) :
    QObject(parent)
{
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
