#ifndef TERRAIN_H
#define TERRAIN_H

#include <primitive.h>

class Terrain : public Primitive
{
public:
    Terrain(std::string name, int id, int tesselation, float3 color);


    void setHorizontalScale(int horizontalScale);
    void setVerticalScale(int verticalScale);
    void bindVAOToShader();
    void draw();
    void createVBO();

public slots:
    //void copyVAOToCurrentContext();
    void createTextures();

private:
    int horizontalScale_;
    int verticalScale_;

    GLuint heightTexture_;

};

#endif // TERRAIN_H
