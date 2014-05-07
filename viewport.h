#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <camera.h>
#include <model.h>

class Viewport : public QGLWidget
{
    Q_OBJECT
public:
    explicit Viewport(QWidget *parent = 0, Model::ViewportType type = Model::PERSPECTIVE);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setCamera(Camera *camera);

signals:

public slots:


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    /*void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);*/

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
    Model::ViewportType type_;

};

#endif // VIEWPORT_H
