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
#include <volume.h>
#include <terrain.h>

inline void glMultMatrix(const GLfloat* matrix) { glMultMatrixf(matrix); }
inline void glMultMatrix(const GLdouble* matrix) { glMultMatrixd(matrix); }
inline void glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLfloat *matrix) {
    glUniformMatrix4fv(location, count, transpose, matrix); }
inline void glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLdouble *matrix) {
    glUniformMatrix4dv(location, count, transpose, matrix); }

Viewport::Viewport(QWidget *parent, QGLFormat format, Model::ViewportType type, Model *model) :
    QGLWidget(format, parent)
    //QGLWidget(parent)
{
    type_ = type;
    model_ = model;

    showGrid_ = true;
    mip_ = false;
    showWireframe_ = false;

    gridSize_ = 5;
    stepSize_ = 1;

    materialTextureIDs_.resize(0);

    // set up default Camera
    camera_ = new Camera();
    grid_ = new Grid(Primitive::float3(0.72, 0.72, 0.72), gridSize_, stepSize_);
    quad_ = new Quad();
}


QSize Viewport::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Viewport::sizeHint() const
{
    return QSize(800, 800);
}

void Viewport::initializeGL()
{

    checkGLErrors("initialize...");

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    checkGLErrors("glew");

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // enable 3D textures
    glEnable(GL_TEXTURE_3D_EXT);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    light0_[0] = QVector4D(0, 20, 0, 1);    // position
    light0_[1] = QVector4D(0.3, 0.3, 0.3, 1.0);  // ambient
    light0_[2] = QVector4D(1, 1, 1, 1);    // diffuse
    light0_[3] = QVector4D(1, 1, 1, 1);    // specular

    // Assign created components to GL_LIGHT0
    float* tmp = (float*) malloc(sizeof(float) * 4);
    tmp[0] = light0_[1].x(); tmp[1] = light0_[1].y(); tmp[2] = light0_[1].z(); tmp[3] = light0_[1].w();
    glLightfv(GL_LIGHT0, GL_AMBIENT, tmp);
    tmp[0] = light0_[2].x(); tmp[1] = light0_[2].y(); tmp[2] = light0_[2].z(); tmp[3] = light0_[2].w();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, tmp);
    tmp[0] = light0_[3].x(); tmp[1] = light0_[3].y(); tmp[2] = light0_[3].z(); tmp[3] = light0_[3].w();
    glLightfv(GL_LIGHT0, GL_SPECULAR, tmp);
    free(tmp);

    // TODO Remove old light stuff

    // position lightsource
    light0Position_[0] = 0.0f;
    light0Position_[1] = 20.0f;
    light0Position_[2] = 0.0f;
    light0Position_[3] = 1.0f;

    // color lightsource
    GLfloat light0Ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat light0Diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light0Specular[4] = {0.05f, 0.05f, 0.05f, 1.0f};

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);



    float specularReflection[4] = {1.0, 1.0, 1.0, 1.0};
    int shininess = 120.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // set clear color
    glClearColor(0, 0.5, 0.5, 1);

    // set patch vertices for tesselation shader
    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    std::cout << "Max supported patch vertices: " << maxPatchVertices << std::endl;
    glPatchParameteri(GL_PATCH_VERTICES, 4);    // we want to tesselate quads


    checkGLErrors("before creating framebuffer");


    // ==== CREATE FRAMEBUFFER AND ITS TEXTURES ==== //

    int width = this->width();
    int height = this->height();
    if (width == 0) width = 1;
    if (height == 0) height = 1;

    glViewport(0, 0, width, height);

    // create a texture for the colors
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_FLOAT, 0);

    // create feedback height texture
    glGenTextures(1, &feedbackHeightTexture_);
    glBindTexture(GL_TEXTURE_2D, feedbackHeightTexture_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);

    emit transmitFeedbackHeightTextureID(feedbackHeightTexture_);

    // create a texture for the object IDs
    glGenTextures(1, &idTexture_);
    glBindTexture(GL_TEXTURE_2D, idTexture_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a renderbuffer object to store depth info - openGL needs this for depth test
    glGenRenderbuffersEXT(1, &depthTexture_);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthTexture_);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
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

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                              GL_COLOR_ATTACHMENT2_EXT,
                              GL_TEXTURE_2D,
                              feedbackHeightTexture_,
                              0);

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

    checkGLErrors("before shaders");

    // compile and link shaders
    // PHONG SHADER
    shadingProgram = new QGLShaderProgram(this);

    shadingVertexShader = new QGLShader(QGLShader::Vertex, this);
    shadingFragmentShader = new QGLShader(QGLShader::Fragment, this);

    shadingVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/phongVertexShader.vertexShader");
    shadingFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/phongFragmentShader.fragmentShader");

    shadingProgram->addShader(shadingVertexShader);
    shadingProgram->addShader(shadingFragmentShader);


    glBindAttribLocation(shadingProgram->programId(), 1, "normal_in");
    glBindAttribLocation(shadingProgram->programId(), 3, "tex_in");
    glBindAttribLocation(shadingProgram->programId(), 4, "tangent_in");
    glBindAttribLocation(shadingProgram->programId(), 5, "bitangent_in");

    shadingProgram->link();

    // SELECTION SHADER

    selectionProgram = new QGLShaderProgram(this);
    selectionVertexShader = new QGLShader(QGLShader::Vertex, this);
    selectionFragmentShader = new QGLShader(QGLShader::Fragment, this);

    selectionVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/selectionVertexShader.vertexShader");
    selectionFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/selectionFragmentShader.fragmentShader");


    selectionProgram->addShader(selectionVertexShader);
    selectionProgram->addShader(selectionFragmentShader);

    glBindAttribLocation(selectionProgram->programId(), 3, "tex_in");

    selectionProgram->link();

    // GRID SHADER

    gridProgram = new QGLShaderProgram(this);
    gridVertexShader = new QGLShader(QGLShader::Vertex, this);
    gridFragmentShader = new QGLShader(QGLShader::Fragment, this);

    gridVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/gridVertexShader.vertexShader");
    gridFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/gridFragmentShader.fragmentShader");

    gridProgram->addShader(gridVertexShader);
    gridProgram->addShader(gridFragmentShader);
    gridProgram->link();

    // VOLUME SHADER

    volumeProgram = new QGLShaderProgram(this);
    volumeVertexShader = new QGLShader(QGLShader::Vertex, this);
    volumeFragmentShader = new QGLShader(QGLShader::Fragment, this);

    volumeVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/volumeVertexShader.vertexShader");
    volumeFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/volumeFragmentShader.fragmentShader");


    volumeProgram->addShader(volumeVertexShader);
    volumeProgram->addShader(volumeFragmentShader);
    volumeProgram->link();

    // TERRAIN SHADER


    terrainProgram = new QOpenGLShaderProgram(this);
    terrainVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    terrainTesselationControlShader = new QOpenGLShader(QOpenGLShader::TessellationControl, this);
    terrainTesselationEvaluationShader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation, this);
    terrainFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);

