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
#include <QKeyEvent>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

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

    createVolumeAction = new QAction("Create &Volume", objectMenu);
    connect(createVolumeAction, SIGNAL(triggered()), this, SLOT(readFile()));

    deleteSelectedObjectAction = new QAction("&Delete selected object", objectMenu);
    deleteSelectedObjectAction->setShortcut(tr("Delete"));


    objectMenu->addAction(createCubeAction);
    objectMenu->addAction(createSphereAction);
    objectMenu->addAction(createCylinderAction);
    objectMenu->addAction(createConeAction);
    objectMenu->addAction(createTorusAction);
    objectMenu->addAction(createVolumeAction);
    objectMenu->addAction(deleteSelectedObjectAction);


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
    connect(setDualViewModeAction, SIGNAL(triggered()), this, SIGNAL(setDualViewMode()));

    setQuadViewModeAction = new QAction("&Quad View", viewModeMenu);
    setQuadViewModeAction->setShortcut(tr("4"));
    setQuadViewModeAction->setIcon(QIcon(":/img/viewports.png"));
    setQuadViewModeAction->setCheckable(true);
    connect(setQuadViewModeAction, SIGNAL(triggered()), this, SIGNAL(setQuadViewMode()));

    viewModeGroup->addAction(setSingleViewModeAction);
    viewModeGroup->addAction(setDualViewModeAction);
    viewModeGroup->addAction(setQuadViewModeAction);

    viewModeMenu->addAction(setSingleViewModeAction);
    viewModeMenu->addAction(setDualViewModeAction);
    viewModeMenu->addAction(setQuadViewModeAction);


    // == ABOUT MENU == //

    aboutMenu = new QMenu("&About");

    aboutAction = new QAction("&About", menuBar);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    aboutMenu->addAction(aboutAction);

    // ===== MENU BAR ===== //

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(objectMenu);
    menuBar->addMenu(interactionModeMenu);
    menuBar->addMenu(cameraMenu);
    menuBar->addMenu(viewModeMenu);
    menuBar->addMenu(aboutMenu);


    // === SCALING === //

//    scalingGroup = new QActionGroup(this);
    scaleXAction = new QAction("X", this);
    scaleXAction->setCheckable(true);
    connect(scaleXAction, SIGNAL(toggled(bool)), this, SIGNAL(setScaleX(bool)));

    scaleYAction = new QAction("Y", this);
    scaleYAction->setCheckable(true);
    connect(scaleYAction, SIGNAL(toggled(bool)), this, SIGNAL(setScaleY(bool)));

    scaleZAction = new QAction("Z", this);
    scaleZAction->setCheckable(true);
    connect(scaleZAction, SIGNAL(toggled(bool)), this, SIGNAL(setScaleZ(bool)));

    // GRID //

    showGrid = new QAction("Grid", this);
    showGrid->setIcon(QIcon(":/img/grid.png"));
    showGrid->setCheckable(true);
    showGrid->setChecked(true);

    gridSizeSlider = new QSlider(toolBar);
    gridSizeSlider->setOrientation(Qt::Horizontal);
    gridSizeSlider->setFixedWidth(80);
    gridSizeSlider->setRange(5, 20);
    gridSizeSlider->setToolTip(QString("grid size"));
    connect(gridSizeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(setGridSize(int)));

    stepSizeSlider = new QSlider(toolBar);
    stepSizeSlider->setOrientation(Qt::Horizontal);
    stepSizeSlider->setFixedWidth(80);
    stepSizeSlider->setRange(1, 20);
    stepSizeSlider->setToolTip(QString("step size"));
    connect(stepSizeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(setStepSize(int)));

    // === MAXIMUM INTENSITY PROJECTION FOR VOLUMES === //

    mipAction = new QAction("MIP", this);
    mipAction->setCheckable(true);
    mipAction->setChecked(false);


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

    toolBar->addSeparator();
    toolBar->addAction(scaleXAction);
    toolBar->addAction(scaleYAction);
    toolBar->addAction(scaleZAction);

    toolBar->addSeparator();
    toolBar->addAction(showGrid);
    toolBar->addWidget(gridSizeSlider);
    toolBar->addWidget(stepSizeSlider);

    toolBar->addSeparator();
    toolBar->addAction(mipAction);





}

View::~View()
{

}

