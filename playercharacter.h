#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H


#include "VisualObject.h"
#include <string>

class PlayerCharacter : public VisualObject
{
public:
    PlayerCharacter(const std::string& filename);
private:
    bool readObjFile(const std::string& filename);
};

#endif // PLAYERCHARACTER_H
