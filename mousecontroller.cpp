#include "mousecontroller.h"
#include <viewport.h>
#include <math.h>
#include <iostream>
#include <model.h>
#include <terrain.h>
#include <GL/gl.h>

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
    connect(viewport_, SIGNAL(transmitFeedbackHeightTextureID(GLuint)), this, SLOT(setFeedbackHeightTextureID(GLuint)));

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
            if (checkCollision()) camera_->translate(-QVector2D(dX * scaleFactor, dY * scaleFactor));

        } else if (model_->getInteractionMode() == Model::OBJECT) {
            QVector3D translation =
                    camera_->getRotation().conjugate().rotatedVector(QVector3D(dX * scaleFactor, dY * scaleFactor, 0));
            if (model_->getActivePrimitive() != NULL) {
                model_->getActivePrimitive()->translate(translation);
            }

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
            if (checkCollision()) camera_->rotate(rotation.conjugate());
        } else if (model_->getInteractionMode() == Model::OBJECT) {
            if (model_->getActivePrimitive() != NULL) {
                model_->getActivePrimitive()->rotate(rotation);
            }
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

        if (model_->getActivePrimitive() != NULL) {
            model_->getActivePrimitive()->scale(scalingVector);
        }
        lastScalingPoint_ = newPoint;
    }

    emit updateViewport();

}

void MouseController::wheelEvent(QWheelEvent *event)
{
    camera_->zoom(event->delta() * 0.002f);
    if (checkCollision()) camera_->zoom(-event->delta() * 0.002f);
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

void MouseController::keyPressEvent(QKeyEvent *event) {
   if (event->key() == Qt::Key_W) {
        QVector3D translation = camera_->getRotation().conjugate().rotatedVector(QVector3D(0, 0, 0.2));
        camera_->translate(translation);
        if (checkCollision()) camera_->translate(-translation);
    } else if (event->key() == Qt::Key_S) {
        QVector3D translation = camera_->getRotation().conjugate().rotatedVector(QVector3D(0, 0, -0.2));
        camera_->translate(translation);
        if (checkCollision()) camera_->translate(-translation);
    } else if (event->key() == Qt::Key_A) {
        QVector3D translation = camera_->getRotation().conjugate().rotatedVector(QVector3D(0.2, 0, 0));
        camera_->translate(translation);
        if (checkCollision()) camera_->translate(-translation);
    } else if (event->key() == Qt::Key_D) {
        QVector3D translation = camera_->getRotation().conjugate().rotatedVector(QVector3D(-0.2, 0, 0));
        camera_->translate(translation);
        if (checkCollision()) camera_->translate(-translation);
    }
    emit updateViewport();
}

void MouseController::keyReleaseEvent(QKeyEvent *event) {

}

bool MouseController::checkCollision() {
    bool collision = false;
    if (viewport_->getType() == Model::PERSPECTIVE) {
        QList<Primitive*> *primitives = model_->getScenegraph();
        for (int i = 0; i < primitives->size(); i++) {
            if (primitives->at(i)->isTerrain()) {
                Terrain* terrain = static_cast<Terrain*>(primitives->at(i));
                QVector4D cameraPosition = camera_->getCameraMatrix().inverted().column(3);
                float horizontalScale = terrain->getHorizontalScale();
                QVector2D cameraTexCoord = QVector2D((cameraPosition.x() + horizontalScale / 2.0f) / horizontalScale,
                                                    (-cameraPosition.z() + horizontalScale / 2.0f) / horizontalScale);


                int terrainWidth = terrain->getWidth();
                cameraTexCoord.setX(cameraTexCoord.x());
                cameraTexCoord.setY(1 -cameraTexCoord.y());
                cameraTexCoord *= (terrainWidth - 1);  // scale from [0, 1] to [0, 4096]

                // TODO row andersherum... col auch?
                int index = (cameraTexCoord.y() * terrainWidth + cameraTexCoord.x());
                std::cout << "row: " << cameraTexCoord.y() << ", column: " << cameraTexCoord.x() << std::endl;
                if (index < terrainWidth * terrainWidth && index > 0) {

                    /*(terrain->getHeightValues()[index] + terrain->getHeightValues()[index + 1] + terrain->getHeightValues()[index - 1] +
                                           terrain->getHeightValues()[index - terrainWidth] + terrain->getHeightValues()[index - terrainWidth - 1] + terrain->getHeightValues()[index - terrainWidth + 1] +
                                           terrain->getHeightValues()[index + terrainWidth] + terrain->getHeightValues()[index + terrainWidth - 1] + terrain->getHeightValues()[index + terrainWidth + 1])
                                           / 9 */

                    float terrainHeight = terrain->getHeightValues()[index] / 65535.0f *
                                          terrain->getVerticalScale()  ;
                    std::cout << "terrainHeight: " << terrain->getHeightValues()[index]
                                 << ", * vS: " << terrain->getHeightValues()[index] *
                                    terrain->getVerticalScale()
                                 << ", / 65535.0f: " << terrainHeight << std::endl;

                    //std::cout << "row: " << cameraTexCoord.y() << ", column: " << cameraTexCoord.x() << ", cam height: " << cameraPosition.y() << ", terrain height: " << terrainHeight + 0.1 << std::endl;

                    if (cameraPosition.y() <= terrainHeight + 0.1) {    // collision!
                        std::cout << "Collision: Camera height: " << cameraPosition.y() << ", terrain height: " << terrainHeight + 0.1 << std::endl;
                        collision = true;
                    } else {
                        std::cout << "No collision: camera height: " << cameraPosition.y() << ", terrain height: " << terrainHeight + 0.1 << std::endl;
                    }
                } else {    // camera is in area of clamped terrain
                    collision = true;
                }
            }
        }
    }
    return collision;
    //return false;
}

void MouseController::setFeedbackHeightTextureID(GLuint id) {
    feedbackHeightTextureID = id;
}
