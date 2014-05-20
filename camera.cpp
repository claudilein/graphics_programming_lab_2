#include "camera.h"
#include <iostream>

Camera::Camera(QObject *parent, ProjectionMode mode, bool lockRotation, QQuaternion rotation) :
    QObject(parent), mode_(mode), lockRotation_(lockRotation), initialRotation_(rotation)
{
    reset();
}

void Camera::rotate(QQuaternion rotation)
{
    if (!lockRotation_)
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
    rotation_ = initialRotation_;
    zoom_ = -3;
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
    if (mode_ == PERSPECTIVE) matrix.translate(0, 0, zoom_);
    else if (mode_ == ORTHOGRAPHIC) {
        matrix.translate(0,0,-3);
        emit zoomChanged(zoom_);
    }
    matrix.rotate(rotation_);
    matrix.translate(pointOfInterest_);
    return matrix;
}


QQuaternion Camera::getRotation()
{
    return rotation_;
}


