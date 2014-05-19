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
    glEnable(GL_COLOR_MATERIAL);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // position lightsource
    light0Position_[0] = 1.0f;
    light0Position_[1] = 1.0f;
    light0Position_[2] = 3.5f;
    light0Position_[3] = 1.0f;

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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_BGRA, GL_FLOAT, 0);

    // create a texture for the object IDs
    glGenTextures(1, &idTexture_);
    glBindTexture(GL_TEXTURE_2D, idTexture_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width(), height(), 0, GL_RED, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a renderbuffer object to store depth info - openGL needs this for depth test
    glGenRenderbuffersEXT(1, &depthTexture_);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthTexture_);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width(), height());
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    // create a framebuffer object
    glGenFramebuffersEXT(1, &framebuffer_);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_);

    // attach the textures to FBO color attachment points
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0_EXT,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           colorTexture_,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT1_EXT,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           idTexture_,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,      // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_ATTACHMENT_EXT, // 2. attachment point
                              GL_RENDERBUFFER_EXT,     // 3. rbo target: GL_RENDERBUFFER
                              depthTexture_);     // 4. rbo ID

    checkFramebufferStatus();
    // define the index array for the outputs
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2,  attachments);

    // ============================================================ //

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // compile and link shaders
    phongProgram = new QGLShaderProgram(this);

    phongVertexShader = new QGLShader(QGLShader::Vertex, this);
    phongFragmentShader = new QGLShader(QGLShader::Fragment, this);

    phongVertexShader->compileSourceFile(":/shaders/phongVertexShader.vertexShader");
    phongFragmentShader->compileSourceFile(":/shaders/phongFragmentShader.fragmentShader");

    phongProgram->addShader(phongVertexShader);
    phongProgram->addShader(phongFragmentShader);

    glBindAttribLocation(phongProgram->programId(), 1, "normal_in");
    //glBindAttribLocation(phongProgram->programId(), 2, "color_in");

    phongProgram->link();
//    std::cout << "1: " << glGetAttribLocation(phongProgram->programId(), "normal_in") << std::endl;
    //std::cout << "2: " << glGetAttribLocation(phongProgram->programId(), "color_in") << std::endl;

    selectionProgram = new QGLShaderProgram(this);
    selectionVertexShader = new QGLShader(QGLShader::Vertex, this);
    selectionFragmentShader = new QGLShader(QGLShader::Fragment, this);

    selectionVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 2/ModelingTool/shaders/selectionVertexShader.vertexShader");
    selectionFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 2/ModelingTool/shaders/selectionFragmentShader.fragmentShader");

    selectionProgram->addShader(selectionVertexShader);
    selectionProgram->addShader(selectionFragmentShader);
    selectionProgram->link();


    idPhongID_ = glGetUniformLocation(phongProgram->programId(), "id");
    colorID_ = glGetUniformLocation(phongProgram->programId(), "color");

    idTextureID_ = glGetUniformLocation(selectionProgram->programId(), "idTexture");
    colorTextureID_ = glGetUniformLocation(selectionProgram->programId(), "colorTexture");
    idSelectionID_ = glGetUniformLocation(selectionProgram->programId(), "id");
    offsetXID_ = glGetUniformLocation(selectionProgram->programId(), "offsetX");
    offsetYID_ = glGetUniformLocation(selectionProgram->programId(), "offsetY");
    activeViewportID_ = glGetUniformLocation(selectionProgram->programId(), "active");

    // attribute buffer 0: vertices
    glEnableVertexAttribArray(0);
    // attribute buffer 1: normals
    glEnableVertexAttribArray(1);
    // attribute buffer 2: colors
    glEnableVertexAttribArray(2);

    grid_ = new Grid("Grid", 0, 0, Primitive::float3(0.72, 0.72, 0.72));


}

void Viewport::paintGL()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_);
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2,  attachments);

    phongProgram->bind();

    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrix(camera_->getCameraMatrix().constData());

    glLightfv(GL_LIGHT0, GL_POSITION, light0Position_);



    grid_->copyVAOToCurrentContext();
    grid_->draw();

    QList<Primitive*> *primitives = model_->getScenegraph();

    for (int i = 0; i < primitives->size(); i++) {
        glUniform1f(idPhongID_, primitives->at(i)->getID());
        glUniform3f(colorID_, primitives->at(i)->getColor()->x_, primitives->at(i)->getColor()->y_, primitives->at(i)->getColor()->z_);

        glPushMatrix();
        glMultMatrix(primitives->at(i)->getModelMatrix().constData());

        primitives->at(i)->draw();

        glPopMatrix();
    }


    phongProgram->release();

    selectionProgram->bind();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint vertexArray;
    GLuint vertexBufferPositions;
    GLuint vertexBufferTextureCoords;
    GLuint indexBuffer;

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBufferPositions);
    glGenBuffers(1, &vertexBufferTextureCoords);

    glGenBuffers(1, &indexBuffer);

    std::vector<float> vertexPositions;
    vertexPositions.push_back(1);
    vertexPositions.push_back(1);
    vertexPositions.push_back(0);

    vertexPositions.push_back(-1);
    vertexPositions.push_back(1);
    vertexPositions.push_back(0);

    vertexPositions.push_back(-1);
    vertexPositions.push_back(-1);
    vertexPositions.push_back(0);

    vertexPositions.push_back(1);
    vertexPositions.push_back(-1);
    vertexPositions.push_back(0);

    std::vector<float> vertexTextureCoords;
    vertexTextureCoords.push_back(1);
    vertexTextureCoords.push_back(1);
    vertexTextureCoords.push_back(0);

    vertexTextureCoords.push_back(0);
    vertexTextureCoords.push_back(1);
    vertexTextureCoords.push_back(0);

    vertexTextureCoords.push_back(0);
    vertexTextureCoords.push_back(0);
    vertexTextureCoords.push_back(0);

    vertexTextureCoords.push_back(1);
    vertexTextureCoords.push_back(0);
    vertexTextureCoords.push_back(0);

    std::vector<int> indicesList;
    indicesList.push_back(0);
    indicesList.push_back(1);
    indicesList.push_back(2);
    indicesList.push_back(3);


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), &vertexPositions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoords);
    glBufferData(GL_ARRAY_BUFFER, vertexTextureCoords.size() *  sizeof(float), &vertexTextureCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList.size() * sizeof(uint), &indicesList[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTextureCoords);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glUniform1i(colorTextureID_, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, idTexture_);
    glUniform1i(idTextureID_, 2);

    float id;

    if (model_->getActivePrimitive() != NULL) {
        id = model_->getActivePrimitive()->getID();
    } else {
        id = 0;
    }

    bool viewportActive = model_->isActiveViewport(type_);

    glUniform1f(idSelectionID_, id);
    glUniform1f(offsetXID_, 1.0f / width());
    glUniform1f(offsetYID_, 1.0f / height());
    glUniform1i(activeViewportID_, viewportActive);



    glDrawElements(
        GL_QUADS,      // mode
        indicesList.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );

    selectionProgram->release();
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
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

void Viewport::setClickedId(int x, int y) {
    makeCurrent();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_);
    GLfloat pixels[4];
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, pixels);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    float id = pixels[0];

    // do not select background when in object interaction mode
    if (model_->getInteractionMode() != Model::OBJECT || id != 0) {
        emit setActivePrimitive(id);
    }

}


void Viewport::copyVAOData(Primitive *p) {
    makeCurrent();
    p->copyVAOToCurrentContext();
}


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
