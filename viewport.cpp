#include "viewport.h"
#include <camera.h>
#include "GL/glu.h"
#include <QMatrix4x4>
#include <model.h>


inline void glMultMatrix(const GLfloat* matrix) { glMultMatrixf(matrix); }
inline void glMultMatrix(const GLdouble* matrix) { glMultMatrixd(matrix); }

Viewport::Viewport(QWidget *parent, Model::ViewportType type) :
    QGLWidget(parent)
{
    type_ = type;

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

    // set up default Camera
    camera_ = new Camera();
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
    glEnable(GL_COLOR_MATERIAL);

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


    // get camera configuration
    QVector3D cameraPosition = camera_->getPosition();
    QVector3D cameraPOI = camera_->getPointOfInterest();
    QVector3D cameraUp = camera_->getUpVector();

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrix(camera_->getCameraMatrix().constData());
    gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
              cameraPOI.x(), cameraPOI.y(), cameraPOI.z(),
              cameraUp.x(), cameraUp.y(), cameraUp.z());


    // draw scene
    glBegin(GL_QUADS);

    for (uint i = 0; i < originalVertices.size(); i++) {
        if (i < 4) glColor3f(1,0,0);
        else if (i < 8) glColor3f(0,1,0);
        else if (i < 12) glColor3f(0,1,1);
        else if (i < 16) glColor3f(1,1,0);
        else if (i < 20) glColor3f(1,0,1);
        else if (i < 24) glColor3f(0,0,1);
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

    if (camera_->getProjectionMode() == Camera::PERSPECTIVE) {
        if (height != 0) gluPerspective(45.0d, ((double) width) / ((double) height), 0.01d, 10.0d);
    } else if (camera_->getProjectionMode() == Camera::ORTHOGRAPHIC) {
        if (height != 0) glOrtho(- (float) width/height, (float) width/height, -1, 1, 0.01, 10.0);
    }
}


// ====================== SETTERS ========================= //

void Viewport::setCamera(Camera *camera) {
    camera_ = camera;
}


