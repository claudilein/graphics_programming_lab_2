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
#include <transferfunctionwidget.h>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <transferfunctioneditor.h>
#include <terrainmaterialeditor.h>
#include <QScrollArea>
#include <materialeditor.h>
#include <materialpreview.h>

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
    void createVolume(QString fileName);
    void createTerrain(QString fileName);

    void setSingleViewMode();
    void setDualViewMode();
    void setQuadViewMode();

    void setTesselation(int t);

    void setObjectMode();
    void setCameraMode();

    void updateViewports();


    void setScaleX(bool on);
    void setScaleY(bool on);
    void setScaleZ(bool on);

    void setGridSize(int i);
    void setStepSize(int i);

    void setHorizontalScale(int i);
    void setVerticalScale(int i);

    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);

    void ambientColorChanged(uint red, uint green, uint blue);
    void diffuseColorChanged(uint red, uint green, uint blue);
    void specularColorChanged(uint red, uint green, uint blue);
    void roughnessChanged(int roughness);
    void roughnessSpecularChanged(int roughness);
    void refractionIndexChanged(int refractionIndex);
    void textureChecked(Primitive::Textures x, bool status);
    void textureUploaded(Primitive::Textures x, QImage texture);
    void diffuseShaderChanged(int i);
    void specularShaderChanged(int i);
    void kaChanged(int ka);
    void kdChanged(int kd);
    void ksChanged(int ks);

public slots:
    void showAboutBox();
    void selectItem(QModelIndex index);
    void updateStatusBar();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void readVolumeFile();
    void readTerrainFile();
    void adjustToActivePrimitive();
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
    QAction *createVolumeAction;
    QAction *createTerrainAction;
    QAction *deleteSelectedObjectAction;

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

    // == ABOUT MENU == //
    QMenu *aboutMenu;
    QAction *aboutAction;

    // == VIEWPORTS == //
    Viewport *viewportPerspective;
    Viewport *viewportFront;
    Viewport *viewportLeft;
    Viewport *viewportTop;

    QSplitter *splitterHorizontalTop;
    QSplitter *splitterHorizontalBottom;
    QSplitter *splitterVertical;

    // === PREVIEW === //
    MaterialPreview *materialPreview;


    // === OUTLINER === //
    QTreeView *outliner;
    QDockWidget *outlinerWidget;

    // === TRANSFER FUNCTION === //
    QDockWidget *transferFunctionDockWidget;
    QScrollArea *scrollArea;
    TransferFunctionEditor *transferFunctionEditor;


    // === SCALING === //
    QActionGroup *scalingGroup;
    QAction *scaleXAction;
    QAction *scaleYAction;
    QAction *scaleZAction;

    // === GRID === //

    QAction *showGrid;
    QSlider *gridSizeSlider;
    QSlider *stepSizeSlider;

    // === MAXIMUM INTENSITY PROJECTION FOR VOLUMES === //

    QAction *mipAction;

    // === TERRAIN === //

    QAction *showWireframeAction;
    QSlider *horizontalScaleSlider;
    QSlider *verticalScaleSlider;
    // terrain material editor
    TerrainMaterialEditor *terrainMaterialEditor;

    // === MATERIAL EDITOR === //

    QDockWidget *materialEditorDockWidget;
    QScrollArea *materialEditorScrollArea;
    MaterialEditor *materialEditor;



};

#endif // VIEW_H
