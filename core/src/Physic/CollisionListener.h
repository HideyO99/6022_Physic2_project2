#pragma once
#include <iCollisionListener.h>
class CollisionListener :
    public iCollisionListener
{
public:
    CollisionListener();
    virtual ~CollisionListener();

    virtual void notifyCollision(iCollision* body1, iCollision* body2) override;
};

