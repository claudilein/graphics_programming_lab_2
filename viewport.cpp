#include "viewport.h"
#include <camera.h>
#include "GL/glu.h"
#include <QMatrix4x4>

Viewport::Viewport(QWidget *parent) :
    QGLWidget(parent)
{

    // set cube vertices
    float x = 0.5f;

    float position0[3] = {-x, -x, +x};
    float position1[3] = {+x, -x, +x};
    float position2[3] = {+x, -x, -x};
    float position3[3] = {-x, -x, -x};
    float position4[3] = {-x, +x, +x};
    float position5[3] = {+x, +x, +x};
    float position6[3] = {+x, +x, -x};
    float position7[3] = {-x, +x, -x};

    // front
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));

    // right
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));

    // back
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));

    // left
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));

    // bottom
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));

    // top
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));

    // set up Camera
    camera = new Camera(this, Camera::PERSPECTIVE, false);
}

QSize Viewport::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Viewport::sizeHint() const
{
    return QSize(400, 400);
}

void Viewport::initializeGL()
{
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_COLOR_MATERIAL);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // position lightsource
    float positionLight0[4] = {0.5f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);

    // set clear color
    glClearColor(0, 0.5, 0.5, 1);

}

void Viewport::paintGL()
{
    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(camera->getCameraMatrix().constData());



    glBegin(GL_QUADS);

    // draw all faces of the cube and set color and normal accordingly
    for (uint i = 0; i < originalVertices.size(); i++) {
        glColor3f(1,0,0);
        glVertex3f(originalVertices[i][0], originalVertices[i][1], originalVertices[i][2]);
    }

    glEnd();

}

void Viewport::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (camera->getProjectionMode() == Camera::PERSPECTIVE) {
        if (height != 0) gluPerspective(45.0d, ((double) width) / ((double) height), 0.01d, 10.0d);
        else gluPerspective(45.0d, 0.0d, 0.01d, 10.0d);
    } else if (camera->getProjectionMode() == Camera::ORTHOGRAPHIC) {
        glOrtho(-width / 2, width / 2, -height / 2, height / 2, 0.01, 10.0);
    }
}

void Viewport::mousePressEvent(QMouseEvent *event)
{

}

void Viewport::mouseMoveEvent(QMouseEvent *event)
{

}

void Viewport::wheelEvent(QWheelEvent *event)
{

}



