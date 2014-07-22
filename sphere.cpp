#include "sphere.h"
#include <math.h>
#include <iostream>

Sphere::Sphere(std::string name, int id, int tesselation, float3 color,
               float radius, int lats, int longs) :
    Primitive(0, name, id, tesselation, color),
    radius_(radius),
    lats_(lats + tesselation),
    longs_(longs + tesselation)
{

    hasVBO_[NORMALS] = true;
    hasVBO_[COLORS] = true;
    hasVBO_[TEXCOORDS] = true;

    // lats = horizontal, so the outer loop iterates over the different horizontal rings
    for (int i = 0; i < lats_; i++)
    {
        double theta_upper = M_PI * i / static_cast<double>(lats_);
        double theta_lower = M_PI * (i+1) / static_cast<double>(lats_);
        // longs = vertical, so the inner loop draws all quads in one horizontal ring
        for (int j = 0; j < longs_; j++)
        {
            double phi_left = 2* M_PI * j / static_cast<double>(longs_);
            double phi_right = 2* M_PI * (j+1) / static_cast<double>(longs_);

            vertexPositions_.push_back(float3(radius_*sin(theta_upper)*cos(phi_left),
                                              radius_*sin(theta_upper)*sin(phi_left),
                                              radius_*cos(theta_upper)));
            vertexPositions_.push_back(float3(radius_*sin(theta_lower)*cos(phi_left),
                                              radius_*sin(theta_lower)*sin(phi_left),
                                              radius_*cos(theta_lower)));
            vertexPositions_.push_back(float3(radius_*sin(theta_lower)*cos(phi_right),
                                              radius_*sin(theta_lower)*sin(phi_right),
                                              radius_*cos(theta_lower)));
            vertexPositions_.push_back(float3(radius_*sin(theta_upper)*cos(phi_right),
                                              radius_*sin(theta_upper)*sin(phi_right),
                                              radius_*cos(theta_upper)));


            float3 normal0 = float3(radius_*sin(theta_upper)*cos(phi_left),
                                    radius_*sin(theta_upper)*sin(phi_left),
                                    radius_*cos(theta_upper));
            vertexNormals_.push_back(normal0);

            float3 normal1 = float3(radius_*sin(theta_lower)*cos(phi_left),
                                    radius_*sin(theta_lower)*sin(phi_left),
                                    radius_*cos(theta_lower));
            vertexNormals_.push_back(normal1);

            float3 normal2 = float3(radius_*sin(theta_lower)*cos(phi_right),
                                    radius_*sin(theta_lower)*sin(phi_right),
                                    radius_*cos(theta_lower));
            vertexNormals_.push_back(normal2);

            float3 normal3 = float3(radius_*sin(theta_upper)*cos(phi_right),
                                    radius_*sin(theta_upper)*sin(phi_right),
                                    radius_*cos(theta_upper));
            vertexNormals_.push_back(normal3);

            vertexTextureCoordinates_.push_back(float3(j / (float) longs_, i / (float) lats_, 0));
            vertexTextureCoordinates_.push_back(float3(j / (float) longs_, (i + 1) / (float) lats_, 0));
            vertexTextureCoordinates_.push_back(float3((j + 1) / (float) longs_, (i + 1) / (float) lats_, 0));
            vertexTextureCoordinates_.push_back(float3((j + 1) / (float) longs_, i / (float) lats_, 0));

            /*if (i == 0 || i == lats_ - 1) {
                std::cout << "tex0 " << i << ": " << float3::toString(vertexTextureCoordinates_[i * longs_ * 4 + j]).toStdString() << std::endl;
                std::cout << "tex1 " << i << ": " << float3::toString(vertexTextureCoordinates_[i * longs_ * 4  + j + 1]).toStdString() << std::endl;
                std::cout << "tex2 " << i << ": " << float3::toString(vertexTextureCoordinates_[i * longs_ * 4  + j + 2]).toStdString() << std::endl;
                std::cout << "tex3 " << i << ": " << float3::toString(vertexTextureCoordinates_[i * longs_ * 4  + j + 3]).toStdString() << std::endl;
            }*/

        }
    }



    // set indices list
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }

    float sphereColor[3] = {0, 1, 0};
    for (uint i = 0; i < vertexPositions_.size(); i++) {
        vertexColors_.push_back(float3(sphereColor));
    }

    generateTangents(4);

}



