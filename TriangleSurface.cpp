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

    //calculating normal vector for vertices
    std::vector<QVector3D> vertexNormals;
    for (int i = 0; i < mVertices.size(); i++)
    {
        QVector3D vertexNormal;
        //Is it on the top border
        if (i < amountOfQuadsX){
            //Is it on the left corner
            if (i % amountOfQuadsX == 0 ){

            }
             //Is it on the right corner
            else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){

            }
            //Is a normal border
            else{

            }
        }
        //Is it on the bottom border
        else if(i >= mVertices.size() - amountOfQuadsX){
            //Is it on the left corner
            if (i % amountOfQuadsX == 0 ){

            }
            //Is it on the right corner
            else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){

            }
            //Is a normal border
            else{

            }
        }
        //Is it on the left border
        if (i % amountOfQuadsX == 0 ){

        }
        //Is it on the right border
        else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){

        }
        //Is a normal vertex
        else{
            //there are 4 triangles using it
            //Triangle 1
            QVector3D vector1 = QVector3D(
                (mVertices.at(i-amountOfQuadsX).x - mVertices.at(i).x),
                (mVertices.at(i-amountOfQuadsX).y - mVertices.at(i).y),
                (mVertices.at(i-amountOfQuadsX).z - mVertices.at(i).z)
                );
            QVector3D vector2 = QVector3D(
                (mVertices.at(i-amountOfQuadsX).x - mVertices.at(i).x),
                (mVertices.at(i-amountOfQuadsX).y - mVertices.at(i).y),
                (mVertices.at(i-amountOfQuadsX).z - mVertices.at(i).z)
                );
            QVector3D::crossProduct(vector1, vector2)
        }
    }
    //Printing to .obj
    /*
    std::ofstream out("lasData.obj");
    if (!out.is_open())
        return;

    for (Vertex v : mVertices){
        out << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }
    for (int i = 0; i < mIndices.size(); i += 3){
        //Obj uses base 1 indices
        out << "f " << (mIndices.at(i) + 1) << " " << (mIndices.at(i + 1) + 2) << " " << (mIndices.at(i + 2) + 3) << "\n";
    }
    out.close();
*/
}
