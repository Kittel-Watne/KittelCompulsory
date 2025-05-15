#include "bullet.h"

Bullet::Bullet() {
    Vertex v1{ -0.2f,  -0.2f,  -0.2f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f};
    Vertex v2{ 0.2f,  -0.2f,  -0.2f,   1.0f, 1.0f, 1.0f,  1.0f, 0.0f };
    Vertex v3{ -0.2f,  -0.2f,  0.2f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f };
    Vertex v4{ 0.2f,  -0.2f,  0.2f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f };
    Vertex v5{ -0.2f,  0.2f,  -0.2f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f};
    Vertex v6{ 0.2f,  0.2f,  -0.2f,   1.0f, 1.0f, 1.0f,  1.0f, 0.0f };
    Vertex v7{ -0.2f,  0.2f,  0.2f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f };
    Vertex v8{ 0.2f,  0.2f,  0.2f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f };

    //Pushing 1st triangle,
    mVertices.push_back(v1);
    mVertices.push_back(v2);
    mVertices.push_back(v3);
    mVertices.push_back(v4);
    mVertices.push_back(v5);
    mVertices.push_back(v6);
    mVertices.push_back(v7);
    mVertices.push_back(v8);

    //Indexes for the two triangles to form a quad
    //Bottom
    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(1);
    mIndices.push_back(3);
    mIndices.push_back(2);
    //Top
    mIndices.push_back(4);
    mIndices.push_back(5);
    mIndices.push_back(6);
    mIndices.push_back(5);
    mIndices.push_back(7);
    mIndices.push_back(6);
    //Front
    mIndices.push_back(2);
    mIndices.push_back(3);
    mIndices.push_back(6);
    mIndices.push_back(3);
    mIndices.push_back(7);
    mIndices.push_back(6);
    //Back
    mIndices.push_back(0);
    mIndices.push_back(5);
    mIndices.push_back(1);
    mIndices.push_back(0);
    mIndices.push_back(4);
    mIndices.push_back(5);
    //Right
    mIndices.push_back(1);
    mIndices.push_back(7);
    mIndices.push_back(3);
    mIndices.push_back(1);
    mIndices.push_back(5);
    mIndices.push_back(7);
    //Left
    mIndices.push_back(0);
    mIndices.push_back(2);
    mIndices.push_back(4);
    mIndices.push_back(2);
    mIndices.push_back(6);
    mIndices.push_back(4);

    mMatrix.translate(0.0f, 0.25f, 0.0f);
    mMatrix.scale(0.2f);

    shouldRender = false;
    drawType = 1;
}
