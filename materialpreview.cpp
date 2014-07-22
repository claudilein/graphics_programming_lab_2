#include <GL/glew.h>
#include "materialpreview.h"
#include "GL/glu.h"
#include <iostream>

inline void glMultMatrix(const GLfloat* matrix) { glMultMatrixf(matrix); }
inline void glMultMatrix(const GLdouble* matrix) { glMultMatrixd(matrix); }



MaterialPreview::MaterialPreview(QWidget *parent) :
    QGLWidget(parent)
{

    sphere_ = new Sphere("Preview Sphere", 0, 5, Primitive::float3(1, 1, 0), 1, 50, 50);
    camera_ = new Camera(0, Camera::PERSPECTIVE, false, QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 45.0f));
    camera_->zoom(7);
}



QSize MaterialPreview::minimumSizeHint() const
{
    return QSize(150, 150);
}

QSize MaterialPreview::sizeHint() const
{
    return QSize(150, 150);
}

void MaterialPreview::initializeGL()
{
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }
    checkGLErrors("glew");

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // set viewport
    glViewport(0, 0, width(), height());


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

    // set clear color
    glClearColor(0, 0.5, 0.5, 1);


    checkGLErrors("before shader");

    // compile and link shader
    shadingProgram = new QGLShaderProgram(this);

    shadingVertexShader = new QGLShader(QGLShader::Vertex, this);
    shadingFragmentShader = new QGLShader(QGLShader::Fragment, this);

    shadingVertexShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/phongVertexShader.vertexShader");
    shadingFragmentShader->compileSourceFile("/home/claudia/OpenGL Praktikum/Assignment 4/Terrain Modeling Tool/shaders/phongFragmentShader.fragmentShader");

    shadingProgram->addShader(shadingVertexShader);
    shadingProgram->addShader(shadingFragmentShader);

    glBindAttribLocation(shadingProgram->programId(), 1, "normal_in");
    glBindAttribLocation(shadingProgram->programId(), 3, "tex_in");

    shadingProgram->link();

    // distribute uniform IDs

    shadingIdID_ = glGetUniformLocation(shadingProgram->programId(), "id");
    ambientColorID_ = glGetUniformLocation(shadingProgram->programId(), "ambientColor_in");
    diffuseColorID_ = glGetUniformLocation(shadingProgram->programId(), "diffuseColor_in");
    specularColorID_ = glGetUniformLocation(shadingProgram->programId(), "specularColor_in");
    roughnessID_ = glGetUniformLocation(shadingProgram->programId(), "roughness_in");
    specularRoughnessID_ = glGetUniformLocation(shadingProgram->programId(), "specularRoughness_in");
    refractionIndexID_ = glGetUniformLocation(shadingProgram->programId(), "refractionIndex_in");
    textureIDs_ = glGetUniformLocation(shadingProgram->programId(), "texture");
    textureActiveIDs_ = glGetUniformLocation(shadingProgram->programId(), "textureActive");
    nrTexturesID_ = glGetUniformLocation(shadingProgram->programId(), "nrTextures");
    diffuseShaderID_ = glGetUniformLocation(shadingProgram->programId(), "diffuseShader");
    specularShaderID_ = glGetUniformLocation(shadingProgram->programId(), "specularShader");
    kaID_ = glGetUniformLocation(shadingProgram->programId(), "ka_in");
    kdID_ = glGetUniformLocation(shadingProgram->programId(), "kd_in");
    ksID_ = glGetUniformLocation(shadingProgram->programId(), "ks_in");

    checkGLErrors("after shaders");

    sphereBufferIDs_ = createPrimitiveBufferIDs(static_cast<Primitive*>(sphere_));
    sphere_->copyVAOToCurrentContext(sphereBufferIDs_);

    checkGLErrors("after copying sphere data");
}