void View::showAboutBox()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About ModelingTool!");
    msgBox.setText("Written by Claudia Pfreundt!");
    msgBox.exec();
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
    QList<int> sizes;
    sizes.append(400);
    sizes.append(400);
    sizes.append(400);
    sizes.append(400);
    splitterVertical->setSizes(sizes);
    setCentralWidget(splitterVertical);


    viewportFront->hide();
    viewportLeft->hide();
    viewportTop->hide();


    viewportPerspective->setCamera(model_->getCamera(Model::PERSPECTIVE));
    viewportFront->setCamera(model_->getCamera(Model::FRONT));
    viewportLeft->setCamera(model_->getCamera(Model::LEFT));
    viewportTop->setCamera(model_->getCamera(Model::TOP));

    connect(this, SIGNAL(updateViewports()), viewportPerspective, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportFront, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportLeft, SLOT(updateGL()));
    connect(this, SIGNAL(updateViewports()), viewportTop, SLOT(updateGL()));

    connect(showGrid, SIGNAL(toggled(bool)), viewportPerspective, SLOT(showGrid(bool)));
    connect(showGrid, SIGNAL(toggled(bool)), viewportFront, SLOT(showGrid(bool)));
    connect(showGrid, SIGNAL(toggled(bool)), viewportLeft, SLOT(showGrid(bool)));
    connect(showGrid, SIGNAL(toggled(bool)), viewportTop, SLOT(showGrid(bool)));

    connect(this, SIGNAL(setGridSize(int)), viewportPerspective, SLOT(setGridSize(int)));
    connect(this, SIGNAL(setGridSize(int)), viewportFront, SLOT(setGridSize(int)));
    connect(this, SIGNAL(setGridSize(int)), viewportLeft, SLOT(setGridSize(int)));
    connect(this, SIGNAL(setGridSize(int)), viewportTop, SLOT(setGridSize(int)));

    connect(this, SIGNAL(setStepSize(int)), viewportPerspective, SLOT(setStepSize(int)));
    connect(this, SIGNAL(setStepSize(int)), viewportFront, SLOT(setStepSize(int)));
    connect(this, SIGNAL(setStepSize(int)), viewportLeft, SLOT(setStepSize(int)));
    connect(this, SIGNAL(setStepSize(int)), viewportTop, SLOT(setStepSize(int)));

    connect(mipAction, SIGNAL(toggled(bool)), viewportPerspective, SLOT(setMip(bool)));
    connect(mipAction, SIGNAL(toggled(bool)), viewportFront, SLOT(setMip(bool)));
    connect(mipAction, SIGNAL(toggled(bool)), viewportLeft, SLOT(setMip(bool)));
    connect(mipAction, SIGNAL(toggled(bool)), viewportTop, SLOT(setMip(bool)));

    // ===== OUTLINER ===== //

    outlinerWidget = new QDockWidget("Outliner: ", this);
    outlinerWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, outlinerWidget);

    outliner = new QTreeView(outlinerWidget);
    outliner->setModel(model_->getScenegraphModel());

    outlinerWidget->setWidget(outliner);
    connect(outliner, SIGNAL(clicked(QModelIndex)), model_, SLOT(modelItemClicked(QModelIndex)));

    // === TRANSFER FUNCTION WIDGET === //

    transferFunctionDockWidget = new QDockWidget("Transfer Function Editor: ", this);
    transferFunctionDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, transferFunctionDockWidget);

    transferFunctionEditor = new TransferFunctionEditor(transferFunctionDockWidget, model_);
    transferFunctionDockWidget->setWidget(transferFunctionEditor);


    // CONNECT SIGNALS AND SLOTS


    connect(model_, SIGNAL(updateGL()), viewportPerspective, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportFront, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportLeft, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportTop, SLOT(updateGL()));

    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportPerspective, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportFront, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportLeft, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportTop, SLOT(copyVAOData(Primitive*)));

    connect(resetCameraAction, SIGNAL(triggered()), model_, SLOT(resetCamera()));
    connect(model_, SIGNAL(selectItem(QModelIndex)), this, SLOT(selectItem(QModelIndex)));
    connect(model_, SIGNAL(updateStatusBar()), this, SLOT(updateStatusBar()));
    connect(deleteSelectedObjectAction, SIGNAL(triggered()), model_, SLOT(deleteActivePrimitive()));
}


void View::readFile() {
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Volumetric Data"), "../", tr("Raw Files (*.raw)"));
    emit createVolume(fileName);
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

void View::updateStatusBar() {
    if (model_->getActivePrimitive() != NULL) {
        activePrimitiveLabel->setText(QString::fromStdString(model_->getActivePrimitive()->getName()));
    } else {
        activePrimitiveLabel->setText(QString("<None>"));
    }
}

void View::selectItem(QModelIndex index) {
    outliner->setCurrentIndex(index);
}


void View::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::CTRL) {
        if (model_->getInteractionMode() == Model::OBJECT) emit setCameraMode();
        else if (model_->getInteractionMode() == Model::CAMERA) emit setObjectMode();
    }
}


void View::keyReleaseEvent(QKeyEvent *event) {
    if ((event->modifiers() & Qt::CTRL) == 0) {
        if (model_->getInteractionMode() == Model::OBJECT) emit setCameraMode();
        else if (model_->getInteractionMode() == Model::CAMERA) emit setObjectMode();
    }
}




