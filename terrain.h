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
    void copyVAOToCurrentContext();
    void draw();
    void createVBO();
    void uploadMaterial(QString fileName);

    void changeRange(int materialID, int minRange, int maxRange);
    int getHorizontalScale();
    int getVerticalScale();
    int getGridSize();
    int getNrMaterials();
    int* getMaterialIDs();
    unsigned short* getHeightValues();
    int getWidth();

public slots:
    //void copyVAOToCurrentContext();
    void createTextures();

private:
    int horizontalScale_;   // size of the terrain in world coordinates. I.e. for 50, the terrain will have a size of (50, 50) with the center being in (0, 0, 0).
    int verticalScale_;     // height of the terrain. Values are transformed into range of [0, 1] when uploaded to OpenGL texture with GL_LUMINANCE, so they have to be scaled accordingly.
    int gridSize_;          // in unit squares. Number of quads the sliding window grid should consist of. 50 should be a good start.

    int width_;
    int height_;

    GLuint heightTexture_;
    GLuint* materialTextures_;
    GLuint vertexBufferTextureCoordinates_;

    attribute vertexTextureCoordinates_;

    uint nrMaterials_;

    unsigned short *heightValues_;

    void checkGLErrors(const char *label);

};

#endif // TERRAIN_H
