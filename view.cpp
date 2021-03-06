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
    tesselationSlider->setValue(6);
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
    connect(createVolumeAction, SIGNAL(triggered()), this, SLOT(readVolumeFile()));

    createTerrainAction = new QAction("Create &Terrain", objectMenu);
    connect(createTerrainAction, SIGNAL(triggered()), this, SLOT(readTerrainFile()));

    deleteSelectedObjectAction = new QAction("&Delete selected object", objectMenu);
    deleteSelectedObjectAction->setShortcut(tr("Delete"));


    objectMenu->addAction(createCubeAction);
    objectMenu->addAction(createSphereAction);
    objectMenu->addAction(createCylinderAction);
    objectMenu->addAction(createConeAction);
    objectMenu->addAction(createTorusAction);
    objectMenu->addAction(createVolumeAction);
    objectMenu->addAction(createTerrainAction);
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

    // === TERRAIN SLIDERS === //

    showWireframeAction = new QAction("Wireframe", this);
    showWireframeAction->setCheckable(true);
    showWireframeAction->setChecked(false);

    horizontalScaleSlider = new QSlider(toolBar);
    horizontalScaleSlider->setOrientation(Qt::Horizontal);
    horizontalScaleSlider->setFixedWidth(80);
    horizontalScaleSlider->setRange(128, 512);
    horizontalScaleSlider->setValue(256);
    connect(horizontalScaleSlider, SIGNAL(valueChanged(int)), this, SIGNAL(setHorizontalScale(int)));

    verticalScaleSlider = new QSlider(toolBar);
    verticalScaleSlider->setOrientation(Qt::Horizontal);
    verticalScaleSlider->setFixedWidth(80);
    verticalScaleSlider->setRange(200, 700);
    verticalScaleSlider->setValue(300);
    connect(verticalScaleSlider, SIGNAL(valueChanged(int)), this, SIGNAL(setVerticalScale(int)));


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

    toolBar->addSeparator();
    toolBar->addAction(showWireframeAction);
    toolBar->addWidget(horizontalScaleSlider);
    toolBar->addWidget(verticalScaleSlider);



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

    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CompatibilityProfile);
    //qglFormat.setVersion(2,1);
    //qglFormat.setProfile(QGLFormat::CoreProfile);


    viewportPerspective = new Viewport(this, qglFormat, Model::PERSPECTIVE, model_);
    viewportFront = new Viewport(this, qglFormat, Model::FRONT, model_);
    viewportLeft = new Viewport(this, qglFormat, Model::LEFT, model_);
    viewportTop = new Viewport(this, qglFormat, Model::TOP, model_);

    // PREVIEW
    materialPreview = new MaterialPreview(this);

    std::cout << "OpenGL Version: " << viewportFront->format().majorVersion() << "." << viewportFront->format().minorVersion() << std::endl;

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

    connect(showWireframeAction, SIGNAL(toggled(bool)), viewportPerspective, SLOT(showWireframe(bool)));
    connect(showWireframeAction, SIGNAL(toggled(bool)), viewportFront, SLOT(showWireframe(bool)));
    connect(showWireframeAction, SIGNAL(toggled(bool)), viewportLeft, SLOT(showWireframe(bool)));
    connect(showWireframeAction, SIGNAL(toggled(bool)), viewportTop, SLOT(showWireframe(bool)));


    // ===== OUTLINER ===== //

    outlinerWidget = new QDockWidget("Outliner", this);
    outlinerWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    outlinerWidget->setFixedWidth(350);
    addDockWidget(Qt::RightDockWidgetArea, outlinerWidget);

    outliner = new QTreeView(outlinerWidget);
    outliner->setModel(model_->getScenegraphModel());

    outlinerWidget->setWidget(outliner);
    connect(outliner, SIGNAL(clicked(QModelIndex)), model_, SLOT(modelItemClicked(QModelIndex)));

    // === TRANSFER FUNCTION WIDGET === //

    /*transferFunctionDockWidget = new QDockWidget("Transfer Function Editor: ", this);
    transferFunctionDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    transferFunctionDockWidget->setFixedWidth(350);
    addDockWidget(Qt::RightDockWidgetArea, transferFunctionDockWidget);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    transferFunctionDockWidget->setWidget(scrollArea);

    transferFunctionEditor = new TransferFunctionEditor(transferFunctionDockWidget, model_);
    //transferFunctionDockWidget->setWidget(transferFunctionEditor);
    scrollArea->setWidget(transferFunctionEditor);

    connect(transferFunctionEditor, SIGNAL(functionChanged(Volume*)), viewportPerspective, SLOT(copyVolumeData(Volume*)));
    connect(transferFunctionEditor, SIGNAL(functionChanged(Volume*)), viewportFront, SLOT(copyVolumeData(Volume*)));
    connect(transferFunctionEditor, SIGNAL(functionChanged(Volume*)), viewportLeft, SLOT(copyVolumeData(Volume*)));
    connect(transferFunctionEditor, SIGNAL(functionChanged(Volume*)), viewportTop, SLOT(copyVolumeData(Volume*)));

    // === TERRAIN MATERIAL EDITOR WIDGET === //

    terrainMaterialEditor = new TerrainMaterialEditor(this);
    connect(terrainMaterialEditor, SIGNAL(materialAdded(QString)), model_, SLOT(materialSelected(QString)));
    //transferFunctionDockWidget->setWidget(terrainMaterialEditor);
    scrollArea->setWidget(terrainMaterialEditor);*/


    // === MATERIAL EDITOR === //

    materialEditorDockWidget = new QDockWidget("Material Editor", this);
    materialEditorDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    materialEditorDockWidget->setFixedWidth(350);
    addDockWidget(Qt::RightDockWidgetArea, materialEditorDockWidget);

    materialEditorScrollArea = new QScrollArea(this);
    materialEditorScrollArea->setWidgetResizable(true);
    materialEditorDockWidget->setWidget(materialEditorScrollArea);

    materialEditor = new MaterialEditor(this, materialPreview);
    materialEditorScrollArea->setWidget(materialEditor);


    // connects to the controller -> model -> active object -> updates viewports
    connect(materialEditor, SIGNAL(ambientColorChanged(uint,uint,uint)), this, SIGNAL(ambientColorChanged(uint,uint,uint)));
    connect(materialEditor, SIGNAL(diffuseColorChanged(uint,uint,uint)), this, SIGNAL(diffuseColorChanged(uint,uint,uint)));
    connect(materialEditor, SIGNAL(specularColorChanged(uint,uint,uint)), this, SIGNAL(specularColorChanged(uint,uint,uint)));
    connect(materialEditor, SIGNAL(roughnessChanged(int)), this, SIGNAL(roughnessChanged(int)));
    connect(materialEditor, SIGNAL(roughnessSpecularChanged(int)), this, SIGNAL(roughnessSpecularChanged(int)));
    connect(materialEditor, SIGNAL(refractionIndexChanged(int)), this, SIGNAL(refractionIndexChanged(int)));
    connect(materialEditor, SIGNAL(textureChecked(Primitive::Textures,bool)), this, SIGNAL(textureChecked(Primitive::Textures,bool)));
    connect(materialEditor, SIGNAL(textureUploaded(Primitive::Textures,QImage)), this, SIGNAL(textureUploaded(Primitive::Textures,QImage)));
    connect(materialEditor, SIGNAL(diffuseShaderChanged(int)), this, SIGNAL(diffuseShaderChanged(int)));
    connect(materialEditor, SIGNAL(specularShaderChanged(int)), this, SIGNAL(specularShaderChanged(int)));
    connect(materialEditor, SIGNAL(kaChanged(int)), this, SIGNAL(kaChanged(int)));
    connect(materialEditor, SIGNAL(kdChanged(int)), this, SIGNAL(kdChanged(int)));
    connect(materialEditor, SIGNAL(ksChanged(int)), this, SIGNAL(ksChanged(int)));

    // connects to the material preview with its one sphere
    connect(materialEditor, SIGNAL(ambientColorChanged(uint,uint,uint)), materialPreview, SLOT(setAmbientColor(uint,uint,uint)));
    connect(materialEditor, SIGNAL(diffuseColorChanged(uint,uint,uint)), materialPreview, SLOT(setDiffuseColor(uint,uint,uint)));
    connect(materialEditor, SIGNAL(specularColorChanged(uint,uint,uint)), materialPreview, SLOT(setSpecularColor(uint,uint,uint)));
    connect(materialEditor, SIGNAL(roughnessChanged(int)), materialPreview, SLOT(setRoughness(int)));
    connect(materialEditor, SIGNAL(roughnessSpecularChanged(int)), materialPreview, SLOT(setSpecularRoughness(int)));
    connect(materialEditor, SIGNAL(refractionIndexChanged(int)), materialPreview, SLOT(setRefractionIndex(int)));
    connect(materialEditor, SIGNAL(textureChecked(Primitive::Textures,bool)), materialPreview, SLOT(setTextureChecked(Primitive::Textures,bool)));
    connect(materialEditor, SIGNAL(textureUploaded(Primitive::Textures,QImage)), materialPreview, SLOT(setTexture(Primitive::Textures,QImage)));
    connect(materialEditor, SIGNAL(diffuseShaderChanged(int)), materialPreview, SLOT(setDiffuseShader(int)));
    connect(materialEditor, SIGNAL(specularShaderChanged(int)), materialPreview, SLOT(setSpecularShader(int)));
    connect(materialEditor, SIGNAL(kaChanged(int)), materialPreview, SLOT(setKa(int)));
    connect(materialEditor, SIGNAL(kdChanged(int)), materialPreview, SLOT(setKd(int)));
    connect(materialEditor, SIGNAL(ksChanged(int)), materialPreview, SLOT(setKs(int)));


    // CONNECT SIGNALS AND SLOTS


    connect(model_, SIGNAL(updateGL()), viewportPerspective, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportFront, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportLeft, SLOT(updateGL()));
    connect(model_, SIGNAL(updateGL()), viewportTop, SLOT(updateGL()));

    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportPerspective, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportFront, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportLeft, SLOT(copyVAOData(Primitive*)));
    connect(model_, SIGNAL(copyVAOData(Primitive*)), viewportTop, SLOT(copyVAOData(Primitive*)));

    connect(model_, SIGNAL(materialUploadNeeded(Terrain*,QString)), viewportPerspective, SLOT(uploadTerrainMaterialData(Terrain*,QString)));
    connect(model_, SIGNAL(materialUploadNeeded(Terrain*,QString)), viewportFront, SLOT(uploadTerrainMaterialData(Terrain*,QString)));
    connect(model_, SIGNAL(materialUploadNeeded(Terrain*,QString)), viewportLeft, SLOT(uploadTerrainMaterialData(Terrain*,QString)));
    connect(model_, SIGNAL(materialUploadNeeded(Terrain*,QString)), viewportTop, SLOT(uploadTerrainMaterialData(Terrain*,QString)));

    connect(resetCameraAction, SIGNAL(triggered()), model_, SLOT(resetCamera()));
    connect(model_, SIGNAL(selectItem(QModelIndex)), this, SLOT(selectItem(QModelIndex)));
    connect(model_, SIGNAL(updateStatusBar()), this, SLOT(updateStatusBar()));
    connect(deleteSelectedObjectAction, SIGNAL(triggered()), model_, SLOT(deleteActivePrimitive()));

    connect(model_, SIGNAL(activePrimitiveChanged()), this, SLOT(adjustToActivePrimitive()));

}


void View::readVolumeFile() {
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Volumetric Data"), "../", tr("Raw Files (*.raw)"));
    emit createVolume(fileName);

    viewportPerspective->copyVolumeData();
    viewportFront->copyVolumeData();
    viewportLeft->copyVolumeData();
    viewportTop->copyVolumeData();

}

void View::readTerrainFile() {
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Height Map"), "../", tr("Pgm Files (*.pgm)"));
    if (fileName != "") {
        emit createTerrain(fileName);
    }
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
    } else {
        emit keyPressed(event);
    }
}


void View::keyReleaseEvent(QKeyEvent *event) {
    if ((event->modifiers() & Qt::CTRL) == 0) {
        if (model_->getInteractionMode() == Model::OBJECT) emit setCameraMode();
        else if (model_->getInteractionMode() == Model::CAMERA) emit setObjectMode();
    } else {
        emit keyReleased(event);
    }
}

void View::adjustToActivePrimitive() {
    materialEditor->adjustToPrimitive(model_->getActivePrimitive());
}




