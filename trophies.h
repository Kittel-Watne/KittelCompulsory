#ifndef TROPHIES_H
#define TROPHIES_H

#include "VisualObject.h"

class Trophies : public VisualObject
{
public:
    Trophies(const std::string& filename, int colorType);
    void becomeWhite();
    float collisionRadius = 0.1f;

    std::vector<Vertex> mOriginalVertices;
    std::vector<Vertex> mWhiteVertices;
private:
    bool readObjFile(const std::string& filename);


};

#endif // TROPHIES_H