void Sphere::draw(bufferIDs buffIDs) {

    bindVAOToShader(buffIDs);

    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}

void Sphere::generateTangents(int verticesPerPrimitive, int startIndex) {
    hasVBO_[TANGENTS] = true;
    hasVBO_[BITANGENTS] = true;

    // compute tangents and bitangents
    float3 deltaPos1, deltaPos2, deltaUV1, deltaUV2, tangent, bitangent;
    float r;

    for (uint i = startIndex; i < longs_ * verticesPerPrimitive; i += verticesPerPrimitive) {

        deltaPos1 = vertexPositions_[i + 1] - vertexPositions_[i];
        deltaPos2 = vertexPositions_[i + 2] - vertexPositions_[i];
        deltaUV1 = vertexTextureCoordinates_[i + 1] - vertexTextureCoordinates_[i];
        deltaUV2 = vertexTextureCoordinates_[i + 2] - vertexTextureCoordinates_[i];

        r = 1.0f / (deltaUV1.x_ * deltaUV2.y_ - deltaUV1.y_ * deltaUV2.x_);
        tangent = (deltaPos1 * deltaUV2.y_ - deltaPos2 * deltaUV1.y_) * r;
        bitangent = (deltaPos2 * deltaUV1.x_ - deltaPos1 * deltaUV2.x_) * r;


        for (int j = 0; j < verticesPerPrimitive; j++) {
            float3 normal = vertexNormals_[i + j];
            float3 t = float3::normalize(tangent - normal * float3::dot(normal, tangent));
            if (float3::dot(normal, t) > 0.0001) {
             //   std::cout << "NdotT(" << i + j << "): " << float3::dot(normal, t) << std::endl;
            }
            vertexTangents_.push_back(t);
            float3 b = float3::normalize((bitangent - normal * float3::dot(normal, bitangent) - tangent * float3::dot(tangent, bitangent)));
            if (float3::dot(t, b) > 0.0001) {
            //    std::cout << "TdotB(" << i + j << "): " << float3::dot(t, b) << std::endl;
            }
            vertexBitangents_.push_back(b);
        }
    }

    for (uint i = longs_ * verticesPerPrimitive; i < vertexPositions_.size(); i += verticesPerPrimitive) {

        deltaPos1 = vertexPositions_[i + 1] - vertexPositions_[i];
        deltaPos2 = vertexPositions_[i + 3] - vertexPositions_[i];
        deltaUV1 = vertexTextureCoordinates_[i + 1] - vertexTextureCoordinates_[i];
        deltaUV2 = vertexTextureCoordinates_[i + 3] - vertexTextureCoordinates_[i];

        r = 1.0f / (deltaUV1.x_ * deltaUV2.y_ - deltaUV1.y_ * deltaUV2.x_);
        tangent = (deltaPos1 * deltaUV2.y_ - deltaPos2 * deltaUV1.y_) * r;
        bitangent = (deltaPos2 * deltaUV1.x_ - deltaPos1 * deltaUV2.x_) * r;


        for (int j = 0; j < verticesPerPrimitive; j++) {
            float3 normal = vertexNormals_[i + j];
            float3 t = float3::normalize(tangent - normal * float3::dot(normal, tangent));
            if (float3::dot(normal, t) > 0.0001) {
            //    std::cout << "NdotT(" << i + j << "): " << float3::dot(normal, t) << std::endl;
            }
            vertexTangents_.push_back(t);
            float3 b = float3::normalize((bitangent - normal * float3::dot(normal, bitangent) - tangent * float3::dot(tangent, bitangent)));
            if (float3::dot(t, b) > 0.0001) {
            //    std::cout << "TdotB(" << i + j << "): " << float3::dot(t, b) << std::endl;
            }
            vertexBitangents_.push_back(b);
        }
    }
}
