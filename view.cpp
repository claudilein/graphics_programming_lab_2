#include "view.h"


#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <QToolBar>
#include <QStatusBar>
#include <QToolButton>
#include <QTreeView>
#include <QDockWidget>

View::View(QWidget *parent)
    : QMainWindow(parent)
{

    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    toolBar = new QToolBar(this);
    addToolBar(toolBar);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    activePrimitiveLabel = new QLabel("Background");
    activePrimitiveLabel->setAlignment(Qt::AlignLeft);
    activePrimitiveLabel->setMinimumSize(activePrimitiveLabel->sizeHint());

    statusBar->addWidget(activePrimitiveLabel);

    tesselationSlider = new QSlider(toolBar);
    tesselationSlider->setOrientation(Qt::Horizontal);
    tesselationSlider->setFixedWidth(80);
    tesselationSlider->setRange(1, 50);
    connect(tesselationSlider, SIGNAL(valueChanged(int)), this, SIGNAL(setTesselation(int)));


    // == FILE MENU == //

    fileMenu = new QMenu("&File");

    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(exitAction);


    // == OBJECT MENU == //

    objectMenu = new QMenu("&Object");

    createCubeAction = new QAction("Create &Cube", objectMenu);
    createCubeAction->setIcon(QIcon(":/img/box.png"));
    connect(createCubeAction, SIGNAL(triggered()), this, SIGNAL(createCube()));


    createSphereAction = new QAction("Create &Sphere", objectMenu);
    createSphereAction->setIcon(QIcon(":/img/sphere.png"));
    connect(createSphereAction, SIGNAL(triggered()), this, SIGNAL(createSphere()));

    createCylinderAction = new QAction("Create C&ylinder", objectMenu);
    createCylinderAction->setIcon(QIcon(":/img/cylinder.png"));
    connect(createCylinderAction, SIGNAL(triggered()), this, SIGNAL(createCylinder()));

    createConeAction = new QAction("Create Co&ne", objectMenu);
    createConeAction->setIcon(QIcon(":/img/cone.png"));
    connect(createConeAction, SIGNAL(triggered()), this, SIGNAL(createCone()));

    createTorusAction = new QAction("Create &Torus", objectMenu);
    createTorusAction->setIcon(QIcon(":/img/torus.png"));
    connect(createTorusAction, SIGNAL(triggered()), this, SIGNAL(createTorus()));

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
    interactionModeGroup = new QActionGroup(interactionModeMenu);

    setCameraModeAction = new QAction("Ca&mera Mode", interactionModeMenu);
    setCameraModeAction->setIcon(QIcon(":/img/camera.png"));
    setCameraModeAction->setCheckable(true);
    setCameraModeAction->setChecked(true);
    connect(setCameraModeAction, SIGNAL(triggered()), this, SIGNAL(setCameraMode()));

    setObjectManipulationModeAction = new QAction("Ob&ject Manipulation Mode", interactionModeMenu);
    setObjectManipulationModeAction->setIcon(QIcon(":/img/select.png"));
    setObjectManipulationModeAction->setCheckable(true);
    connect(setObjectManipulationModeAction, SIGNAL(triggered()), this, SIGNAL(setObjectMode()));

    interactionModeGroup->addAction(setCameraModeAction);
    interactionModeGroup->addAction(setObjectManipulationModeAction);

    interactionModeMenu->addAction(setCameraModeAction);
    interactionModeMenu->addAction(setObjectManipulationModeAction);


    // == CAMERA MENU == //

    cameraMenu = new QMenu("C&amera Menu");

    resetCameraAction = new QAction("&Reset Camera", cameraMenu);
    resetCameraAction->setIcon(QIcon(":/img/cam_home.png"));
    //connect(resetCameraAction, SIGNAL(triggered()), controller, SLOT(resetCamera(Camera)));

    cameraMenu->addAction(resetCameraAction);


    // == VIEW MODE MENU == //

    viewModeMenu = new QMenu("&View Mode");
    viewModeGroup = new QActionGroup(viewModeMenu);

    viewModeButton = new QToolButton(viewModeMenu);
    viewModeButton->setMenu(viewModeMenu);
    viewModeButton->setPopupMode(QToolButton::InstantPopup);
    viewModeButton->setIcon(QIcon(":/img/viewports.png"));

    setSingleViewModeAction = new QAction("S&ingle View", viewModeMenu);
    setSingleViewModeAction->setIcon(QIcon(":/img/view-single.png"));
    setSingleViewModeAction->setShortcut(tr("1"));
    setSingleViewModeAction->setCheckable(true);
    setSingleViewModeAction->setChecked(true);
    connect(setSingleViewModeAction, SIGNAL(triggered()), this, SIGNAL(setSingleViewMode()));

    setDualViewModeAction = new QAction("&Dual View", viewModeMenu);
    setDualViewModeAction->setShortcut(tr("2"));
    setDualViewModeAction->setIcon(QIcon(":/img/view-dual.png"));
    setDualViewModeAction->setCheckable(true);
    connect(setSingleViewModeAction, SIGNAL(triggered()), this, SIGNAL(setDualViewMode()));

    setQuadViewModeAction = new QAction("&Quad View", viewModeMenu);
    setQuadViewModeAction->setShortcut(tr("4"));
    setQuadViewModeAction->setIcon(QIcon(":/img/viewports.png"));
    setQuadViewModeAction->setCheckable(true);
    connect(setSingleViewModeAction, SIGNAL(triggered()), this, SIGNAL(setQuadViewMode()));

    viewModeGroup->addAction(setSingleViewModeAction);
    viewModeGroup->addAction(setDualViewModeAction);
    viewModeGroup->addAction(setQuadViewModeAction);

    viewModeMenu->addAction(setSingleViewModeAction);
    viewModeMenu->addAction(setDualViewModeAction);
    viewModeMenu->addAction(setQuadViewModeAction);


    // ===== MENU BAR ===== //

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(objectMenu);
    menuBar->addMenu(interactionModeMenu);
    menuBar->addMenu(cameraMenu);
    menuBar->addMenu(viewModeMenu);

    // ===== TOOL BAR ===== //

    toolBar->addAction(createCubeAction);
    toolBar->addAction(createSphereAction);
    toolBar->addAction(createCylinderAction);
    toolBar->addAction(createConeAction);
    toolBar->addAction(createTorusAction);

    toolBar->addSeparator();
    toolBar->addAction(setCameraModeAction);
    toolBar->addAction(setObjectManipulationModeAction);
    toolBar->addAction(resetCameraAction);

    toolBar->addSeparator();
    toolBar->addWidget(viewModeButton);

    toolBar->addSeparator();
    toolBar->addWidget(tesselationSlider);









}

