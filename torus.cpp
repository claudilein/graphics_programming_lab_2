#include "torus.h"
#include <math.h>

Torus::Torus(std::string name, int id, int tesselation,
             float innerRadius, float outerRadius, int sides, int rings) :
    Primitive(0, name, id, tesselation),
    innerRadius_(innerRadius),
    outerRadius_(outerRadius),
    sides_(sides),
    rings_(rings)
{

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
    for (int i = 0; i < vertexPositions_.size(); i++) {
        indicesList_.push_back(i);
    }


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions_.size() * sizeof(float3), &vertexPositions_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glBufferData(GL_ARRAY_BUFFER, vertexNormals_.size() * 3 * sizeof(float3), &vertexNormals_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glBufferData(GL_ARRAY_BUFFER, vertexColors_.size() * 3 * sizeof(float3), &vertexColors_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesList_.size() * sizeof(uint), &indicesList_[0], GL_STATIC_DRAW);




}

void Torus::draw() {
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPositions_);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNormals_);
    glVertexAttribPointer(
        1,                  // attribute 1
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferColors_);
    glVertexAttribPointer(
        2,                  // attribute 2
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);


    glDrawElements(
        GL_QUADS,      // mode
        indicesList_.size(),    // count
        GL_UNSIGNED_INT,       // type
        (void*)0           // element array buffer offset
    );
}
