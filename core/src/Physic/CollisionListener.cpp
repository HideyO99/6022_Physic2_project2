#include "CollisionListener.h"

CollisionListener::CollisionListener()
{
}

CollisionListener::~CollisionListener()
{
}

void CollisionListener::notifyCollision(iCollision* body1, iCollision* body2)
{
	fmod->stop_sound(FX1_CH);
	fmod->play_sound("fx1", FX1_CH);

}
