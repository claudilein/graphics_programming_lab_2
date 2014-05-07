#include "camera.h"

Camera::Camera(QObject *parent, ProjectionMode mode, bool lockRotation) :
    QObject(parent)
{
    mode_ = mode;
    lockRotation_ = lockRotation;
    reset();
}

void Camera::rotate(QQuaternion rotation)
{
    rotation_ = rotation_ * rotation;
}

void Camera::translate(QVector3D translation)
{
    translation_ = translation_ + translation;
}

void Camera::setPointOfInterest(QVector3D pointOfInterest)
{
    pointOfInterest_ = pointOfInterest;
}


void Camera::setProjectionMode(ProjectionMode mode)
{
    mode_ = mode;
}


void Camera::reset()
{
    rotation_ = QQuaternion();
    translation_ = QVector3D(0,0,-3);
    pointOfInterest_ = QVector3D();
}


Camera::ProjectionMode Camera::getProjectionMode()
{
    return mode_;
}

QMatrix4x4 Camera::getCameraMatrix()
{
    QMatrix4x4 matrix;
    matrix.rotate(rotation_);
    matrix.translate(translation_);
    return matrix;
}


QVector3D Camera::getPosition()
{
    return position_;
}

QVector3D Camera::getPointOfInterest()
{
    return pointOfInterest_;
}
