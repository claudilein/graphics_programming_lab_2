#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/glew.h>
#include <QObject>
#include <vector>
#include <QQuaternion>
#include <QVector3D>



class Primitive : public QObject
{
    Q_OBJECT
public:

    struct float3 {
        float3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
        float3(float* vec) { x_ = vec[0]; y_ = vec[1]; z_ = vec[2]; }
        float x_;
        float y_;
        float z_;
    };

    struct RBTNode {
        RBTNode(QQuaternion q, QVector3D v) : rotation(q), translation(v) {}
        RBTNode() {}
        QQuaternion rotation;
        QVector3D translation;
    };

    typedef std::vector<float3> attribute;

    explicit Primitive(QObject *parent = 0, std::string name = 0, int id = 0, int tesselation = 0, float3 color = 0);

    void translate(QVector3D translation);
    void rotate(QQuaternion rotation);
    void scale(QVector3D scalingFactor);

    QMatrix4x4 getModelMatrix();
    virtual void draw();
    float3* getColor();
    int getID();
    std::string getName();
    void setName(std::string name);
    virtual void bindVAOToShader();

signals:

public slots:
    virtual void copyVAOToCurrentContext();

protected:
    std::string name_;
    float3 color_;
    int id_;
    int tesselation_;

    attribute vertexPositions_;
    attribute vertexColors_;
    attribute vertexNormals_;
    std::vector<uint> indicesList_;

    GLuint vertexArray_;
    GLuint vertexBufferPositions_;
    GLuint vertexBufferNormals_;
    GLuint vertexBufferColors_;
    GLuint indexBuffer_;
    float ambientColor_[4];

    QVector3D translation_;
    QQuaternion rotation_;
    QVector3D scalingFactor_;

    RBTNode rbtNode_;


};

#endif // PRIMITIVE_H