/* LAPTOP COMPATIBILITY
    terrainProgram = new QGLShaderProgram(this);
    terrainVertexShader = new QGLShader(QGLShader::Vertex, this);
    terrainFragmentShader = new QGLShader(QGLShader::Fragment, this);
*/
    terrainVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/terrainVertexShader.vertexShader");
    terrainTesselationControlShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/terrainTCS.tesselationControlShader");
    terrainTesselationEvaluationShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/terrainTES.tesselationEvaluationShader");
    terrainFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/terrainFragmentShader.fragmentShader");


    terrainProgram->addShader(terrainVertexShader);
    terrainProgram->addShader(terrainTesselationControlShader);
    terrainProgram->addShader(terrainTesselationEvaluationShader);
    terrainProgram->addShader(terrainFragmentShader);
    terrainProgram->link();

    // distribute uniform IDs

    shadingIdID_ = glGetUniformLocation(shadingProgram->programId(), "id");
    //shadingColorID_ = glGetUniformLocation(shadingProgram->programId(), "color");
    ambientColorID_ = glGetUniformLocation(shadingProgram->programId(), "ambientColor_in");
    diffuseColorID_ = glGetUniformLocation(shadingProgram->programId(), "diffuseColor_in");
    specularColorID_ = glGetUniformLocation(shadingProgram->programId(), "specularColor_in");
    roughnessID_ = glGetUniformLocation(shadingProgram->programId(), "roughness_in");
    refractionIndexID_ = glGetUniformLocation(shadingProgram->programId(), "refractionIndex_in");
    textureIDs_ = glGetUniformLocation(shadingProgram->programId(), "texture");
    textureActiveIDs_ = glGetUniformLocation(shadingProgram->programId(), "textureActive");
    nrTexturesID_ = glGetUniformLocation(shadingProgram->programId(), "nrTextures");
    diffuseShaderID_ = glGetUniformLocation(shadingProgram->programId(), "diffuseShader");
    specularShaderID_ = glGetUniformLocation(shadingProgram->programId(), "specularShader");
    kaID_ = glGetUniformLocation(shadingProgram->programId(), "ka_in");
    kdID_ = glGetUniformLocation(shadingProgram->programId(), "kd_in");
    ksID_ = glGetUniformLocation(shadingProgram->programId(), "ks_in");



    idTextureID_ = glGetUniformLocation(selectionProgram->programId(), "idTexture");
    colorTextureID_ = glGetUniformLocation(selectionProgram->programId(), "colorTexture");
    selectionIdID_ = glGetUniformLocation(selectionProgram->programId(), "id");
    offsetXID_ = glGetUniformLocation(selectionProgram->programId(), "offsetX");
    offsetYID_ = glGetUniformLocation(selectionProgram->programId(), "offsetY");
    activeViewportID_ = glGetUniformLocation(selectionProgram->programId(), "active");

    gridColorID_ = glGetUniformLocation(gridProgram->programId(), "color");

    volumeIdID_ = glGetUniformLocation(volumeProgram->programId(), "id");
    volumeTextureID_ = glGetUniformLocation(volumeProgram->programId(), "volumeTexture");
    transferTextureID_ = glGetUniformLocation(volumeProgram->programId(), "transferTexture");
    aspectRatioID_ = glGetUniformLocation(volumeProgram->programId(), "aspectRatio");
    mipID_ = glGetUniformLocation(volumeProgram->programId(), "mip");
    maxResolutionID_ = glGetUniformLocation(volumeProgram->programId(), "maxResolution");

    terrainIdID_ = glGetUniformLocation(terrainProgram->programId(), "id");
    modelMatrixID_ = glGetUniformLocation(terrainProgram->programId(), "modelMatrix");
    viewProjectionMatrixID_ = glGetUniformLocation(terrainProgram->programId(), "viewProjectionMatrix");
    cameraPositionID_ = glGetUniformLocation(terrainProgram->programId(), "camPosition_WS");
    cameraTexCoordID_ = glGetUniformLocation(terrainProgram->programId(), "camTexCoord_Terrain");
    heightMapID_ = glGetUniformLocation(terrainProgram->programId(), "heightMap");
    gridFractionID_ = glGetUniformLocation(terrainProgram->programId(), "gridFraction");
    verticalScalingID_ = glGetUniformLocation(terrainProgram->programId(), "verticalScaling");
    gridSizeID_ = glGetUniformLocation(terrainProgram->programId(), "nrQuads");
    nrMaterialsID_ = glGetUniformLocation(terrainProgram->programId(), "nrMaterials");
    material0ID_ = glGetUniformLocation(terrainProgram->programId(), "material0");
    material1ID_ = glGetUniformLocation(terrainProgram->programId(), "material1");
    material2ID_ = glGetUniformLocation(terrainProgram->programId(), "material2");
    material3ID_ = glGetUniformLocation(terrainProgram->programId(), "material3");
    light0ID_ = glGetUniformLocation(terrainProgram->programId(), "light0");
    wireframeID_ = glGetUniformLocation(terrainProgram->programId(), "wireframe");





    gridBufferIDs_ = createPrimitiveBufferIDs(static_cast<Primitive*>(grid_));
    grid_->copyVAOToCurrentContext(gridBufferIDs_);



    quadBufferIDs_ = createPrimitiveBufferIDs(static_cast<Primitive*>(quad_));
    quad_->copyVAOToCurrentContext(quadBufferIDs_);

    checkGLErrors("after shaders");

}

