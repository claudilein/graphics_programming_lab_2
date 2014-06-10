#ifndef VOLUME_H
#define VOLUME_H

#include <primitive.h>
#include <GL/gl.h>

class Volume : public Primitive
{
public:
    Volume(std::string name, int id, int tesselation, float3 color);
    ~Volume();

    struct transferScalar {
        transferScalar() { r_ = 0; g_ = 0; b_ = 0; a_ = 0; }
        transferScalar(int r, int g, int b, int a) {
            r_ = r; g_ = g; b_ = b; a_ = a;
        }

        int r_; int g_; int b_; int a_;
    };

    struct changeTransferScalar {
        changeTransferScalar() {
            r_ = true; g_ = true; b_ = true; a_ = true;
        }
        changeTransferScalar(bool r, bool g, bool b, bool a) {
            r_ = r; g_ = g; b_ = b; a_ = a;
        }

        bool r_; bool g_; bool b_; bool a_;
    };

    void parseFile(QString fileName);

    void setTransferFunction(int index, changeTransferScalar change, transferScalar scalar);
    void setTransferFunction(transferScalar *scalar);
    void resetTransferFunction(changeTransferScalar change);
    void uploadTransferFunction();

    Primitive::float3 getAspectRatio();
    transferScalar* getTransferFunction();
    int getMaxResolution();
    int* getHistogram();

    void bindVAOToShader();
    void draw();

public slots:
    void copyVAOToCurrentContext();

private:
    static const int MAX_SCALAR_VALUE_ = 256;
    int resolution_[3];
    float3 aspectRatio_;
    float3 normalizedAspectRatio_;

    GLuint volumeTexture_;
    GLuint transferTexture_;

    uchar* floatData_;

    GLuint vertexBufferTextureCoordinates_;
    attribute vertexTextureCoordinates_;

    transferScalar transferFunction_[MAX_SCALAR_VALUE_];
    int histogram_[MAX_SCALAR_VALUE_];

    void createCuboid(float width, float height, float depth);
};

#endif // VOLUME_H
