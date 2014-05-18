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

    GLuint idPhongID_;
    GLuint idSelectionID_;
    GLuint idTextureID_;
    GLuint colorTextureID_;
    GLuint offsetXID_;
    GLuint offsetYID_;
    GLuint activeViewportID_;


    bool checkFramebufferStatus();

    Cube *cube_;
    Torus *torus_;
    Sphere *sphere_;
    Cone *cone_;
    Cylinder *cylinder_;
    Grid *grid_;


};

#endif // VIEWPORT_H
