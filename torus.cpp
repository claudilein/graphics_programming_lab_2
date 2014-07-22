#include "torus.h"
#include <math.h>
#include <iostream>

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
    hasVBO_[TEXCOORDS] = true;


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

    for (i = 0; i < rings_; i++) {
        theta1 = theta + ringDelta;
        cosTheta1 = (float)cos(theta1);
        sinTheta1 = (float)sin(theta1);

        phi = 0.0f;

        for (j = 0; j < sides_; j++) {
            float cosPhi, sinPhi, dist;
            //phi += sideDelta;
            cosPhi = (float)cos(phi);
            sinPhi = (float)sin(phi);
            dist = outerRadius_ + innerRadius_ * cosPhi;

            vertexPositions_.push_back(float3(cosTheta1 * dist, -sinTheta1 * dist, innerRadius_ * sinPhi));
            vertexPositions_.push_back(float3(cosTheta * dist, -sinTheta * dist, innerRadius_ * sinPhi));

            vertexNormals_.push_back(float3(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi));
            vertexNormals_.push_back(float3(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi));

            vertexTextureCoordinates_.push_back(float3(i / (float) rings_, j / (float) sides_ ,0));
            vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, j / (float) sides_, 0));

            phi += sideDelta;
            cosPhi = (float)cos(phi);
            sinPhi = (float)sin(phi);
            dist = outerRadius_ + innerRadius_ * cosPhi;

            vertexPositions_.push_back(float3(cosTheta * dist, -sinTheta * dist, innerRadius_ * sinPhi));
            vertexPositions_.push_back(float3(cosTheta1 * dist, -sinTheta1 * dist, innerRadius_ * sinPhi));

            vertexNormals_.push_back(float3(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi));
            vertexNormals_.push_back(float3(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi));

            vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, (j + 1) / (float) sides_,0));
            vertexTextureCoordinates_.push_back(float3(i / (float) rings_, (j + 1) / (float) sides_, 0));

            //phi -= sideDelta;
        }

        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }

/*
    // one ring equals a vertical slice of the torus
    for (i = rings_ - 1; i >= 0; i--)
    {
        theta1 = theta + ringDelta;
        cosTheta1 = (float)cos(theta1);
        sinTheta1 = (float)sin(theta1);

        phi = 0.0f;

        // a side equals a horizontal slice of the vertical slice
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

                vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, (j + 1) / (float) (sides_ + 1),0));
                vertexTextureCoordinates_.push_back(float3(i / (float) rings_, (j + 1) / (float) (sides_ + 1), 0));


                //vertexTextureCoordinates_.push_back(float3(0,0,0));
                //vertexTextureCoordinates_.push_back(float3(1,0,0));

            }

            vertexPositions_.push_back(float3(cosTheta1 * dist, -sinTheta1 * dist, innerRadius_ * sinPhi));
            vertexPositions_.push_back(float3(cosTheta * dist, -sinTheta * dist, innerRadius_ * sinPhi));

            vertexNormals_.push_back(float3(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi));
            vertexNormals_.push_back(float3(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi));

            vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, j / (float) (sides_ + 1),0));
            vertexTextureCoordinates_.push_back(float3(i / (float) rings_, j / (float) (sides_ + 1), 0));
            //vertexTextureCoordinates_.push_back(float3(1,1,0));
            //vertexTextureCoordinates_.push_back(float3(0,1,0));
        }

        vertexPositions_.push_back(vertexPositions_[(rings_ - 1 - i) * (sides_ * 4)]);
        vertexPositions_.push_back(vertexPositions_[(rings_ - 1 - i) * (sides_ * 4) + 1]);

        vertexNormals_.push_back(vertexNormals_[(rings_ - 1 - i) * (sides_ * 4)]);
        vertexNormals_.push_back(vertexNormals_[(rings_ - 1 - i) * (sides_ * 4) + 1]);

        vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, (j + 1) / (float) (sides_ + 1),0));
        vertexTextureCoordinates_.push_back(float3(i / (float) rings_, (j + 1) / (float) (sides_ + 1), 0));

        //vertexTextureCoordinates_.push_back(vertexTextureCoordinates_[(rings_ - 1 - i) * (sides_ * 4)]);
        //vertexTextureCoordinates_.push_back(vertexTextureCoordinates_[(rings_ - 1 - i) * (sides_ * 4) + 1]);

        //vertexTextureCoordinates_.push_back(float3(i / (float) rings_, j / (float) sides_,0));
        //vertexTextureCoordinates_.push_back(float3((i + 1) / (float) rings_, j / (float) sides_, 0));

        //vertexTextureCoordinates_.push_back(float3(0,0,0));
        //vertexTextureCoordinates_.push_back(float3(1,0,0));

        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
*/
    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float torusColor[3] = {1, 0, 1};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(torusColor));
    }

    generateTangents(4);
    std::cout << "vertices numer: " << vertexPositions_.size() << std::endl;
}

/*
void Torus::generateTangents(int verticesPerPrimitive, int startIndex) {
    hasVBO_[TANGENTS] = true;
    hasVBO_[BITANGENTS] = true;

    // compute tangents and bitangents
    float3 deltaPos1, deltaPos2, deltaUV1, deltaUV2, tangent, bitangent;
    float r;

    //for (uint i = startIndex; i < vertexPositions_.size(); i += verticesPerPrimitive) {
    for (uint i = startIndex; i < rings_ * sides_; i += 2) {
        deltaPos1 = vertexPositions_[i + 1] - vertexPositions_[i];
        deltaPos2 = vertexPositions_[i + 2] - vertexPositions_[i];
        deltaUV1 = vertexTextureCoordinates_[i + 1] - vertexTextureCoordinates_[i];
        deltaUV2 = vertexTextureCoordinates_[i + 2] - vertexTextureCoordinates_[i];

        r = 1.0f / (deltaUV1.x_ * deltaUV2.y_ - deltaUV1.y_ * deltaUV2.x_);
        tangent = (deltaPos1 * deltaUV2.y_ - deltaPos2 * deltaUV1.y_) * r;
        bitangent = (deltaPos2 * deltaUV1.x_ - deltaPos1 * deltaUV2.x_) * r;

        // make tangent base orthogonal
        //tangent = normalize(tangent - n * glm::dot(n, t));

        for (int j = 0; j < verticesPerPrimitive; j++) {
            float3 normal = vertexNormals_[i + j];
            float3 t = float3::normalize(tangent - normal * float3::dot(normal, tangent));
            vertexTangents_.push_back(t);
            float3 b = float3::normalize((bitangent - normal * float3::dot(normal, bitangent) - tangent * float3::dot(tangent, bitangent)));
            vertexBitangents_.push_back(b);
        }
    }
}*/

void Torus::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}
