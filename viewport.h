#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
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
    explicit Viewport(QWidget *parent = 0, Model::ViewportType type = Model::PERSPECTIVE, Model *model = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setCamera(Camera *camera);
    Model::ViewportType getType();


signals:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void setActivePrimitive(float id);

public slots:
    void setClickedId(int x, int y);
    void copyVAOData(Primitive *p);
    void copyVolumeData(Volume *volume);
    void copyVolumeData();
    void updateProjectionMatrix();
    void showGrid(bool on);
    void setGridSize(int i);
    void setStepSize(int i);
    void setMip(bool on);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:

    // shader that uses phong shading on the object
    QGLShaderProgram *phongProgram;
    QGLShader *phongVertexShader;
    QGLShader *phongFragmentShader;

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

    // shader inputs
    GLuint phongIdID_;
    GLuint phongColorID_;

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

    float light0Position_[4];

    bool showGrid_;
    bool mip_;

    Cube *cube_;
    Torus *torus_;
    Sphere *sphere_;
    Cone *cone_;
    Cylinder *cylinder_;
    Grid *grid_;
    Quad *quad_;

    int gridSize_;
    int stepSize_;

    bool checkFramebufferStatus();
};

#endif // VIEWPORT_H
