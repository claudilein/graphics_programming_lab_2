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
    void draw(bufferIDs buffIDs);
    void uploadMaterial(QString fileName, GLuint textureID);

    int getHorizontalScale();
    int getVerticalScale();
    int getGridSize();
    unsigned short* getHeightValues();
    int getWidth();

public slots:
    //void copyVAOToCurrentContext();
    void createTextures(GLuint id);

private:
    int horizontalScale_;   // size of the terrain in world coordinates. I.e. for 50, the terrain will have a size of (50, 50) with the center being in (0, 0, 0).
    int verticalScale_;     // height of the terrain. Values are transformed into range of [0, 1] when uploaded to OpenGL texture with GL_LUMINANCE, so they have to be scaled accordingly.
    int gridSize_;          // in unit squares. Number of quads the sliding window grid should consist of. 50 should be a good start.

    int width_;
    int height_;

    GLuint heightTexture_;

    unsigned short *heightValues_;

    void checkGLErrors(const char *label);

};

#endif // TERRAIN_H
