#ifndef BULLET_H
#define BULLET_H

#include "VisualObject.h"

class Bullet : public VisualObject
{
public:
    Bullet();

    float collisionRadius = 0.2f;
};

#endif // BULLET_H
