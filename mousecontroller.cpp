#include "mousecontroller.h"
#include <viewport.h>
#include <math.h>
#include <iostream>
#include <model.h>

MouseController::MouseController(QObject *parent, Viewport *viewport, Camera *camera, Model *model) :
    QObject(parent)
{
    viewport_ = viewport;
    camera_ = camera;
    model_ = model;
    trackballRadius_ = 1.0f;

    connect(viewport_, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(viewport_, SIGNAL(mouseMoveEvent(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(viewport_, SIGNAL(wheelEvent(QWheelEvent*)), this, SLOT(wheelEvent(QWheelEvent*)));
    connect(viewport_, SIGNAL(setActivePrimitive(float)), this, SIGNAL(setActivePrimitive(float)));

    connect(this, SIGNAL(setClickedId(int,int)), viewport_, SLOT(setClickedId(int,int)));
}


void MouseController::mousePressEvent(QMouseEvent *event)
{


    // set the current mouse point for further translation or rotation calculations (in mouseMoveEvent)
    if (event->button() == Qt::RightButton) {
        lastTranslationPoint_ = QVector2D(event->x(), event->y());
    } else if (event->button() == Qt::LeftButton) {
        lastRotationPoint_ = mapPointToTrackball(event->x(), event->y());
        emit setClickedId(event->x(), viewport_->height() - event->y());
    } else if (event->button() == Qt::MiddleButton) {
        lastScalingPoint_ = QVector2D(event->x(), event->y());
    }

    emit setViewportActive(viewport_->getType());
    emit updateViewport();
}

void MouseController::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    bool valid = true;

    // only translate or rotate when mouse pointer within the widget
    if (x > viewport_->width() || y > viewport_->height() || x < 0 || y < 0) valid = false;

    if ((event->buttons() & Qt::RightButton) == Qt::RightButton && valid) {

        QVector2D newPoint = QVector2D(x, y);
        float dX = newPoint.x() - lastTranslationPoint_.x();
        // Qt has an inverted y-axis compared to OpenGL
        float dY = lastTranslationPoint_.y() - newPoint.y();

        float scaleFactor = 0.007; //-camera_->getZoom() * 0.001166;    // this factor was chosen through testing
//        currentTranslation += QVector2D(dX * scaleFactor, dY * scaleFactor);

        if (model_->getInteractionMode() == Model::CAMERA) {
            camera_->translate(QVector2D(dX * scaleFactor, dY * scaleFactor));

        } else if (model_->getInteractionMode() == Model::OBJECT) {
            QVector3D translation =
                    camera_->getRotation().conjugate().rotatedVector(QVector3D(dX * scaleFactor, dY * scaleFactor, 0));
            model_->getActivePrimitive()->translate(translation);
        }

        lastTranslationPoint_ = newPoint;

    } else if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton && valid) {

        QVector3D newPoint = mapPointToTrackball(x, y);

        // calculate direction to last point and construct vector that is perpendicular to the plane spanned by the two vectors
        QVector3D normal;
        normal = QVector3D::crossProduct(lastRotationPoint_, newPoint);

        /* transform the normal with the current rotation */
        normal = camera_->getRotation().conjugate().rotatedVector(normal);

        /* convert the distance between the two points to a number of degrees for the rotation */
        float degrees = acosf(QVector3D::dotProduct(newPoint, lastRotationPoint_)) * 180 / M_PI;

        // create quaternion from the axis and the angle
        QQuaternion rotation = QQuaternion::fromAxisAndAngle(normal, degrees);
        //QQuaternion rotation (degrees, normal);
        //rotation.normalize();

        if (model_->getInteractionMode() == Model::CAMERA) {
            camera_->rotate(rotation);
        } else if (model_->getInteractionMode() == Model::OBJECT) {
            model_->getActivePrimitive()->rotate(rotation);
        }



        lastRotationPoint_ = newPoint;
    } else if ((event->buttons() & Qt::MiddleButton) == Qt::MiddleButton) {

        QVector2D newPoint = QVector2D(event->x(), event->y());
        QVector2D difference = newPoint - lastScalingPoint_;

        float scalingFactor = 0;
        if (difference.x() > 0) scalingFactor = difference.length();
        else scalingFactor = -1 * difference.length();

        QVector3D scalingVector = model_->getScaleMask() * QVector3D(scalingFactor / 10,
                                                                   scalingFactor / 10,
                                                                   scalingFactor / 10);

        model_->getActivePrimitive()->scale(scalingVector);
        lastScalingPoint_ = newPoint;
    }

    emit updateViewport();

}

void MouseController::wheelEvent(QWheelEvent *event)
{
    camera_->zoom(event->delta() * 0.002f);
    emit updateViewport();
}


QVector3D MouseController::mapPointToTrackball(float x, float y) {

    /* map mouse point (given in xy-plane) to the range of [-1, 1] in both dimensions
     * note: (0, 0) equals the upper left corner in the mouse coordinates retrieved,
     *       but OpenGL considers (0, 0) to be the lower left corner. Thus, the y-axis
     *       is inverted here.
     */
    QPointF newPoint ((2 * x - viewport_->width()) / (float) viewport_->width(), (-1) * (2 * y - viewport_->height()) / (float) viewport_->height());
    // convert position of object into image space [-1, 1] with MVP
    // subtract this point from newPoint (or the other direction..)

    // treat this point as point on a unit hemisphere -> calculate corresponding z-value and normalize the vector
    float squaredDistanceFromOrigin2D = newPoint.x() * newPoint.x() + newPoint.y() * newPoint.y();

    // determine whether to map z to the sphere or the hyperbolic sheet
    float z = 0.0f;
    if (squaredDistanceFromOrigin2D < trackballRadius_ * trackballRadius_ / 2) {
        z = sqrt(trackballRadius_ * trackballRadius_ - squaredDistanceFromOrigin2D);
    } else {
        z = trackballRadius_ * trackballRadius_ / 2 / sqrt(squaredDistanceFromOrigin2D);
    }

    QVector3D newPoint3D (newPoint.x(), newPoint.y(), z);
    newPoint3D.normalize();

    return newPoint3D;
}

