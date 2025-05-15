#include "TriangleSurface.h"
#include <fstream>
#include <QDebug>
TriangleSurface::TriangleSurface() : VisualObject()
{
    Vertex v1{ -3.0f,  0.0f,  -5.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f};  //  bottom-left corner
    Vertex v2{ 3.0f,  0.0f,  -5.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f }; //  bottom-right corner
    Vertex v3{ -3.0f,  0.0f,  8.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f }; //  top-left corner
    Vertex v4{ 3.0f,  0.0f,  8.0f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f }; //  top-right corner

    //Pushing 1st triangle,
    mVertices.push_back(v1);
    mVertices.push_back(v2);
    mVertices.push_back(v3);
    mVertices.push_back(v4);

	//Indexes for the two triangles to form a quad
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
    mIndices.push_back(1);
    mIndices.push_back(3);
    mIndices.push_back(2);

    //Temporary scale and positioning
    mMatrix.scale(0.5f);
    mMatrix.translate(0.5f, 0.1f, 0.1f);

    colorType = 1;
}

TriangleSurface::TriangleSurface(const std::string &filename)
{
    std::ifstream inn(filename);
    if (!inn.is_open())
        return;
    // read input from math part of compulsory
    int n;
    Vertex v;
    inn >> n;
    for (auto i=0; i<n; i++)
    {
        inn >> v;
        mVertices.push_back(v);
        //qDebug() << v.x << v.y << v.z;
    }
    inn.close();
}
