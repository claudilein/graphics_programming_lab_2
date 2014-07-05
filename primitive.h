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
    enum VBO { NORMALS, COLORS, TEXCOORDS };
    static const int NR_VBOS = 3;


    struct bufferIDs {
        bufferIDs() : VAO_(0), positions_(0), indices_(0), normals_(0), colors_(0), texCoords_(0), hasNormals_(false), hasColors_(false), hasTexCoords_(false) {}

        bufferIDs(GLuint VAO, GLuint positions, GLuint indices, GLuint normals, GLuint colors, GLuint texCoords, bool hasNormals, bool hasColors, bool hasTexCoords) :
            VAO_(VAO), positions_(positions), indices_(indices), normals_(normals), colors_(colors), texCoords_(texCoords), hasNormals_(hasNormals), hasColors_(hasColors), hasTexCoords_(hasTexCoords)
        {}

        GLuint VAO_;
        GLuint positions_;
        GLuint indices_;
        GLuint normals_;
        GLuint colors_;
        GLuint texCoords_;

        bool hasNormals_;
        bool hasColors_;
        bool hasTexCoords_;
    };

    struct float3 {
        float3() { x_ = 0.0f; y_ = 0.0f; z_ = 0.0f; }
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
    virtual void draw(bufferIDs buffIDs);
    float3* getColor();
    int getID();
    bool isVolume();
    bool isTerrain();
    std::string getName();
    bool hasVBO(VBO vbo);

    void setName(std::string name);
    virtual void bindVAOToShader(bufferIDs buffIDs);

signals:

public slots:
    virtual void copyVAOToCurrentContext(bufferIDs buffIDs);

protected:
    std::string name_;
    float3 color_;
    int id_;
    int tesselation_;

    attribute vertexPositions_;
    attribute vertexColors_;
    attribute vertexNormals_;
    attribute vertexTextureCoordinates_;
    std::vector<uint> indicesList_;

    float ambientColor_[4];

    QVector3D translation_;
    QQuaternion rotation_;
    QVector3D scalingFactor_;

    RBTNode rbtNode_;
    bool isVolume_;
    bool isTerrain_;

    bool hasVBO_[NR_VBOS];


};

#endif // PRIMITIVE_H
