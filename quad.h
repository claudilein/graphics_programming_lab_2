#ifndef QUAD_H
#define QUAD_H

#include <primitive.h>

class Quad : public Primitive
{    
    Q_OBJECT
public:
    Quad();
    void bindVAOToShader();
    void draw();

public slots:
    void copyVAOToCurrentContext();

private:
    GLuint vertexBufferTextureCoordinates_;
    attribute vertexTextureCoordinates_;
};

#endif // QUAD_H
