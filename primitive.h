#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/glew.h>
#include <QObject>
#include <vector>
#include <QQuaternion>
#include <QVector3D>
#include <QImage>



class Primitive : public QObject
{
    Q_OBJECT
public:
    enum VBO { NORMALS, COLORS, TEXCOORDS };
    static const int NR_VBOS = 3;
    enum Textures { AMBIENT, DIFFUSE, SPECULAR, ROUGHNESS, REFRACTION_INDEX, NORMAL };
    static const int NR_TEXTURES = 6;

    struct bufferIDs {
        bufferIDs() : VAO_(0), positions_(0), indices_(0), normals_(0), colors_(0), texCoords_(0), hasNormals_(false), hasColors_(false), hasTexCoords_(false) {
            for (int i = 0; i < NR_TEXTURES; i++) {
                textures_[i] = 0;
                hasTextures_[i] = false;
            }
        }

        bufferIDs(GLuint VAO, GLuint positions, GLuint indices, GLuint normals, GLuint colors, GLuint texCoords, bool hasNormals, bool hasColors, bool hasTexCoords) :
            VAO_(VAO), positions_(positions), indices_(indices), normals_(normals), colors_(colors), texCoords_(texCoords), hasNormals_(hasNormals), hasColors_(hasColors), hasTexCoords_(hasTexCoords)
        {
            for (int i = 0; i < NR_TEXTURES; i++) {
                textures_[i] = 0;
                hasTextures_[i] = false;
            }
        }

        GLuint VAO_;
        GLuint positions_;
        GLuint indices_;
        GLuint normals_;
        GLuint colors_;
        GLuint texCoords_;
        GLuint textures_[NR_TEXTURES];

        bool hasNormals_;
        bool hasColors_;
        bool hasTexCoords_;
        bool hasTextures_[NR_TEXTURES];
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
    void setAmbientColor(float3 color);
    void setDiffuseColor(float3 color);
    void setSpecularColor(float3 color);
    void setRoughness(float roughness);
    void setRefractionIndex(float refractionIndex);
    void setTexture(Textures x, QImage texture);
    void setTextureActive(Textures x, bool status);
    void copyTextureToCurrentContext(GLuint textureID, Textures x);

    virtual void bindVAOToShader(bufferIDs buffIDs);

signals:

public slots:
    virtual void copyVAOToCurrentContext(bufferIDs buffIDs);

protected:
    std::string name_;
    int id_;
    int tesselation_;

    // material properties
    float3 ambientColor_;
    float3 diffuseColor_;
    float3 specularColor_;
    float roughness_;
    float refractionIndex_;
    QImage textures[NR_TEXTURES];
    bool texturesActive[NR_TEXTURES];


    float3 color_;

    attribute vertexPositions_;
    attribute vertexColors_;
    attribute vertexNormals_;
    attribute vertexTextureCoordinates_;
    std::vector<uint> indicesList_;

    QVector3D translation_;
    QQuaternion rotation_;
    QVector3D scalingFactor_;

    RBTNode rbtNode_;
    bool isVolume_;
    bool isTerrain_;

    bool hasVBO_[NR_VBOS];


};

#endif // PRIMITIVE_H
