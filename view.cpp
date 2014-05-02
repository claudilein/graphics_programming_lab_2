#include "view.h"


#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <QToolBar>
#include <QStatusBar>

View::View(QWidget *parent)
    : QMainWindow(parent)
{

    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    toolBar = new QToolBar(this);
    addToolBar(toolBar);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    tesselationSlider = new QSlider(toolBar);
    tesselationSlider->setOrientation(Qt::Horizontal);
    tesselationSlider->setFixedWidth(80);
    tesselationSlider->setRange(1, 50);
    // connect..

    // == FILE MENU == //

    fileMenu = new QMenu("&File");

    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(exitAction);


    // == OBJECT MENU == //

    objectMenu = new QMenu("&Object");

    createCubeAction = new QAction("Create &Cube", objectMenu);
    //connect(createCubeAction, SIGNAL(triggered()), controller, SLOT(createCube()));

    createSphereAction = new QAction("Create &Sphere", objectMenu);
    //connect(createSphereAction, SIGNAL(triggered()), controller, SLOT(createSphere()));

    createCylinderAction = new QAction("Create C&ylinder", objectMenu);
    //connect(createCylinderAction, SIGNAL(triggered()), controller, SLOT(createCylinder()));

    createConeAction = new QAction("Create Co&ne", objectMenu);
    //connect(createConeAction, SIGNAL(triggered()), controller, SLOT(createCone()));

    createTorusAction = new QAction("Create &Torus", objectMenu);
    //connect(createTorusAction, SIGNAL(triggered()), controller, SLOT(createTorus()));

    deleteSelectedObjectAction = new QAction("&Delete selected object", objectMenu);
    //connect(deleteSelectedObjectAction, SIGNAL(triggered()), controller, SLOT(deleteSelectedObject()));

    renameSelectedObjectAction = new QAction("Rename se&lected object", objectMenu);
    //connect(renameSelectedObjectAction, SIGNAL(triggered()), controller, SLOT(renameSelectedObject()));

    objectMenu->addAction(createCubeAction);
    objectMenu->addAction(createSphereAction);
    objectMenu->addAction(createCylinderAction);
    objectMenu->addAction(createConeAction);
    objectMenu->addAction(createTorusAction);
    objectMenu->addAction(deleteSelectedObjectAction);
    objectMenu->addAction(renameSelectedObjectAction);


    // == INTERACTION MODE MENU == //

    interactionModeMenu = new QMenu("&Interaction Mode");

    setCameraModeAction = new QAction("Ca&mera Mode", interactionModeMenu);
    //connect(setCameraModeAction, SIGNAL(triggered()), controller, SLOT(selectMode(CAMERA_MODE)));

    setObjectManipulationModeAction = new QAction("Ob&ject Manipulation Mode", interactionModeMenu);
    //connect(setObjectManipulationModeAction, SIGNAL(triggered()), controller, SLOT(selectMode(OBJECT_MANIPULATION_MODE)));

    interactionModeMenu->addAction(setCameraModeAction);
    interactionModeMenu->addAction(setObjectManipulationModeAction);


    // == CAMERA MENU == //

    cameraMenu = new QMenu("C&amera Menu");

    resetCameraAction = new QAction("&Reset Camera", cameraMenu);
    //connect(resetCameraAction, SIGNAL(triggered()), controller, SLOT(resetCamera(Camera)));

    cameraMenu->addAction(resetCameraAction);


    // == VIEW MODE MENU == //

    viewModeMenu = new QMenu("&View Mode");

    setSingleViewModeAction = new QAction("S&ingle View", viewModeMenu);
    //connect(setSingleViewModeAction, SIGNAL(triggered()), controller, SLOT(setViewMode(SINGLE)));

    setDualViewModeAction = new QAction("&Dual View", viewModeMenu);
    //connect(setDualViewModeAction, SIGNAL(triggered()), controller, SLOT(setViewMode(DUAL)));

    setQuadViewModeAction = new QAction("&Quad View", viewModeMenu);
    //connect(setQuadViewModeAction, SIGNAL(triggered()), controller, SLOT(setViewMode(QUAD)));

    viewModeMenu->addAction(setSingleViewModeAction);
    viewModeMenu->addAction(setDualViewModeAction);
    viewModeMenu->addAction(setQuadViewModeAction);


    // ===== MENU BAR ===== //

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(objectMenu);
    menuBar->addMenu(interactionModeMenu);
    menuBar->addMenu(cameraMenu);
    menuBar->addMenu(viewModeMenu);

}

View::~View()
{

}
