#ifndef VIEW_H
#define VIEW_H

#include <controller.h>

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QSlider>

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);
    ~View();

    Controller *controller;


    // visible Items

    QMenuBar *menuBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;
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
    QAction *setCameraModeAction;
    QAction *setObjectManipulationModeAction;

    // == CAMERA MENU == //
    QMenu *cameraMenu;
    QAction *resetCameraAction;

    // == VIEW MODE MENU == //
    QMenu *viewModeMenu;
    QAction *setSingleViewModeAction;
    QAction *setDualViewModeAction;
    QAction *setQuadViewModeAction;


};

#endif // VIEW_H
