#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    enum ProjectionMode {ORTHOGRAPHIC, PERSPECTIVE};

signals:

public slots:

//    setPosition(QVector3D position);
//    setPointOfInterest(QVector3D pointOfInterest);
//    setProjectionMode(ProjectionMode mode);
//    reset();


private:
    ProjectionMode mode;
    bool lockRotation;

    QVector3D position;
    QVector3D pointOfInterest;
    QMatrix4x4 cameraMatrix;

};

#endif // CAMERA_H
