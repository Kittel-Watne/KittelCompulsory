#include "TriangleSurface.h"
#include "Triangulering.h"
#include <fstream>
#include <QDebug>
#include <iomanip>
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
        v.x -= xOffset;    //getting coordinates closer to origo
        v.y = v.y - yOffset;
        v.z -= zOffset;
        mVertices.push_back(v);
    }

    //set drawtype to 2 and comment out everything below to show pointcloud
    drawType = 2;
    inn.close();

    //Everything below here is for triangulation/printing of obj

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

    //calculating normal vector for vertices
    std::vector<QVector3D> vertexNormals;
    //normal vertex, top border, bottom border, left border, right border, top left, top right, bottom left, bottom right
    std::array<int, 9> vertexResults {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < mVertices.size(); i++)
    {
        QVector3D vertexNormal{0, 0, 0};

        //Is it on the top border
        if (i < amountOfQuadsX){
            //Is it on the left corner
            if (i % amountOfQuadsX == 0 ){
                //there is 1 triangle using it

                //Triangle 1, bottom right
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i).x),
                    (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i).y),
                    (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i+1).x - mVertices.at(i).x),
                    (mVertices.at(i+1).y - mVertices.at(i).y),
                    (mVertices.at(i+1).z - mVertices.at(i).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(5)++;
            }
             //Is it on the right corner
            else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){
                //there is 1 triangle using it

                //Triangle 1, bottom left
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i-1).x),
                    (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i-1).y),
                    (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i-1).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-1).x),
                    (mVertices.at(i).y - mVertices.at(i-1).y),
                    (mVertices.at(i).z - mVertices.at(i-1).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(6)++;

            }
            //Is a normal border
            else{
                //there are 2 triangles using it

                //Triangle 1, bottom left
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i-1).x),
                    (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i-1).y),
                    (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i-1).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-1).x),
                    (mVertices.at(i).y - mVertices.at(i-1).y),
                    (mVertices.at(i).z - mVertices.at(i-1).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);

                //Triangle 2, bottom right
                vector1 = QVector3D(
                    (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i).x),
                    (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i).y),
                    (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i).z)
                    );
                vector2 = QVector3D(
                    (mVertices.at(i+1).x - mVertices.at(i).x),
                    (mVertices.at(i+1).y - mVertices.at(i).y),
                    (mVertices.at(i+1).z - mVertices.at(i).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(1)++;
            }
        }
        //Is it on the bottom border
        else if(i >= mVertices.size() - amountOfQuadsX){
            //Is it on the left corner
            if (i % amountOfQuadsX == 0 ){
                //there is 1 triangle using it

                //Triangle 1, top right
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i+1).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i+1).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i+1).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(7)++;

            }
            //Is it on the right corner
            else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){
                //there is 1 triangle using it

                //Triangle 1, top left
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i-1).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i-1).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i-1).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(8)++;

            }
            //Is a normal border
            else{
                //there are 2 triangles using it
                //Triangle 1, top left
                QVector3D vector1 = QVector3D(
                    (mVertices.at(i-1).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i-1).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i-1).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                QVector3D vector2 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);

                //Triangle 2, top right
                vector1 = QVector3D(
                    (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                vector2 = QVector3D(
                    (mVertices.at(i+1).x - mVertices.at(i-amountOfQuadsX).x),
                    (mVertices.at(i+1).y - mVertices.at(i-amountOfQuadsX).y),
                    (mVertices.at(i+1).z - mVertices.at(i-amountOfQuadsX).z)
                    );
                vertexNormal += QVector3D::crossProduct(vector1, vector2);
                vertexResults.at(2)++;
            }
        }
        //Is it on the left border
        else if (i % amountOfQuadsX == 0 ){
            //there are 2 triangles using it


            //Triangle 1, top right
            QVector3D vector1 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                );
            QVector3D vector2 = QVector3D(
                (mVertices.at(i+1).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i+1).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i+1).z - mVertices.at(i-amountOfQuadsX).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);

            //Triangle 2, bottom right
            vector1 = QVector3D(
                (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i).x),
                (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i).y),
                (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i).z)
                );
            vector2 = QVector3D(
                (mVertices.at(i+1).x - mVertices.at(i).x),
                (mVertices.at(i+1).y - mVertices.at(i).y),
                (mVertices.at(i+1).z - mVertices.at(i).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);
            vertexResults.at(3)++;

        }
        //Is it on the right border
        else if(i % amountOfQuadsX == (amountOfQuadsX - 1)){
            //there are 2 triangles using it

            //Triangle 1, top left
            QVector3D vector1 = QVector3D(
                (mVertices.at(i-1).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i-1).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i-1).z - mVertices.at(i-amountOfQuadsX).z)
                );
            QVector3D vector2 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);

            //Triangle 2, bottom left
            vector1 = QVector3D(
                (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i-1).x),
                (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i-1).y),
                (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i-1).z)
                );
            vector2 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-1).x),
                (mVertices.at(i).y - mVertices.at(i-1).y),
                (mVertices.at(i).z - mVertices.at(i-1).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);
            vertexResults.at(4)++;

        }
        //Is a normal vertex
        else{
            //there are 4 triangles using it
            //Triangle 1, top left
            QVector3D vector1 = QVector3D(
                (mVertices.at(i-1).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i-1).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i-1).z - mVertices.at(i-amountOfQuadsX).z)
                );
            QVector3D vector2 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);

            //Triangle 2, top right
            vector1 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i).z - mVertices.at(i-amountOfQuadsX).z)
                );
            vector2 = QVector3D(
                (mVertices.at(i+1).x - mVertices.at(i-amountOfQuadsX).x),
                (mVertices.at(i+1).y - mVertices.at(i-amountOfQuadsX).y),
                (mVertices.at(i+1).z - mVertices.at(i-amountOfQuadsX).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);

            //Triangle 3, bottom left
            vector1 = QVector3D(
                (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i-1).x),
                (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i-1).y),
                (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i-1).z)
                );
            vector2 = QVector3D(
                (mVertices.at(i).x - mVertices.at(i-1).x),
                (mVertices.at(i).y - mVertices.at(i-1).y),
                (mVertices.at(i).z - mVertices.at(i-1).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);

            //Triangle 4, bottom right
            vector1 = QVector3D(
                (mVertices.at(i+amountOfQuadsX).x - mVertices.at(i).x),
                (mVertices.at(i+amountOfQuadsX).y - mVertices.at(i).y),
                (mVertices.at(i+amountOfQuadsX).z - mVertices.at(i).z)
                );
            vector2 = QVector3D(
                (mVertices.at(i+1).x - mVertices.at(i).x),
                (mVertices.at(i+1).y - mVertices.at(i).y),
                (mVertices.at(i+1).z - mVertices.at(i).z)
                );
            vertexNormal += QVector3D::crossProduct(vector1, vector2);
            vertexResults.at(0)++;
        }
        vertexNormal.normalize();

        vertexNormals.push_back(vertexNormal);
    }



    //I thought i needed it, so i made a quick/bad calculation of texture coordinates
    //Turns out i do actually need it, since the game engine does not check whether or not an .obj has texture vertices :)

    std::vector<std::array<float, 2>> vertexTextures;
    for (int i = 0; i < amountOfQuadsZ; i++)
    {
        //i want to keep decimals, so one of the parts of the division should be a float
        float floatDivI = i;

        for (int j = 0; j < amountOfQuadsX; j++){
            float floatDivJ = j;
            vertexTextures.push_back(std::array<float, 2>{floatDivJ/(amountOfQuadsX-1), floatDivI/(amountOfQuadsZ-1)});
        }
    }

    //Printing to .obj
    std::ofstream out("lasData.obj");
    if (!out.is_open())
        return;
    out << "o Kittel\n";

    for (Vertex v : mVertices){
        out << "v " << std::setprecision(6) << v.x << " " << v.y << " " << v.z << "\n";
    }

    for (auto vt : vertexTextures){
        out << "vt " << std::setprecision(6) << vt.at(0) << " " << vt.at(1) << "\n";
    }

    for (QVector3D n : vertexNormals){
        out << "vn " << std::setprecision(6) << n.x() << " " << n.y() << " " << n.z() << "\n";
    }
    for (int i = 0; i < mIndices.size(); i += 3){
        //Obj uses base 1 indices
        out << "f " << (mIndices.at(i) + 1) << "/" << (mIndices.at(i) + 1) << "/" << (mIndices.at(i) + 1) << " "
            << (mIndices.at(i + 1) + 1) << "/" << (mIndices.at(i + 1) + 1) << "/" << (mIndices.at(i + 1) + 1) << " "
            << (mIndices.at(i + 2) + 1) << "/" << (mIndices.at(i + 2) + 1) << "/" << (mIndices.at(i + 2) + 1) << "\n";
    }
    out.close();

}
