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


    Camera* getCamera(ViewportType type);
    bool getActive(ViewportType type);

signals:

public slots:

    void setActive(Model::ViewportType type);

private:
    static const int NR_CAMERAS = 4;
    Camera *cameras_[NR_CAMERAS];
    ViewportType active_;


};

#endif // MODEL_H
