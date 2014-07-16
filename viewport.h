#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <camera.h>
#include <model.h>
#include <cube.h>
#include <torus.h>
#include <sphere.h>
#include <cone.h>
#include <cylinder.h>
#include <grid.h>
#include <quad.h>
#include <volume.h>

class Viewport : public QGLWidget
{
    Q_OBJECT
public:


    explicit Viewport(QWidget *parent = 0, QGLFormat format = QGLFormat(), Model::ViewportType type = Model::PERSPECTIVE, Model *model = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setCamera(Camera *camera);
    Model::ViewportType getType();


signals:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void setActivePrimitive(float id);
    void transmitFeedbackHeightTextureID(GLuint id);

public slots:
    void setClickedId(int x, int y);
    void uploadTerrainMaterialData(Terrain *t, QString fileName);
    void copyVAOData(Primitive *p);
    void copyVolumeData(Volume *volume);
    void copyVolumeData();
    void copyTextureData(Primitive *p, Primitive::Textures x);
    void updateProjectionMatrix();
    void showGrid(bool on);
    void setGridSize(int i);
    void setStepSize(int i);
    void setMip(bool on);
    void showWireframe(bool on);
    Primitive::bufferIDs createPrimitiveBufferIDs(Primitive* p);
    void setDiffuseShader(int id);
    void setSpecularShader(int id);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    static const double NEAR_PLANE = 0.01d;
    static const double FAR_PLANE = 100.0d;

    // master shader that uses the selected shading on the object
    QGLShaderProgram *shadingProgram;
    QGLShader *shadingVertexShader;
    QGLShader *shadingFragmentShader;

    // shader that colors the selected object
    QGLShaderProgram *selectionProgram;
    QGLShader *selectionVertexShader;
    QGLShader *selectionFragmentShader;

    // shader that colors the grid (no shading, no selection)
    QGLShaderProgram *gridProgram;
    QGLShader *gridVertexShader;
    QGLShader *gridFragmentShader;

    // shader that colors the volumes
    QGLShaderProgram *volumeProgram;
    QGLShader *volumeVertexShader;
    QGLShader *volumeFragmentShader;

    // shader that colors the terrains

    QOpenGLShaderProgram *terrainProgram;
    QOpenGLShader *terrainVertexShader;
    QOpenGLShader *terrainTesselationControlShader;
    QOpenGLShader *terrainTesselationEvaluationShader;
    QOpenGLShader *terrainFragmentShader;

/*
    QGLShaderProgram *terrainProgram;
    QGLShader *terrainVertexShader;
    QGLShader *terrainTesselationControlShader;
    QGLShader *terrainTesselationEvaluationShader;
    QGLShader *terrainFragmentShader;
*/

    // vertices of cube
    std::vector<std::vector<float> > originalVertices;

    // Camera
    Camera *camera_;
    Model *model_;
    Model::ViewportType type_;

    // Framebuffer textures
    GLuint framebuffer_;
    GLuint colorTexture_;
    GLuint idTexture_;
    GLuint depthTexture_;
    GLuint feedbackHeightTexture_;

    // shader inputs
    GLuint shadingIdID_;
    GLuint shadingColorID_;
    GLuint shadingDiffuseTextureID_;
    GLuint shadingDiffuseShaderID_;
    GLuint shadingSpecularShaderID_;

    GLuint selectionIdID_;
    GLuint idTextureID_;
    GLuint colorTextureID_;
    GLuint offsetXID_;
    GLuint offsetYID_;
    GLuint activeViewportID_;

    GLuint gridColorID_;

    GLuint volumeIdID_;
    GLuint volumeTextureID_;
    GLuint transferTextureID_;
    GLuint aspectRatioID_;
    GLuint mipID_;
    GLuint maxResolutionID_;

    GLuint terrainIdID_;
    GLuint modelMatrixID_;
    GLuint viewProjectionMatrixID_;
    GLuint cameraPositionID_;
    GLuint cameraTexCoordID_;
    GLuint heightMapID_;
    GLuint gridFractionID_;
    GLuint verticalScalingID_;
    GLuint gridSizeID_;
    GLuint material0ID_;
    GLuint material1ID_;
    GLuint material2ID_;
    GLuint material3ID_;
    GLuint nrMaterialsID_;
    GLuint light0ID_;
    GLuint wireframeID_;

    QVector4D light0_[4];

    bool showGrid_;
    bool mip_;
    bool showWireframe_;

    Cube *cube_;
    Torus *torus_;
    Sphere *sphere_;
    Cone *cone_;
    Cylinder *cylinder_;
    Grid *grid_;
    Quad *quad_;

    int gridSize_;
    int stepSize_;

    std::vector<Primitive::bufferIDs> primitiveBufferIDs_;
    Primitive::bufferIDs quadBufferIDs_;
    Primitive::bufferIDs gridBufferIDs_;

    GLuint heightMapTextureID_;
    std::vector<GLuint> materialTextureIDs_;

    bool checkFramebufferStatus();
    void checkGLErrors(const char *label);

    int diffuseShader_;
    int specularShader_;


    float light0Position_[4];
};

#endif // VIEWPORT_H
