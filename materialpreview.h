#ifndef MATERIALPREVIEW_H
#define MATERIALPREVIEW_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <sphere.h>
#include <camera.h>


class MaterialPreview : public QGLWidget
{
    Q_OBJECT
public:
    explicit MaterialPreview(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:

public slots:
    Primitive::bufferIDs createPrimitiveBufferIDs(Primitive* p);

    void setAmbientColor(uint red, uint green, uint blue);
    void setDiffuseColor(uint red, uint green, uint blue);
    void setSpecularColor(uint red, uint green, uint blue);
    void setRoughness(int roughness);
    void setSpecularRoughness(int roughness);
    void setRefractionIndex(int refractionIndex);
    void setTextureChecked(Primitive::Textures x, bool status);
    void setTexture(Primitive::Textures x, QImage texture);
    void setDiffuseShader(int i);
    void setSpecularShader(int i);
    void setKa(int ka);
    void setKd(int kd);
    void setKs(int ks);

protected:
    void initializeGL();
    void paintGL();


private:
    static const double NEAR_PLANE = 0.01d;
    static const double FAR_PLANE = 20.0d;

    // master shader that uses the selected shading on the object
    QGLShaderProgram *shadingProgram;
    QGLShader *shadingVertexShader;
    QGLShader *shadingFragmentShader;

    // master shader for lighting
    GLuint shadingIdID_;
    GLuint ambientColorID_;
    GLuint diffuseColorID_;
    GLuint specularColorID_;
    GLuint roughnessID_;
    GLuint specularRoughnessID_;
    GLuint refractionIndexID_;
    GLuint textureIDs_;
    GLuint textureActiveIDs_;
    GLuint nrTexturesID_;
    GLuint diffuseShaderID_;
    GLuint specularShaderID_;
    GLuint kaID_;
    GLuint kdID_;
    GLuint ksID_;

    Sphere *sphere_;
    Camera *camera_;
    Primitive::bufferIDs sphereBufferIDs_;

    float light0Position_[4];


    void checkGLErrors(const char *label);

};

#endif // MATERIALPREVIEW_H
