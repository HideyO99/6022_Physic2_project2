#pragma once
#include <iPhysicFactory.h>
#include <iPhysicWorld.h>

#include "cObject.h"

#define GDP_Physics
#if defined GDP_Physics
#pragma comment(lib, "PhysicModule.lib")
#include <PhysicFactory.h>
#endif

#define SHOWWALL true

class Physic
{
public:
	Physic();
	~Physic();

	void init();
	void destroy();
	void update(float dt);

//private:
#if SHOWWALL
	void createPlane(cMeshObj* mGround, cMeshObj* mWallN, cMeshObj* mWallE, cMeshObj* mWallW, cMeshObj* mWallS);
#else
	void createPlane();
#endif
	void createBall(cMeshObj* mOBJ);
	
	
	iPhysicFactory* Factory;
	iPhysicWorld* World;
};

