#ifndef TRIANGULERING_H
#define TRIANGULERING_H

#include <vector>
#include "Vertex.h"
#include <math.h>



std::vector<Vertex> triangulate(std::vector<Vertex> inVertices){
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
    for (int i = 0; i < amountOfQuadsX; i++){
        for (int j = 0; j < amountOfQuadsZ; j++){
            quads.push_back(Vertex((i + 0.5f) * quadLength, 0, (j + 0.5f) * quadWidth, 0, 1, 0, 0, 0));
        }
    }

    for (Vertex v : inVertices){
        float vPositionX = floor(v.x/quadLength);
        float vPositionZ = floor(v.z/quadWidth);

        quads.at(vPositionX * amountOfQuadsX + vPositionZ).y = (quads.at(vPositionX * amountOfQuadsX + vPositionZ).y + v.y) / 2;
    }

    return quads;

};
#endif // TRIANGULERING_H
