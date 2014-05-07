#include "camera.h"

Camera::Camera(QObject *parent, ProjectionMode mode, bool lockRotation, QVector3D position, QVector3D up) :
    QObject(parent)
{
    mode_ = mode;
    lockRotation_ = lockRotation;
    startPosition_ = position;
    up_ = up;
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
    position_ = startPosition_;
    rotation_ = QQuaternion();
    translation_ = QVector3D();
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

QVector3D Camera::getUpVector()
{
    return up_;
}
