#include "mousecontroller.h"
#include <viewport.h>

MouseController::MouseController(QObject *parent, Viewport *viewport) :
    QObject(parent)
{
    viewport_ = viewport;
    connect(viewport_, SIGNAL(mousePressEvent(QMouseEvent *event)), this, SLOT(mousePressEvent(QMouseEvent*)));
}


void MouseController::mousePressEvent(QMouseEvent *event)
{
    // set the current mouse point for further translation or rotation calculations (in mouseMoveEvent)
    if (event->button() == Qt::RightButton) {
        lastTranslationPoint_ = QVector2D(event->x(), event->y());
    } else if (event->button() == Qt::LeftButton) {
        lastRotationPoint_ = mapPointToTrackball(event->x(), event->y());
    }

    // emit updateGL();
}

void MouseController::mouseMoveEvent(QMouseEvent *event)
{

}

void MouseController::wheelEvent(QWheelEvent *event)
{

}


QVector3D MouseController::mapPointToTrackball(float x, float y) {

    /* map mouse point (given in xy-plane) to the range of [-1, 1] in both dimensions
     * note: (0, 0) equals the upper left corner in the mouse coordinates retrieved,
     *       but OpenGL considers (0, 0) to be the lower left corner. Thus, the y-axis
     *       is inverted here.
     */
    QPointF newPoint ((2 * x - width()) / (float) width(), (-1) * (2 * y - height()) / (float) height());

    // treat this point as point on a unit hemisphere -> calculate corresponding z-value and normalize the vector
    float squaredDistanceFromOrigin2D = newPoint.x() * newPoint.x() + newPoint.y() * newPoint.y();

    // determine whether to map z to the sphere or the hyperbolic sheet
    float z = 0.0f;
    if (squaredDistanceFromOrigin2D < trackballRadius * trackballRadius / 2) {
        z = sqrt(trackballRadius * trackballRadius - squaredDistanceFromOrigin2D);
    } else {
        z = trackballRadius * trackballRadius / 2 / sqrt(squaredDistanceFromOrigin2D);
    }

    QVector3D newPoint3D (newPoint.x(), newPoint.y(), z);
    newPoint3D.normalize();

    return newPoint3D;
}
