#include "model.h"

Model::Model(QObject *parent) :
    QObject(parent)
{
    perspectiveCamera = new Camera(0, Camera::PERSPECTIVE, false, QVector3D(2,2,3), QVector3D(0,1,0));
    frontCamera = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,0,3), QVector3D(0,1,0));
    leftCamera = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(-3,0,0), QVector3D(0,1,0));
    topCamera = new Camera(0, Camera::ORTHOGRAPHIC, true, QVector3D(0,3,0), QVector3D(0,0,-1));

}

Camera* Model::getCamera(ViewportType type) {
    switch (type) {
    case PERSPECTIVE: return perspectiveCamera;
    case FRONT: return frontCamera;
    case LEFT: return leftCamera;
    case TOP: return topCamera;
    default: return perspectiveCamera;
    }

}
