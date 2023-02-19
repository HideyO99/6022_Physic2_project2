#include "PhysicWorld.h"

PhysicWorld::PhysicWorld()
	:iPhysicWorld()
	,m_collisionHandler(nullptr)
{
	m_collisionHandler = new CollisionHandler();
}

PhysicWorld::~PhysicWorld()
{
	m_rigidBody.clear();
	m_body.clear();

	delete m_collisionHandler;
	delete m_collisionListener;
}
