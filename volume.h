#ifndef VOLUME_H
#define VOLUME_H

#include <primitive.h>
#include <GL/gl.h>

class Volume : public Primitive
{
public:
    Volume(std::string name, int id, int tesselation, float3 color);
    ~Volume();
    void parseFile(QString fileName);
    void bindVAOToShader();
    void draw();

public slots:
    void copyVAOToCurrentContext();

private:
    int resolution_[3];
    float aspectRatio_[3];
    float normalizedAspectRatio_[3];

    GLuint volumeTexture_;
    uchar* data_;    

    GLuint vertexBufferTextureCoordinates_;
    attribute vertexTextureCoordinates_;

    void createCuboid(float width, float height, float depth);
    void transfer(uchar *data, uchar *transferredData, uint size);
};

#endif // VOLUME_H
