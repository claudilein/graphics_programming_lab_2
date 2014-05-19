#include "camera.h"

Camera::Camera(QObject *parent, ProjectionMode mode, bool lockRotation, QVector3D position, QVector3D up) :
    QObject(parent), mode_(mode), lockRotation_(lockRotation)
{
    reset();
}

void Camera::rotate(QQuaternion rotation)
{
    rotation_ = rotation_ * rotation;
}

void Camera::translate(QVector2D translation)
{
    QVector3D newTranslation = rotation_.conjugate().rotatedVector(QVector3D(translation, 0));
    pointOfInterest_ += newTranslation;
}

void Camera::zoom(float zoom)
{
    zoom_ += zoom;
}

void Camera::reset()
{
    rotation_ = QQuaternion();
    zoom_ = -2;
    pointOfInterest_ = QVector3D();
}



void Camera::setProjectionMode(ProjectionMode mode)
{
    mode_ = mode;
}

Camera::ProjectionMode Camera::getProjectionMode()
{
    return mode_;
}

QMatrix4x4 Camera::getCameraMatrix()
{
    QMatrix4x4 matrix;
    matrix.translate(0, 0, zoom_);
    matrix.rotate(rotation_);
    matrix.translate(pointOfInterest_);
    return matrix;
}

QQuaternion Camera::getRotation()
{
    return rotation_;
}