void Viewport::paintGL()
{

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer_);
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2,  attachments);


    checkGLErrors("before glClear");

    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    checkGLErrors("before modelview");

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrix(camera_->getCameraMatrix().constData());



    if (showGrid_) {

        glDisable(GL_DEPTH_TEST);
        gridProgram->bind();
        glUniform3f(gridColorID_, grid_->getColor()->x_, grid_->getColor()->y_, grid_->getColor()->z_);
        grid_->draw(gridBufferIDs_);
        gridProgram->release();
        glEnable(GL_DEPTH_TEST);
    }


    // DRAW PRIMITIVES

    checkGLErrors("before phong");
    shadingProgram->bind();

    checkGLErrors("after phong");

    float* tmp = (float*) malloc(sizeof(float) * 4);
    tmp[0] = light0_[0].x(); tmp[1] = light0_[0].y(); tmp[2] = light0_[0].z(); tmp[3] = light0_[0].w();
    glLightfv(GL_LIGHT0, GL_POSITION, tmp);
    free(tmp);


    glLightfv(GL_LIGHT0, GL_POSITION, light0Position_);


    QList<Primitive*> *primitives = model_->getScenegraph();


    // draw all primitives that are not a volume
    for (int i = 0; i < primitives->size(); i++) {
        Primitive *p = primitives->at(i);
        if (!p->isVolume() && !p->isTerrain()) {
            glUniform1f(shadingIdID_, p->getID());
            //glUniform3f(shadingColorID_, primitives->at(i)->getColor()->x_, primitives->at(i)->getColor()->y_, primitives->at(i)->getColor()->z_);
            glUniform3f(ambientColorID_, p->getAmbientColor().x_, p->getAmbientColor().y_, p->getAmbientColor().z_);
            glUniform3f(diffuseColorID_, p->getDiffuseColor().x_, p->getDiffuseColor().y_, p->getDiffuseColor().z_);
            glUniform3f(specularColorID_, p->getSpecularColor().x_, p->getSpecularColor().y_, p->getSpecularColor().z_);
            glUniform1f(kaID_, p->getKa());
            glUniform1f(kdID_, p->getKd());
            glUniform1f(ksID_, p->getKs());
            glUniform1f(roughnessID_, p->getRoughness());
            glUniform1f(refractionIndexID_, p->getRefractionIndex());

            int *textureIDs = (int*) malloc(sizeof(int) * Primitive::NR_TEXTURES);
            int *textureActiveIDs = (int*) malloc(sizeof(int) * Primitive::NR_TEXTURES);
            for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
                textureIDs[i] = 10 + i;
                if (p->isTextureActive(static_cast<Primitive::Textures>(i))) {
                    textureActiveIDs[i] = 1;
                } else {
                    textureActiveIDs[i] = 0;
                }

                if (primitiveBufferIDs_[p->getID()].hasTextures_[i]) {
                    glActiveTexture(GL_TEXTURE10 + i);
                    glBindTexture(GL_TEXTURE_2D, primitiveBufferIDs_[p->getID()].textures_[i]);
                } else {
                    glActiveTexture(GL_TEXTURE10 + i);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    textureActiveIDs[i] = 0;
                }
            }
            glActiveTexture(GL_TEXTURE0);
            checkGLErrors("binding textures to master shader");

            glUniform1iv(textureIDs_, Primitive::NR_TEXTURES, textureIDs);
            glUniform1iv(textureActiveIDs_, Primitive::NR_TEXTURES, textureActiveIDs);
            glUniform1i(nrTexturesID_, Primitive::NR_TEXTURES);

            glUniform1i(diffuseShaderID_, p->getDiffuseShader());
            glUniform1i(specularShaderID_, p->getSpecularShader());

            glPushMatrix();
            glMultMatrix(p->getModelMatrix().constData());

            p->draw(primitiveBufferIDs_[p->getID()]);
            shadingProgram->release();
            p->debugTangentSpace(false, false, false);
            shadingProgram->bind();
            glPopMatrix();


        }
    }

    checkGLErrors("before volume drawing loop");

    // DRAW VOLUMES

    shadingProgram->release();

