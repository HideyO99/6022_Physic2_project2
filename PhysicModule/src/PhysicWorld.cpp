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

void PhysicWorld::setGravity(const Vec3& gravity)
{
	m_gravity = gravity;
}

void PhysicWorld::addBody(iCollision* body)
{
	if (body == nullptr)
	{
		return;
	}
	
	//find existing body if not found then push new body
	if (std::find(m_body.begin(), m_body.end(), body) == m_body.end())
	{
		m_body.push_back(body);
	}
	switch (body->getBodyType())
	{
	case BodyType::RigidBody:
		addRigid(body);
		break;
	default:
		break;
	}
}

void PhysicWorld::removeBody(iCollision* body)
{
	if (body == nullptr)
	{
		return;
	}

	switch (body->getBodyType())
	{
	case BodyType::RigidBody:
		removeRigid(body);
		break;
	default:
		break;
	}

	m_body.erase(std::find(m_body.begin(), m_body.end(), body));
	
}

void PhysicWorld::timeStep(float dt)
{
}

void PhysicWorld::addRigid(iCollision* body)
{
	RigidBody* rigid = RigidBody::cast(body);

	if (std::find(m_rigidBody.begin(), m_rigidBody.end(), rigid) == m_rigidBody.end())
	{
		m_rigidBody.push_back(rigid);
	}
}

void PhysicWorld::removeRigid(iCollision* body)
{
	RigidBody* rigid = RigidBody::cast(body);
	m_rigidBody.erase(std::find(m_rigidBody.begin(), m_rigidBody.end(), rigid));

}
