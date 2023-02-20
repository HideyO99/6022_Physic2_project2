#include "CollisionListener.h"

CollisionListener::CollisionListener()
{
}

CollisionListener::~CollisionListener()
{
}

void CollisionListener::notifyCollision(iCollision* body1, iCollision* body2)
{
	fmod->play_sound("fx1", FX1_CH);
}
