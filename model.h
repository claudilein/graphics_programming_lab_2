#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <camera.h>
#include <primitive.h>
#include <scenegraph.h>
#include <QModelIndex>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    enum ViewportType {PERSPECTIVE, FRONT, LEFT, TOP};
    enum PrimitiveType {CUBE, SPHERE, CYLINDER, CONE, TORUS};
    enum InteractionType {OBJECT, CAMERA};


    Camera* getCamera(ViewportType type);
    bool isActiveViewport(ViewportType type);
    Primitive* getActivePrimitive();
    QList<Primitive*>* getScenegraph();
    Scenegraph* getScenegraphModel();
    InteractionType getInteractionMode();

signals:
    void copyVAOData(Primitive *p);
    void selectItem(QModelIndex index);
    void updateStatusBar();
    void updateGL();


public slots:

    void setActive(Model::ViewportType type);
    void addCube();
    void addSphere();
    void addCylinder();
    void addCone();
    void addTorus();
    void setActivePrimitive(float ID);
    void setTesselation(int t);
    void setObjectInteractionMode();
    void setCameraInteractionMode();
    void resetCamera();
    void modelItemClicked(QModelIndex index);
    void renamePrimitive(QString name);
    void deleteActivePrimitive();

private:
    static const int NR_CAMERAS = 4;
    static const int NR_PRIMITIVES = 5;
    static const int NR_INTERACTION_TYPES = 2;
    Camera *cameras_[NR_CAMERAS];
    ViewportType active_;
    Primitive* activePrimitive_;

    //QList<Primitive*> *scenegraph_;
    Scenegraph *scenegraphModel_;
    std::vector<int> idCounters_;
    int nrIDs_;
    int tesselation_;
    InteractionType interactionMode_;



};

#endif // MODEL_H
