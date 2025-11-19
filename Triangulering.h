#ifndef TRIANGULERING_H
#define TRIANGULERING_H

#include <vector>
#include "Vertex.h"
#include <math.h>


void triangulate(std::vector<Vertex> inVertices){
    //std::vector<float> verticesCopy = inVertices;
    float maxX = -1.f;
    float minX = 1000.0f;
    float maxZ = -1.f;
    float minZ = 1000.0f;


    for (Vertex v : inVertices) {
        if (v.x > maxX)
            maxX = v.x;
        if (v.x < minX)
            minX = v.x;
        if (v.z > maxZ)
            maxZ = v.z;
        if (v.z < minZ)
            minZ = v.z;
    }
    int amountOfQuadsX = 10;
    int amountOfQuadsZ = 10;

    float quadLength = (maxX - minX)/amountOfQuadsX;
    float quadWidth = (maxZ - minZ)/amountOfQuadsZ;

    std::vector<Vertex> quads;
    for (int i = 0; i < amountOfQuadsZ; i++){
        for (int j = 0; j < amountOfQuadsX; j++){
            quads.push_back(Vertex((j + 0.5f) * quadLength, 0, (i + 0.5f) * quadWidth, 0, 1, 0, 0, 0));
        }
    }

    for (Vertex v : inVertices){
        float vPositionX;
        float vPositionZ;
        if (v.x >= ((amountOfQuadsX-1) * quadLength) + minX){
            vPositionX = (amountOfQuadsX-1);
        }
        else{
        vPositionX = floor((v.x - minX)/quadLength);
        }
        if (v.z >= (amountOfQuadsZ-1) * quadWidth + minZ){
            vPositionZ = (amountOfQuadsZ-1);
        }
        else{
            vPositionZ = floor((v.z - minZ)/quadWidth);
        }
        quads.at(vPositionZ * amountOfQuadsX + vPositionX).y = (quads.at(vPositionZ * amountOfQuadsX + vPositionX).y + v.y) / 2;
    }
    std::vector<Vertex> vertices;
    //This is incomplete and deletes the outermost part of the model
    for (int i = 0; i < amountOfQuadsZ; i++){
        for (int j = 0; j < amountOfQuadsX; j++){
            vertices.push_back(Vertex(j * quadLength, quads.at(i * amountOfQuadsX + j).y, i * quadWidth, 0, 1, 0, 0, 0));
        }
    }

    std::vector<int> indices;

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

            //std::cout << indices.at(i * 6 * (amountOfQuadsX - 1) + (j * 6)) << " " << indices.at(i * 6 *(amountOfQuadsX - 1) + (j * 6) + 1) << " " << indices.at(i * 6 *(amountOfQuadsX - 1) + (j * 6) + 2) << "\n";
            //std::cout << indices.at(i * 6 * (amountOfQuadsX - 1) + (j * 6) + 3) << " " << indices.at(i * 6 *(amountOfQuadsX - 1) + (j * 6) + 4) << " " << indices.at(i * 6 *(amountOfQuadsX - 1) + (j * 6) + 5) << "\n";
        }
    }

    return;

};
#endif // TRIANGULERING_H
