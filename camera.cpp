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

void Camera::translate(QVector3D translation) {
    //QVector3D newTranslation = rotation_.conjugate().rotatedVector(translation);
    pointOfInterest_ += translation;
}

void Camera::zoom(float zoom)
{
    zoom_ += zoom;
    emit zoomChanged();
}

void Camera::reset()
{
    rotation_ = initialRotation_;
    zoom_ = -40;
    pointOfInterest_ = QVector3D();
    emit zoomChanged();
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
        matrix.translate(0,0,-5);
    }
    matrix.rotate(rotation_);
    matrix.translate(pointOfInterest_);
    return matrix;
}

QMatrix4x4 Camera::getRotatedOnSpotCameraMatrix()
{
    QMatrix4x4 matrix;
    if (mode_ == PERSPECTIVE) matrix.translate(0, 0, zoom_);
    else if (mode_ == ORTHOGRAPHIC) {
        matrix.translate(0,0,-5);
    }
    matrix.translate(pointOfInterest_);
    matrix.rotate(rotation_);

    return matrix;
}

QQuaternion Camera::getRotation()
{
    return rotation_;
}

QVector3D Camera::getTranslation()
{
    return QVector3D(pointOfInterest_.x(), pointOfInterest_.y(), 0);
}

float Camera::getZoom() {
    return zoom_;
}

