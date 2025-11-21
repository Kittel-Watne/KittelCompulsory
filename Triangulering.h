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
            if (v.x < maxX + 5000)
                maxX = v.x;
        if (v.x < minX)
            if (v.x > minX - 5000)
                minX = v.x;
        if (v.z > maxZ)
            if (v.z < maxZ + 5000)
            maxZ = v.z;
        if (v.z < minZ)
            if (v.z > minZ - 5000)
            minZ = v.z;
    }
    int amountOfQuadsX = 10;
    int amountOfQuadsZ = 10;

    float quadLength = (maxX - minX)/amountOfQuadsX;
    float quadWidth = (maxZ - minZ)/amountOfQuadsZ;

    std::vector<Vertex> quads;
    for (int i = 0; i < amountOfQuadsZ; i++){
        for (int j = 0; j < amountOfQuadsX; j++){
            quads.push_back(Vertex((j + 0.5f) * quadLength, 0.f, (i + 0.5f) * quadWidth, 0, 1, 0, 0, 0));
        }
    }

    for (Vertex v : inVertices){
        float vPositionX;
        float vPositionZ;

        //If the position is maximum or greater, it is forced to maximum
        if (v.x >= maxX){
            vPositionX = (amountOfQuadsX-1);
        }
        else{
        vPositionX = floor((v.x - minX)/quadLength);
        }
        //If the position is maximum or greater, it is forced to maximum
        if (v.z >= maxZ - 1){
            vPositionZ = (amountOfQuadsZ-1);
        }
        else{
            vPositionZ = floor((v.z - minZ)/quadWidth);
        }

        if (vPositionX >= 0 && vPositionX <= 9 && vPositionZ >= 0 && vPositionZ <= 9){
        //temporarily using the color of the vertex to count how many points are at each quad
        quads.at(vPositionZ * amountOfQuadsX + vPositionX).r++;
        quads.at(vPositionZ * amountOfQuadsX + vPositionX).y = (quads.at(vPositionZ * amountOfQuadsX + vPositionX).y + v.y);
        }
    }
    //filling in incomplete information
    for (int i = 0; i< quads.size(); i++)
    {
        if (quads.at(i).r == 0){
            float neighborSum = 0.f;
            int sums = 0;
            if (i > 0){
                neighborSum += quads.at(i - 1).y;
                sums++;
            }
            if (i >= amountOfQuadsX){
                neighborSum += quads.at(i - amountOfQuadsX).y;
                sums++;
            }
            if (i < quads.size() - 1){
                neighborSum += quads.at(i + 1).y;
                sums++;
            }
            if (i < quads.size() - amountOfQuadsX){
                neighborSum += quads.at(i + amountOfQuadsX).y;
                sums++;
            }
            //Finding the average of the neighbor-sums
            if (sums > 0){
                quads.at(i).y = neighborSum / sums;
            }
            else{
                quads.at(i).y = 1;
            }
        }
    }
    for (Vertex q : quads){
        if (q.r != 0){
        q.y = q.y / q.r;
        q.r = 0.f;
        std::cout << q.x << " " << q.y << " " << q.z << "\n";
        }
    }

    std::vector<Vertex> vertices;
    //This is incomplete and deletes the outermost part of the model
    for (int i = 0; i < amountOfQuadsZ; i++){
        for (int j = 0; j < amountOfQuadsX; j++){
            vertices.push_back(Vertex(j * quadLength, quads.at(i * amountOfQuadsX + j).y, i * quadWidth, 0, 1, 0, 0, 0));
            //std::cout << vertices.back() << "\n";
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