/*    volumeProgram->bind();

    for (int i = 0; i < primitives->size(); i++) {
        if (primitives->at(i)->isVolume()) {
            Volume* volume = static_cast<Volume*>(primitives->at(i));
            glUniform1f(volumeIdID_, primitives->at(i)->getID());
            glUniform1i(mipID_, mip_);
            glUniform1i(maxResolutionID_, volume->getMaxResolution());
            // volume texture will be in GL_TEXTURE1. It is bound in the volume's draw method.
            glUniform1i(volumeTextureID_, 1);
            // transfer texture will be in GL_TEXTURE2. It is bound in the volume's draw method.
            glUniform1i(transferTextureID_, 2);
            glUniform3f(aspectRatioID_, volume->getAspectRatio().x_, volume->getAspectRatio().y_, volume->getAspectRatio().z_);

            glPushMatrix();
            glMultMatrix(primitives->at(i)->getModelMatrix().constData());

            primitives->at(i)->draw(primitiveBufferIDs_[primitives->at(i)->getID()]);

            glPopMatrix();
        }
    }

    checkGLErrors("before terrain drawing loop");

    volumeProgram->release();

    // DRAW TERRAINS

    terrainProgram->bind();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glMultMatrix(camera_->getRotatedOnSpotCameraMatrix().constData());
    glMultMatrix(camera_->getCameraMatrix().constData());

    for (int i = 0; i < primitives->size(); i++) {
        if (primitives->at(i)->isTerrain()) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, heightMapTextureID_);

            for (uint j = 0; j < materialTextureIDs_.size(); j++) {
                glActiveTexture(GL_TEXTURE2 + j);
                glBindTexture(GL_TEXTURE_2D, materialTextureIDs_[j]);
                std::cout << "material texture " << j << " ID (viewport " << type_ << "): " << materialTextureIDs_[j] << std::endl;
            }
            glActiveTexture(GL_TEXTURE0);

            // send terrain id and height map to shader ================================================
            glUniform1f(terrainIdID_, primitives->at(i)->getID());
            glUniform1i(heightMapID_, 1);
            float* tmp = (float*) malloc(sizeof(float) * 16);
            for (int j = 0; j < 4; j++) {
                tmp[(j*4)] = light0_[j].x();
                tmp[(j*4)+1] = light0_[j].y();
                tmp[(j*4)+2] = light0_[j].z();
                tmp[(j*4)+3] = light0_[j].w();
            }
            glUniform4fv(light0ID_, 4, tmp);
            free(tmp);

            glUniform1i(nrMaterialsID_, materialTextureIDs_.size());
            glUniform1i(material0ID_, 2);
            glUniform1i(material1ID_, 3);
            glUniform1i(material2ID_, 4);
            glUniform1i(material3ID_, 5);
            glUniform1i(wireframeID_, 0);

            checkGLErrors("after sending uniforms for materials");

            // SWITCH DOUBLE <-> FLOAT FOR DIFFERENT ARCHITECTURE

            float* floatMatrix = (float*) malloc(16 * sizeof(float));
            double* doubleMatrix = (double*) malloc(16 * sizeof(double));

            floatMatrix = (float*) primitives->at(i)->getModelMatrix().constData();
            //for (int j = 0; j < 16; j++) { floatMatrix[j] = (float) doubleMatrix[j]; }

            // send model matrix to shader =============================================================
            glUniformMatrix4fv(modelMatrixID_, 1, GL_FALSE, floatMatrix);

            glGetFloatv(GL_MODELVIEW_MATRIX, floatMatrix);   // column-major
            //for (int j = 0; j < 16; j++) { doubleMatrix[j] = (double) floatMatrix[j]; }

            // extract camera position and compute camera texture coordinates
            QMatrix4x4 viewMatrix = QMatrix4x4(floatMatrix).transposed(); // convert column-major to row-major
            QVector4D cameraPosition = viewMatrix.inverted().column(3);
            glUniform3f(cameraPositionID_, cameraPosition.x(), cameraPosition.y(), cameraPosition.z());



            float horizontalScale = static_cast<Terrain*>(primitives->at(i))->getHorizontalScale();
            int verticalScale = static_cast<Terrain*>(primitives->at(i))->getVerticalScale();
            glUniform1i(verticalScalingID_, verticalScale);

            int gridSize = static_cast<Terrain*>(primitives->at(i))->getGridSize();



            // how big is the grid with regard to the size of the terrain. Grid's local texture coordinates
            // need to be downscaled by this factor.
            float gridFraction = gridSize / horizontalScale;
            glUniform1f(gridFractionID_, gridFraction);
            glUniform1i(gridSizeID_, gridSize);

            QVector2D cameraTexCoord = QVector2D((cameraPosition.x() + horizontalScale / 2.0f) / horizontalScale,
                                                ((-cameraPosition.z() + horizontalScale / 2.0f) / horizontalScale));


            if (type_ == 0) {
                //std::cout << "cameraTexCoord: (" << cameraTexCoord.x() << ", " << cameraTexCoord.y() << ")" << std::endl;
                //std::cout << "cameraPosition: (" << cameraPosition.x() << ", " << cameraPosition.y() << ", " << cameraPosition.z() << ")" << std::endl;
            }
            // send camera tex coords to shader ========================================================
            glUniform2f(cameraTexCoordID_, cameraTexCoord.x(), cameraTexCoord.y());

            glGetFloatv(GL_PROJECTION_MATRIX, floatMatrix);  // column-major
            //for (int j = 0; j < 16; j++) { doubleMatrix[j] = (double) floatMatrix[j]; }

            QMatrix4x4 projectionMatrix = QMatrix4x4(floatMatrix).transposed() * QMatrix4x4(viewMatrix);  // row-major
            floatMatrix = (float*) projectionMatrix.constData();
            //for (int j = 0; j < 16; j++) { floatMatrix[j] = (float) doubleMatrix[j]; }

            // send projection matrix to shader ========================================================
            glUniformMatrix4fv(viewProjectionMatrixID_, 1, GL_FALSE, floatMatrix);

            // draw terrain
            primitives->at(i)->draw(primitiveBufferIDs_[primitives->at(i)->getID()]);

            //free(floatMatrix);

            if (showWireframe_) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glUniform1i(wireframeID_, 1);
                primitives->at(i)->draw(primitiveBufferIDs_[primitives->at(i)->getID()]);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }

    checkGLErrors("after terrain drawing loop");

    // HIGHLIGHT SELECTED OBJECT

    terrainProgram->release();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    */

    selectionProgram->bind();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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


    glUniform1f(selectionIdID_, id);
    glUniform1f(offsetXID_, 1.0f / width());
    glUniform1f(offsetYID_, 1.0f / height());
    glUniform1i(activeViewportID_, viewportActive);

    quad_->draw(quadBufferIDs_);


    selectionProgram->release();

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);


}


