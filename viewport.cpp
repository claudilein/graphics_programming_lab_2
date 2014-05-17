#include <GL/glew.h>
#include "viewport.h"
#include <camera.h>
#include "GL/glu.h"
#include <QMatrix4x4>
#include <model.h>
#include <iostream>
#include <cube.h>
#include <torus.h>
#include <cone.h>
#include <cylinder.h>
#include <grid.h>
#include <primitive.h>

inline void glMultMatrix(const GLfloat* matrix) { glMultMatrixf(matrix); }
inline void glMultMatrix(const GLdouble* matrix) { glMultMatrixd(matrix); }

Viewport::Viewport(QWidget *parent, Model::ViewportType type, Model *model) :
    QGLWidget(parent)
{
    type_ = type;
    model_ = model;



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
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // position lightsource
    float positionLight0[4] = {0.5f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);

    // set material properties for the cube
    float specularReflection[4] = {1.0, 1.0, 1.0, 1.0};
    int shininess = 120.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    // set clear color
    glClearColor(0, 0.5, 0.5, 1);



    // ==== CREATE FRAMEBUFFER AND ITS TEXTURES ==== //

    // create a texture for the colors
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_BGRA, GL_FLOAT, 0);

    // create a texture for the object IDs
    glGenTextures(1, &idTexture_);
    glBindTexture(GL_TEXTURE_2D, idTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width(), height(), 0, GL_RED, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a renderbuffer object to store depth info - openGL needs this for depth test
    glGenRenderbuffers(1, &depthTexture_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthTexture_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // create a framebuffer object
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    // attach the textures to FBO color attachment points
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           colorTexture_,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT1,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           idTexture_,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_ATTACHMENT, // 2. attachment point
                              GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                              depthTexture_);     // 4. rbo ID

    checkFramebufferStatus();
    // define the index array for the outputs
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2,  attachments);

    // ============================================================ //

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // compile and link shaders
    phongProgram = new QGLShaderProgram(this);

    phongVertexShader = new QGLShader(QGLShader::Vertex, this);
    phongFragmentShader = new QGLShader(QGLShader::Fragment, this);

    phongVertexShader->compileSourceFile(":/shaders/phongVertexShader.vertexShader");
    phongFragmentShader->compileSourceFile(":/shaders/phongFragmentShader.fragmentShader");

    phongProgram->addShader(phongVertexShader);
    phongProgram->addShader(phongFragmentShader);
    phongProgram->link();
    phongProgram->bind();


    //GLuint mvp;
    //mvp = glGetUniformLocation(phongProgram, "MVP");

    id_ID_ = glGetUniformLocation(phongProgram->programId(), "id");

    grid_ = new Grid("Grid", 0, 0);
    torus_ = new Torus("s", 0, 0, 0.5, 1.5, 10, 10);

}

void Viewport::paintGL()
{
    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (model_->getActive(type_))
    {
        std::cout << "active: " << type_ << std::endl;
    } else {
        std::cout << "inactive: " << type_ << std::endl;
    }


    // get camera configuration
    QVector3D cameraPosition = camera_->getPosition();
    QVector3D cameraPOI = camera_->getPointOfInterest();
    QVector3D cameraUp = camera_->getUpVector();

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
              cameraPOI.x(), cameraPOI.y(), cameraPOI.z(),
              cameraUp.x(), cameraUp.y(), cameraUp.z());
    glMultMatrix(camera_->getCameraMatrix().constData());


//    grid_->draw();


    QList<Primitive*> *primitives = model_->getScenegraph();

    for (int i = 0; i < primitives->size(); i++) {

        glUniform1f(id_ID_, primitives->at(i)->getID());
        primitives->at(i)->draw();
    }




//    cube_->draw();
    torus_->draw();






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

// ====================== GETTERS ========================= //

Model::ViewportType Viewport::getType() {
    return type_;
}

// ====================== SLOTS =========================== //


bool Viewport::checkFramebufferStatus() {
    GLenum status;
    status=(GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            printf("Framebuffer incomplete,incomplete attachment\n");
            return false;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            printf("Unsupported framebuffer format\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            printf("Framebuffer incomplete,missing attachment\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            printf("Framebuffer incomplete,attached images must have same dimensions\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
             printf("Framebuffer incomplete,attached images must have same format\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            printf("Framebuffer incomplete,missing draw buffer\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            printf("Framebuffer incomplete,missing read buffer\n");
            return false;
    }
    return false;
}
