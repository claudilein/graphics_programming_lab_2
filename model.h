#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <camera.h>
#include <primitive.h>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    enum ViewportType {PERSPECTIVE, FRONT, LEFT, TOP};
    enum PrimitiveType {CUBE, SPHERE, CYLINDER, CONE, TORUS};


    Camera* getCamera(ViewportType type);
    bool getActive(ViewportType type);
    QList<Primitive*>* getScenegraph();

signals:

public slots:

    void setActive(Model::ViewportType type);
    void addCube();
    void addSphere();
    void addCylinder();
    void addCone();
    void addTorus();

private:
    static const int NR_CAMERAS = 4;
    static const int NR_PRIMITIVES = 5;
    Camera *cameras_[NR_CAMERAS];
    ViewportType active_;

    QList<Primitive*> *scenegraph_;
    std::vector<int> idCounters_;
    int nrIDs_;
    int tesselation_;


};

#endif // MODEL_H
