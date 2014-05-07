#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H


#include <QObject>
#include <QMouseEvent>
#include <QVector2D>
#include <QVector3D>
#include <viewport.h>


class MouseController : public QObject
{
    Q_OBJECT

public:
    explicit MouseController(QObject *parent = 0, Viewport *viewport = 0);

signals:

public slots:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Viewport *viewport_;

    QVector2D lastTranslationPoint_;
    QVector3D lastRotationPoint_;

    QVector3D mapPointToTrackball(float x, float y);

};

#endif // MOUSECONTROLLER_H



