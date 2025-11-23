#include "TriangleSurface.h"
#include "Triangulering.h"
#include <fstream>
#include <QDebug>
TriangleSurface::TriangleSurface() : VisualObject()
{
    Vertex v1{ 0.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f};  //  bottom-left corner
	Vertex v2{ 1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f }; //  bottom-right corner
	Vertex v3{ 0.0f,  1.0f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f }; //  top-left corner
	Vertex v4{ 1.0f,  1.0f,  0.0f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f }; //  top-right corner

    //Pushing 1st triangle,
    mVertices.push_back(v1);
    mVertices.push_back(v2);
    mVertices.push_back(v3);
    mVertices.push_back(v4);

	//Indexes for the two triangles to form a quad
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
	mIndices.push_back(2);
	mIndices.push_back(1);
	mIndices.push_back(3);

    //Temporary scale and positioning
    mMatrix.scale(0.5f);
    mMatrix.translate(0.5f, 0.1f, 0.1f);
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
        static float xOffset = v.x;
        static float yOffset = v.y;
        static float zOffset = v.z;
        //mMatrix.translate(v.x, v.y, v.z);
        v.x -= xOffset;    //Trying to get the coordinates closer to origo
        v.y = v.y * 3 - yOffset;
        v.z -= zOffset;
        mVertices.push_back(v);
        //qDebug() << v.x << v.y << v.z;
    }
    mVertices = triangulate(mVertices);

    int amountOfQuadsX = 100;
    int amountOfQuadsZ = 100;
    std::vector<uint32_t> indices;

    for (int i = 0; i < amountOfQuadsZ - 1; i++){
        for (int j = 0; j < amountOfQuadsX - 1; j++){
            //1st triangle
            indices.push_back(i * amountOfQuadsX + j);
            indices.push_back((i + 1) * amountOfQuadsX + j);
            indices.push_back(i * amountOfQuadsX + j + 1);
            //2nd triangle
            indices.push_back(i * amountOfQuadsX + j + 1);
            indices.push_back((i + 1) * amountOfQuadsX + j);
            indices.push_back((i + 1) * amountOfQuadsX + j + 1);
        }
    }
    mIndices = indices;
    //drawType = 2;
    inn.close();
    //Printing to .obj
    std::ofstream out("lasData.obj");
    if (!out.is_open())
        return;

    for (Vertex v : mVertices){
        out << "v" << v.x << v.y << v.z << "\n";
    }
    for (int i = 0; i < mIndices.size(); i += 3){
        out << "f" << mIndices[i] << mIndices[i+1] << mIndices[i+2] << "\n";
    }
    out.close();
}
