#ifndef TROPHIES_H
#define TROPHIES_H

#include "VisualObject.h"

class Trophies : public VisualObject
{
public:
    Trophies(const std::string& filename, int colorType);

private:
    bool readObjFile(const std::string& filename);
};

#endif // TROPHIES_H
