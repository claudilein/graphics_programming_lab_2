#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/glew.h>
#include <QObject>
#include <vector>



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
    typedef std::vector<float3> attribute;

    explicit Primitive(QObject *parent = 0, std::string name = 0, int id = 0, int tesselation = 0);

    attribute getVertexPositions();
    attribute getVertexColors();
    attribute getVertexNormals();
    virtual void draw();

signals:

public slots:

protected:
    std::string name_;
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
};

#endif // PRIMITIVE_H
