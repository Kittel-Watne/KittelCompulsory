#ifndef ENEMYCHARACTER_H
#define ENEMYCHARACTER_H

#include "VisualObject.h"

class EnemyCharacter : public VisualObject
{
public:
    EnemyCharacter(const std::string& filename);

    float mCollisionRadius = 0.5f;
    void walking();
    void turnAround();
private:
    bool readObjFile(const std::string& filename);
    float mWalkSpeed = 0.01f;
};

#endif // ENEMYCHARACTER_H
