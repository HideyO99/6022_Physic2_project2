#pragma once
#include "iCollision.h"
#include "iShape.h"

class iRigidBody : public iCollision
{
public:
	virtual ~iRigidBody() {}

	virtual void getPos(Vec3& pos) = 0;



};