void Viewport::resizeGL(int width, int height)
{
    //if (width == 0) width = 1;
    //if (height == 0) height = 1;
/*
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0_EXT,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           0,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT1_EXT,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           0,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,      // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_ATTACHMENT_EXT, // 2. attachment point
                              GL_RENDERBUFFER_EXT,     // 3. rbo target: GL_RENDERBUFFER
                              0);     // 4. rbo ID
*/
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, idTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, feedbackHeightTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthTexture_);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
/*
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
*/
    checkFramebufferStatus();

    glViewport(0, 0, width, height);

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    if (camera_->getProjectionMode() == Camera::PERSPECTIVE) {
        if (height != 0) gluPerspective(45.0d, ((double) width) / ((double) height), NEAR_PLANE, FAR_PLANE);
    } else if (camera_->getProjectionMode() == Camera::ORTHOGRAPHIC) {
        updateProjectionMatrix();
    }

    checkGLErrors("after setting projection matrices");
}


// ====================== SETTERS ========================= //

void Viewport::setCamera(Camera *camera) {
    camera_ = camera;
    connect(camera_, SIGNAL(zoomChanged()), this, SLOT(updateProjectionMatrix()));
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

Primitive::bufferIDs Viewport::createPrimitiveBufferIDs(Primitive* p) {
    GLuint VAO, positions, indices, normals, colors, texCoords, tangents, bitangents;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &positions);
    glGenBuffers(1, &indices);

    if (p->hasVBO(Primitive::NORMALS)) {
        glGenBuffers(1, &normals);
    }
    if (p->hasVBO(Primitive::COLORS)) {
        glGenBuffers(1, &colors);
    }
    if (p->hasVBO(Primitive::TEXCOORDS)) {
        glGenBuffers(1, &texCoords);
    }
    if (p->hasVBO(Primitive::TANGENTS)) {
        glGenBuffers(1, &tangents);
    }
    if (p->hasVBO(Primitive::BITANGENTS)) {
        glGenBuffers(1, &bitangents);
    }

    Primitive::bufferIDs buffIDs = Primitive::bufferIDs(VAO, positions, indices, normals, colors, texCoords, tangents, bitangents,
                                                        p->hasVBO(Primitive::NORMALS), p->hasVBO(Primitive::COLORS),
                                                        p->hasVBO(Primitive::TEXCOORDS), p->hasVBO(Primitive::TANGENTS), p->hasVBO(Primitive::BITANGENTS));
    return buffIDs;
}

