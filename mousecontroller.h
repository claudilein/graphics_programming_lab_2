#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H


#include <QObject>
#include <QMouseEvent>
#include <QVector2D>
#include <QVector3D>
#include <viewport.h>
#include <camera.h>
#include <model.h>


class MouseController : public QObject
{
    Q_OBJECT

public:
    explicit MouseController(QObject *parent = 0, Viewport *viewport = 0, Camera *camera = 0);

signals:
    void updateViewport();
    void setViewportActive(Model::ViewportType type);

public slots:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Viewport *viewport_;
    Camera *camera_;
    float trackballRadius_;

    QVector2D lastTranslationPoint_;
    QVector3D lastRotationPoint_;

    QVector3D mapPointToTrackball(float x, float y);

};

#endif // MOUSECONTROLLER_H