void MaterialPreview::paintGL() {
    // clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrix(camera_->getCameraMatrix().constData());

    shadingProgram->bind();

    glLightfv(GL_LIGHT0, GL_POSITION, light0Position_);

    // set uniforms for the sphere
    glUniform1f(shadingIdID_, sphere_->getID());
    glUniform3f(ambientColorID_, sphere_->getAmbientColor().x_, sphere_->getAmbientColor().y_, sphere_->getAmbientColor().z_);
    glUniform3f(diffuseColorID_, sphere_->getDiffuseColor().x_, sphere_->getDiffuseColor().y_, sphere_->getDiffuseColor().z_);
    glUniform3f(specularColorID_, sphere_->getSpecularColor().x_, sphere_->getSpecularColor().y_, sphere_->getSpecularColor().z_);
    glUniform1f(kaID_, sphere_->getKa());
    glUniform1f(kdID_, sphere_->getKd());
    glUniform1f(ksID_, sphere_->getKs());
    glUniform1f(roughnessID_, sphere_->getRoughness());
    glUniform1f(specularRoughnessID_, sphere_->getSpecularRoughness());
    glUniform1f(refractionIndexID_, sphere_->getRefractionIndex());

    int *textureIDs = (int*) malloc(sizeof(int) * Primitive::NR_TEXTURES);
    int *textureActiveIDs = (int*) malloc(sizeof(int) * Primitive::NR_TEXTURES);
    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        textureIDs[i] = 10 + i;
        if (sphere_->isTextureActive(static_cast<Primitive::Textures>(i))) {
            textureActiveIDs[i] = 1;
        } else {
            textureActiveIDs[i] = 0;
        }

        if (sphereBufferIDs_.hasTextures_[i]) {
            glActiveTexture(GL_TEXTURE10 + i);
            glBindTexture(GL_TEXTURE_2D, sphereBufferIDs_.textures_[i]);
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

    glUniform1i(diffuseShaderID_, sphere_->getDiffuseShader());
    glUniform1i(specularShaderID_, sphere_->getSpecularShader());

    sphere_->draw(sphereBufferIDs_);

    shadingProgram->release();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (height() != 0) gluPerspective(45.0d, ((double) width()) / ((double) height()), NEAR_PLANE, FAR_PLANE);
    checkGLErrors("after setting projection matrix");
}



Primitive::bufferIDs MaterialPreview::createPrimitiveBufferIDs(Primitive* p) {
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



// SETTERS FOR THE CUBE PROPERTIES

void MaterialPreview::setAmbientColor(uint red, uint green, uint blue) {
    sphere_->setAmbientColor(Primitive::float3(red / 255.0f, green / 255.0f, blue / 255.0f));
    updateGL();
}

void MaterialPreview::setDiffuseColor(uint red, uint green, uint blue) {
    sphere_->setDiffuseColor(Primitive::float3(red / 255.0f, green / 255.0f, blue / 255.0f));
    updateGL();
}

void MaterialPreview::setSpecularColor(uint red, uint green, uint blue) {
    sphere_->setSpecularColor(Primitive::float3(red / 255.0f, green / 255.0f, blue / 255.0f));
    updateGL();
}

void MaterialPreview::setRoughness(int roughness) {
    sphere_->setRoughness(roughness / 100.0f);
    updateGL();
}

void MaterialPreview::setSpecularRoughness(int roughness) {
    sphere_->setSpecularRoughness(roughness / 100.0f);

    updateGL();
}

void MaterialPreview::setRefractionIndex(int refractionIndex) {
    sphere_->setRefractionIndex((refractionIndex / 100.0f) * 3 + 1);
    updateGL();
}

void MaterialPreview::setTextureChecked(Primitive::Textures x, bool status) {
    sphere_->setTextureActive(x, status);
    updateGL();
}

void MaterialPreview::setTexture(Primitive::Textures x, QImage texture) {
    makeCurrent();
    sphere_->setTexture(x, texture);

    glGenTextures(1, &sphereBufferIDs_.textures_[x]);
    sphere_->copyTextureToCurrentContext(sphereBufferIDs_.textures_[x], x);
    sphereBufferIDs_.hasTextures_[x] = true;

    updateGL();
}

void MaterialPreview::setDiffuseShader(int i) {
    sphere_->setDiffuseShader(i);
    updateGL();
}

void MaterialPreview::setSpecularShader(int i) {
    sphere_->setSpecularShader(i);
    updateGL();
}

void MaterialPreview::setKa(int ka) {
    sphere_->setKa(ka / 100.0f);
    updateGL();
}


void MaterialPreview::setKd(int kd) {
    sphere_->setKd(kd / 100.0f);
    updateGL();
}


void MaterialPreview::setKs(int ks) {
    sphere_->setKs(ks / 100.0f);
    updateGL();
}

void MaterialPreview::checkGLErrors(const char *label) {
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
