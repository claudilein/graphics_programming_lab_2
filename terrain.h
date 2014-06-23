#ifndef TERRAIN_H
#define TERRAIN_H

#include <primitive.h>

class Terrain : public Primitive
{
public:
    Terrain(std::string name, int id, int tesselation, float3 color);

    void parseHeightMap(QString fileName);
    void setHorizontalScale(int horizontalScale);
    void setVerticalScale(int verticalScale);
    void bindVAOToShader();
    void draw();
    void createVBO();
    void uploadMaterial(QString fileName);

    void changeRange(int materialID, int minRange, int maxRange);

public slots:
    //void copyVAOToCurrentContext();
    void createTextures();

private:
    int horizontalScale_;
    int verticalScale_;

    int width_;
    int height_;

    GLuint heightTexture_;
    GLuint* materialTextures_;
    uint nrMaterials_;

    unsigned short *heightValues_;

    void checkGLErrors(const char *label);

};

#endif // TERRAIN_H
