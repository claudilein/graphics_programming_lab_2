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
    QVector3D getTranslation();
    float getZoom();

signals:
    void zoomChanged();

public slots:

    void rotate(QQuaternion newRotation);
    void translate(QVector2D translation);
    void translate(QVector3D translation);
    void zoom(float zoom);
    void reset();

    void setProjectionMode(ProjectionMode newMode);
    ProjectionMode getProjectionMode();
    QMatrix4x4 getCameraMatrix();
    QMatrix4x4 getRotatedOnSpotCameraMatrix();

private:
    ProjectionMode mode_;
    bool lockRotation_;

    QVector3D pointOfInterest_;
    QQuaternion rotation_;
    QQuaternion initialRotation_;
    float zoom_;

};

#endif // CAMERA_H
