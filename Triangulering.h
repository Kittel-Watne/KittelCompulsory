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
    int amountOfQuadsX = 100;
    int amountOfQuadsZ = 100;

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
        if (v.z >= maxZ - (10/amountOfQuadsZ)){
            vPositionZ = (amountOfQuadsZ-1);
        }
        else{
            vPositionZ = floor((v.z - minZ)/quadWidth);
        }

        if (vPositionX >= 0 && vPositionX <= amountOfQuadsX-1 && vPositionZ >= 0 && vPositionZ <= amountOfQuadsZ-1){
        //temporarily using the color of the vertex to count how many points are at each quad
        quads.at(vPositionZ * amountOfQuadsX + vPositionX).r++;
        quads.at(vPositionZ * amountOfQuadsX + vPositionX).y = (quads.at(vPositionZ * amountOfQuadsX + vPositionX).y + v.y);
        }
    }
    //dividing to find average height
    for (int i = 0; i< quads.size(); i++){
        if (quads.at(i).r != 0){
            quads.at(i).y = quads.at(i).y / quads.at(i).r;
        }
    }
    std::vector<int> approximationList;
    int loopItr = 10;

    for (int h = 0; h < loopItr; h++){

        for (int i = 0; i< quads.size(); i++)
            if (quads.at(i).r == 0)
                approximationList.push_back(i);

        //filling in incomplete information
        for (int i = 0; i< approximationList.size(); i++)
        {
            float neighborSum = 0.f;
            int sums = 0;

            if (approximationList.at(i) > 1 && quads.at(approximationList.at(i) - 1).r > 0){
                neighborSum += quads.at(approximationList.at(i) - 1).y;
                sums++;
            }
            if (approximationList.at(i) > amountOfQuadsX && quads.at(approximationList.at(i) - amountOfQuadsX).r > 0){
                neighborSum += quads.at(approximationList.at(i) - amountOfQuadsX).y;
                sums++;
            }
            if (approximationList.at(i) < quads.size() - 1 && quads.at(approximationList.at(i) + 1).r > 0){
                neighborSum += quads.at(approximationList.at(i) + 1).y;
                sums++;
            }
            if (approximationList.at(i) < quads.size() - amountOfQuadsX && quads.at(approximationList.at(i) + amountOfQuadsX).r > 0){
                neighborSum += quads.at(approximationList.at(i) + amountOfQuadsX).y;
                sums++;
            }

            if (sums > 2)
                quads.at(approximationList.at(i)).r = 1;
            if (sums > 0)
                quads.at(approximationList.at(i)).y = neighborSum/sums;
        }
        approximationList.clear();
        //On final iteration
        if (h + 1 == loopItr)
            //refill approximationlist

            for(int j = 0; j< quads.size(); j++){
                //Approximate one last time without quality assurance
                if (quads.at(j).r == 0){
                    float neighborSum = 0.f;
                    int sums = 0;

                    if (j > 1 ){
                        neighborSum += quads.at(j - 1).y;
                        sums++;
                    }
                    if (j > amountOfQuadsX ){
                        neighborSum += quads.at(j - amountOfQuadsX).y;
                        sums++;
                    }
                    if (j < quads.size() - 1 ){
                        neighborSum += quads.at(j + 1).y;
                        sums++;
                    }
                    if (j < quads.size() - amountOfQuadsX ){
                        neighborSum += quads.at(j + amountOfQuadsX).y;
                        sums++;
                    }
                    if (sums > 0)
                        quads.at(j).y = neighborSum/sums;
                    else{
                        quads.at(j).y = 1;
                    }
                }
            }
    }

    //removing color used for categorizing during approximation
    for (int i = 0; i< quads.size(); i++){
        quads.at(i).r = 0;
    }

    std::vector<Vertex> vertices;
    //This is incomplete and deletes the outermost part of the model
    for (int i = 0; i < amountOfQuadsZ; i++){
        for (int j = 0; j < amountOfQuadsX; j++){
            vertices.push_back(Vertex(j * quadLength, quads.at(i * amountOfQuadsX + j).y, i * quadWidth, 0, 1, 0, 0, 0));
            //std::cout << vertices.back() << "\n";
        }
    }


    return vertices;

};
#endif // TRIANGULERING_H
