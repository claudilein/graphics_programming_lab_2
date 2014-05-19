#ifndef VIEW_H
#define VIEW_H

#include <model.h>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <QToolButton>
#include <QSplitter>
#include <viewport.h>
#include <QLabel>
#include <QTreeView>
#include <QDockWidget>

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);
    ~View();

    void setModel(Model *model);

    Viewport* getViewport(Model::ViewportType type);

signals:
    void createCube();
    void createSphere();
    void createCylinder();
    void createCone();
    void createTorus();

    void setSingleViewMode();
    void setDualViewMode();
    void setQuadViewMode();

    void setTesselation(int t);

    void setObjectMode();
    void setCameraMode();

    void updateViewports();

public slots:
    void selectItem(QModelIndex index);
    void updateStatusBar();


private:

    Model *model_;


    // visible Items

    QMenuBar *menuBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;
    QLabel *activePrimitiveLabel;
    QSlider *tesselationSlider;

    // == FILE MENU == //
    QMenu *fileMenu;
    QAction *exitAction;

    // == OBJECT MENU == //
    QMenu *objectMenu;
    QAction *createCubeAction;
    QAction *createSphereAction;
    QAction *createCylinderAction;
    QAction *createConeAction;
    QAction *createTorusAction;
    QAction *deleteSelectedObjectAction;
    QAction *renameSelectedObjectAction;

    // == INTERACTION MODE MENU == //
    QMenu *interactionModeMenu;
    QActionGroup *interactionModeGroup;
    QAction *setCameraModeAction;
    QAction *setObjectManipulationModeAction;

    // == CAMERA MENU == //
    QMenu *cameraMenu;
    QAction *resetCameraAction;

    // == VIEW MODE MENU == //
    QMenu *viewModeMenu;
    QToolButton *viewModeButton;
    QActionGroup *viewModeGroup;
    QAction *setSingleViewModeAction;
    QAction *setDualViewModeAction;
    QAction *setQuadViewModeAction;

    // == VIEWPORTS == //
    Viewport *viewportPerspective;
    Viewport *viewportFront;
    Viewport *viewportLeft;
    Viewport *viewportTop;

    QSplitter *splitterHorizontalTop;
    QSplitter *splitterHorizontalBottom;
    QSplitter *splitterVertical;

    // === OUTLINER === //
    QTreeView *outliner;
    QDockWidget *dockWidget;


};

#endif // VIEW_H
