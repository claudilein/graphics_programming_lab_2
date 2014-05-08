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
                    QVector3D position = QVector3D(0,0,-3), QVector3D up = QVector3D(0,1,0));

signals:

public slots:

    void rotate(QQuaternion newRotation);
    void translate(QVector2D newTranslation);
    void zoom(float zoom);
    void setPointOfInterest(QVector3D newPointOfInterest);
    void setProjectionMode(ProjectionMode newMode);
    void reset();

    ProjectionMode getProjectionMode();
    QMatrix4x4 getCameraMatrix();
    QVector3D getPosition();
    QVector3D getPointOfInterest();
    QVector3D getUpVector();
    float getZoom();

private:
    ProjectionMode mode_;
    bool lockRotation_;

    QVector3D startPosition_;
    QVector3D position_;
    QVector3D up_;

    QVector3D pointOfInterest_;
    QQuaternion rotation_;
    QVector2D translation_;
    float zoom_;

};

#endif // CAMERA_H