void Viewport::uploadTerrainMaterialData(Terrain *t, QString fileName) {
    makeCurrent();
    GLuint index;
    glGenTextures(1, &index);
    materialTextureIDs_.push_back(index);
    t->uploadMaterial(fileName, index);
    updateGL();
}

void Viewport::copyTextureData(Primitive *p, Primitive::Textures x) {
    makeCurrent();
    int primitiveID = p->getID();

    glGenTextures(1, &primitiveBufferIDs_[primitiveID].textures_[x]);
    std::cout << "GLuint ID for Primitive " << p->getID() << " for texture " << x << ": " << primitiveBufferIDs_[primitiveID].textures_[x] << std::endl;
    p->copyTextureToCurrentContext(primitiveBufferIDs_[primitiveID].textures_[x], x);
    primitiveBufferIDs_[primitiveID].hasTextures_[x] = true;
}

void Viewport::copyVAOData(Primitive *p) {
    makeCurrent();
    Primitive::bufferIDs buffIDs = createPrimitiveBufferIDs(p);

    int primitiveID = p->getID();
    while (primitiveID >= primitiveBufferIDs_.size()) {
        primitiveBufferIDs_.push_back(Primitive::bufferIDs());
    }
    primitiveBufferIDs_[primitiveID] = buffIDs;

    p->copyVAOToCurrentContext(primitiveBufferIDs_[p->getID()]);
    if (p->isTerrain()) {
        glGenBuffers(1, &heightMapTextureID_);
        static_cast<Terrain*>(p)->createTextures(heightMapTextureID_);
    }
}

