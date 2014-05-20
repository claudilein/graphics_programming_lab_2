#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QVector2D>

class Camera : public QObject
{
    Q_OBJECT
public:
    enum ProjectionMode {ORTHOGRAPHIC, PERSPECTIVE};
    explicit Camera(QObject *parent = 0, ProjectionMode mode = PERSPECTIVE, bool lockRotation = false,
                    QQuaternion rotation = QQuaternion());
    QQuaternion getRotation();

signals:
    void zoomChanged(float zoom);

public slots:

    void rotate(QQuaternion newRotation);
    void translate(QVector2D newTranslation);
    void zoom(float zoom);
    void reset();

    void setProjectionMode(ProjectionMode newMode);
    ProjectionMode getProjectionMode();
    QMatrix4x4 getCameraMatrix();

private:
    ProjectionMode mode_;
    bool lockRotation_;

    QVector3D pointOfInterest_;
    QQuaternion rotation_;
    QQuaternion initialRotation_;
    float zoom_;

};

#endif // CAMERA_H
