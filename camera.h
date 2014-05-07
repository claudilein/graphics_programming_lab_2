#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>

class Camera : public QObject
{
    Q_OBJECT
public:
    enum ProjectionMode {ORTHOGRAPHIC, PERSPECTIVE};
    explicit Camera(QObject *parent = 0, ProjectionMode mode = PERSPECTIVE, bool lockRotation = false);

signals:

public slots:

    void rotate(QQuaternion newRotation);
    void translate(QVector3D newTranslation);
    void setPointOfInterest(QVector3D newPointOfInterest);
    void setProjectionMode(ProjectionMode newMode);
    void reset();

    ProjectionMode getProjectionMode();
    QMatrix4x4 getCameraMatrix();
    QVector3D getPosition();
    QVector3D getPointOfInterest();

private:
    ProjectionMode mode_;
    bool lockRotation_;

    QVector3D position_;
    QVector3D pointOfInterest_;
    QQuaternion rotation_;
    QVector3D translation_;

};

#endif // CAMERA_H
