#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <camera.h>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    enum ViewportType {PERSPECTIVE, FRONT, LEFT, TOP};

signals:

public slots:
    Camera* getCamera(ViewportType type);


private:
    Camera *perspectiveCamera;
    Camera *frontCamera;
    Camera *leftCamera;
    Camera *topCamera;


};

#endif // MODEL_H
