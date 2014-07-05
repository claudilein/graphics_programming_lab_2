#include "torus.h"
#include <math.h>

Torus::Torus(std::string name, int id, int tesselation, float3 color,
             float innerRadius, float outerRadius, int sides, int rings) :
    Primitive(0, name, id, tesselation, color),
    innerRadius_(innerRadius),
    outerRadius_(outerRadius),
    sides_(sides + tesselation_),
    rings_(rings + tesselation_)
{
    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;

    int i, j;
    float theta, phi, theta1;
    float cosTheta, sinTheta;
    float cosTheta1, sinTheta1;
    float ringDelta, sideDelta;

    ringDelta = (float)(2.0f * M_PI / rings_);
    sideDelta = (float)(2.0f * M_PI / sides_);

    theta = 0.0f;
    cosTheta = 1.0f;
    sinTheta = 0.0f;

    for (i = rings_ - 1; i >= 0; i--)
    {
        theta1 = theta + ringDelta;
        cosTheta1 = (float)cos(theta1);
        sinTheta1 = (float)sin(theta1);

        phi = 0.0f;

        for (j = sides_; j >= 0; j--)
        {
            float cosPhi, sinPhi, dist;

            phi += sideDelta;
            cosPhi = (float)cos(phi);
            sinPhi = (float)sin(phi);
            dist = outerRadius_ + innerRadius_ * cosPhi;

            if (j != sides_ && j != 0) {
                vertexPositions_.push_back(float3(cosTheta1 * dist, -sinTheta1 * dist, innerRadius_ * sinPhi));
                vertexPositions_.push_back(float3(cosTheta * dist, -sinTheta * dist, innerRadius_ * sinPhi));

                vertexNormals_.push_back(float3(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi));
                vertexNormals_.push_back(float3(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi));
            }

            vertexPositions_.push_back(float3(cosTheta1 * dist, -sinTheta1 * dist, innerRadius_ * sinPhi));
            vertexPositions_.push_back(float3(cosTheta * dist, -sinTheta * dist, innerRadius_ * sinPhi));

            vertexNormals_.push_back(float3(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi));
            vertexNormals_.push_back(float3(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi));
        }

        vertexPositions_.push_back(vertexPositions_[(rings_ - 1 - i) * (sides_ * 4)]);
        vertexPositions_.push_back(vertexPositions_[(rings_ - 1 - i) * (sides_ * 4) + 1]);
        vertexNormals_.push_back(vertexNormals_[(rings_ - 1 - i) * (sides_ * 4)]);
        vertexNormals_.push_back(vertexNormals_[(rings_ - 1 - i) * (sides_ * 4) + 1]);

        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }

    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float torusColor[3] = {1, 0, 1};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(torusColor));
    }

    ambientColor_[0] = 0.2;
    ambientColor_[2] = 0.2;
}

void Torus::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_QUAD_STRIP,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}