View::~View()
{

}

void View::setModel(Model *model)
{
    model_ = model;

    // ===== VIEWPORTS ===== //

    viewportPerspective = new Viewport(this, Model::PERSPECTIVE, model_);
    viewportFront = new Viewport(this, Model::FRONT, model_);
    viewportLeft = new Viewport(this, Model::LEFT, model_);
    viewportTop = new Viewport(this, Model::TOP, model_);

    splitterHorizontalTop = new QSplitter(this);
    splitterHorizontalTop->addWidget(viewportPerspective);
    splitterHorizontalTop->addWidget(viewportFront);

    splitterHorizontalBottom = new QSplitter(this);
    splitterHorizontalBottom->addWidget(viewportLeft);
    splitterHorizontalBottom->addWidget(viewportTop);

    splitterVertical = new QSplitter(this);
    splitterVertical->setOrientation(Qt::Vertical);
    splitterVertical->addWidget(splitterHorizontalTop);
    splitterVertical->addWidget(splitterHorizontalBottom);

    setCentralWidget(splitterVertical);


    viewportPerspective->setCamera(model_->getCamera(Model::PERSPECTIVE));
    viewportFront->setCamera(model_->getCamera(Model::FRONT));
    viewportLeft->setCamera(model_->getCamera(Model::LEFT));
    viewportTop->setCamera(model_->getCamera(Model::TOP));

    connect(this, SIGNAL(updateViewports()), viewportPerspective, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportFront, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportLeft, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportTop, SLOT(updateGL()));


    // ===== OUTLINER ===== //

    dockWidget = new QDockWidget("Outliner: ", this);
    outliner = new QTreeView(dockWidget);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    outliner->setModel(model_->getScenegraphModel());
    dockWidget->setWidget(outliner);


    connect(model_, SIGNAL(updateGL()), viewportPerspective, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportFront, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportLeft, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportTop, SLOT(updateGL()));

    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportPerspective, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportFront, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportLeft, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportTop, SLOT(copyVAOData(Primitive*)));

    connect(model_, SIGNAL(updateStatusBar()), this, SLOT(updateStatusBar()));

}

Viewport* View::getViewport(Model::ViewportType type)
{
    switch (type) {
    case Model::PERSPECTIVE : return viewportPerspective;
    case Model::FRONT: return viewportFront;
    case Model::LEFT: return viewportLeft;
    case Model::TOP: return viewportTop;
    default: return viewportPerspective;
    }
}

void View::updateStatusBar()
{
    if (model_->getActivePrimitive() != NULL) {
        activePrimitiveLabel->setText(QString::fromStdString(model_->getActivePrimitive()->getName()));
    } else {
        activePrimitiveLabel->setText(QString("Background"));
    }

    outliner->setModel(0);
    outliner->setModel(model_->getScenegraphModel());
}