void Viewport::copyVolumeData(Volume *volume) {
    if (volume != NULL) {
        makeCurrent();
        volume->uploadTransferFunction();
    }
    updateGL();

}

void Viewport::copyVolumeData() {
    QList<Primitive*> *primitives = model_->getScenegraph();
    for (int i = 0; i < primitives->size() && primitives->at(i)->isVolume(); i++) {
        makeCurrent();
        static_cast<Volume*>(primitives->at(i))->uploadTransferFunction();
    }
    updateGL();
}

void Viewport::updateProjectionMatrix() {

    if (camera_->getProjectionMode() == Camera::ORTHOGRAPHIC) {

        makeCurrent();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float zoom = camera_->getZoom();
        float aspectRatio = 1;
        if (height() != 0) aspectRatio = (float) width() / height();
        float goodZoomFactor = zoom / 10;

        glOrtho(- aspectRatio * (1 - goodZoomFactor), aspectRatio * (1 - goodZoomFactor), -1 + goodZoomFactor, 1 - goodZoomFactor, NEAR_PLANE, FAR_PLANE);

        checkGLErrors("glOrtho (ignore)");
    }
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

void Viewport::showGrid(bool on) {
    showGrid_ = on;

    updateGL();
}

void Viewport::setMip(bool on) {
    mip_ = on;

    updateGL();
}

void Viewport::showWireframe(bool on) {
    showWireframe_ = on;

    updateGL();
}

void Viewport::setGridSize(int i) {
    makeCurrent();
    grid_->setGridSize(i);
    grid_->copyVAOToCurrentContext(gridBufferIDs_);
    updateGL();

}

void Viewport::setStepSize(int i) {
    makeCurrent();
    grid_->setStepSize(i);
    grid_->copyVAOToCurrentContext(gridBufferIDs_);
    updateGL();

}



void Viewport::checkGLErrors(const char *label) {
    GLenum errCode;
    const GLubyte *errStr;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errStr = gluErrorString(errCode);
        printf("OpenGL ERROR: ");
        printf((char*)errStr);
        printf("(Label: ");
        printf(label);
        printf(")\n.");
    }
}
