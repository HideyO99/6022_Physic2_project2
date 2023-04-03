#pragma once
#include <iPhysicFactory.h>
#include <iPhysicWorld.h>

#include "cObject.h"
#include "CollisionListener.h"

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
	void UserForce(glm::vec3 dir);
	void setActive(int num);

//private:
#if SHOWWALL
	void createPlane(cMeshObj* mGround, cMeshObj* mWallN, cMeshObj* mWallE, cMeshObj* mWallW, cMeshObj* mWallS);
#else
	void createPlane();
#endif
	void createBall(cMeshObj* mOBJ,float size);
	void createBox(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size);
	void createFloor(cMeshObj* mGround);
	void createCylinder(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size);
	
	CollisionListener* collisionListener;
	iPhysicFactory* Factory;
	iPhysicWorld* World;
	std::vector<cObject*> m_ballList;
};

