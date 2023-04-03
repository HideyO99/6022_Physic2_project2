#include "Physic.h"

Physic::Physic()
{
	Factory = nullptr;
	World = nullptr;
}

Physic::~Physic()
{
	delete Factory;
	delete World;
}

void Physic::init()
{
	Factory = new PhysicFactory();
	World = Factory->createWorld();

	collisionListener = new CollisionListener();
	World->setGravity(glm::vec3(0, -9.8f, 0));
	World->addToCollisionListener(collisionListener);

#if !SHOWWALL
	createPlane();
#endif
}

void Physic::destroy()
{

	
}

void Physic::update(float dt)
{
	World->timeStep(dt);
	for (int i = 0; i < m_ballList.size(); i++)
	{
		cObject* ball = m_ballList[i];
		iRigidBody* rigidBody = dynamic_cast<iRigidBody*>(ball->PhysicBody);
		if (ball->PhysicBody != nullptr)
		{
			ball->pMeshObj->position = rigidBody->getPosition();
			ball->pMeshObj->rotation = glm::eulerAngles(rigidBody->getRotation());
		}
	}
}

void Physic::UserForce(glm::vec3 dir)
{
	for (int i = 0; i < m_ballList.size(); i++)
	{
		if (m_ballList[i]->isCTRL)
		{
			cObject* ball = m_ballList[i];
			iRigidBody* rigidBody = dynamic_cast<iRigidBody*>(ball->PhysicBody);
			if (ball->PhysicBody != nullptr)
			{
				rigidBody->addForce(dir * 5.f);
			}
		}
	}
}

void Physic::setActive(int num)
{
	for (int i = 0; i < m_ballList.size(); i++)
	{
		m_ballList[i]->isCTRL = false;
		m_ballList[i]->pMeshObj->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 1.f);
	}
	m_ballList[num]->isCTRL = true;
	m_ballList[num]->pMeshObj->color_RGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
}

#if !SHOWWALL
void Physic::createPlane()
{
	cObject* ground = new cObject();
	iShape* groundShape = new iPlaneShape(glm::vec3(0.f, 1.f, 0.f), 0.f);
	iRigidBodyDesc groundDesc;
	groundDesc.bStatic = true;
	groundDesc.mass = 0;
	groundDesc.position = glm::vec3(0.f);
	groundDesc.velocity = glm::vec3(0.f);
	ground->PhysicBody = Factory->createRigidBody(groundDesc, groundShape);
	World->addBody(ground->PhysicBody);

	cObject* wallN = new cObject();
	iShape* wallNShape = new iPlaneShape(glm::vec3(0.f, 0.f, 1.f), 0.f);
	iRigidBodyDesc wallNDesc;
	wallNDesc.bStatic = true;
	wallNDesc.mass = 0;
	wallNDesc.position = glm::vec3(0, 0, -50);
	wallNDesc.velocity = glm::vec3(0.f);
	wallN->PhysicBody = Factory->createRigidBody(wallNDesc, wallNShape);
	World->addBody(wallN->PhysicBody);

	cObject* wallE = new cObject();
	iShape* wallEShape = new iPlaneShape(glm::vec3(-1.f, 0.f, 0.f), 0.f);
	iRigidBodyDesc wallEDesc;
	wallEDesc.bStatic = true;
	wallEDesc.mass = 0;
	wallEDesc.position = glm::vec3(50, 0, 0);
	wallEDesc.velocity = glm::vec3(0.f);
	wallE->PhysicBody = Factory->createRigidBody(wallEDesc, wallEShape);
	World->addBody(wallE->PhysicBody);

	cObject* wallW = new cObject();
	iShape* wallWShape = new iPlaneShape(glm::vec3(1.f, 0.f, 0.f), 0.f);
	iRigidBodyDesc wallWDesc;
	wallWDesc.bStatic = true;
	wallWDesc.mass = 0;
	wallWDesc.position = glm::vec3(-50, 0, 0);
	wallWDesc.velocity = glm::vec3(0.f);
	wallW->PhysicBody = Factory->createRigidBody(wallWDesc, wallWShape);
	World->addBody(wallW->PhysicBody);

	cObject* wallS = new cObject();
	iShape* wallSShape = new iPlaneShape(glm::vec3(0.f, 0.f, -1.f), 0.f);
	iRigidBodyDesc wallSDesc;
	wallSDesc.bStatic = true;
	wallSDesc.mass = 0;
	wallSDesc.position = glm::vec3(0, 0, 50);
	wallSDesc.velocity = glm::vec3(0.f);
	wallS->PhysicBody = Factory->createRigidBody(wallSDesc, wallSShape);
	World->addBody(wallS->PhysicBody);
}

#endif
#if SHOWWALL
void Physic::createPlane(cMeshObj* mGround, cMeshObj* mWallN, cMeshObj* mWallE, cMeshObj* mWallW, cMeshObj* mWallS)
{
	cObject* ground = new cObject();
	ground->pMeshObj = mGround;
	ground->pMeshObj->position = glm::vec3(0, 0, 0);;
	ground->pMeshObj->scale = glm::vec3(100, 100, 1);
	ground->pMeshObj->rotation = glm::vec3(glm::radians(-90.f),0,0);
	iShape* groundShape = new iPlaneShape( glm::vec3(0.f, 1.f, 0.f), 0.f);
	iRigidBodyDesc groundDesc;
	groundDesc.bStatic = true;
	groundDesc.mass = 0;
	groundDesc.position = glm::vec3(0.f);
	groundDesc.velocity = glm::vec3(0.f);
	ground->PhysicBody = Factory->createRigidBody(groundDesc, groundShape);
	World->addBody(ground->PhysicBody);

	cObject* wallN = new cObject();
	wallN->pMeshObj = mWallN;
	wallN->pMeshObj->position = glm::vec3(0, 0, -50);
	wallN->pMeshObj->scale = glm::vec3(100, 100, 1);
	wallN->pMeshObj->rotation = glm::vec3(0, 0, 0);
	iShape* wallNShape = new iPlaneShape(glm::vec3(0.f, 0.f, 1.f), 0.f);
	iRigidBodyDesc wallNDesc;
	wallNDesc.bStatic = true;
	wallNDesc.mass = 0;
	wallNDesc.position = glm::vec3(0, 0, -50);
	wallNDesc.velocity = glm::vec3(0.f);
	wallN->PhysicBody = Factory->createRigidBody(wallNDesc, wallNShape);
	World->addBody(wallN->PhysicBody);

	cObject* wallE = new cObject();
	wallE->pMeshObj = mWallE;
	wallE->pMeshObj->position = glm::vec3(50, 0, 0);
	wallE->pMeshObj->scale = glm::vec3(100, 100, 1);
	wallE->pMeshObj->rotation = glm::vec3(0, glm::radians(90.f), 0);
	iShape* wallEShape = new iPlaneShape(glm::vec3(-1.f, 0.f, 0.f), 0.f);
	iRigidBodyDesc wallEDesc;
	wallEDesc.bStatic = true;
	wallEDesc.mass = 0;
	wallEDesc.position = glm::vec3(50, 0, 0);
	wallEDesc.velocity = glm::vec3(0.f);
	wallE->PhysicBody = Factory->createRigidBody(wallEDesc, wallEShape);
	World->addBody(wallE->PhysicBody);

	cObject* wallW = new cObject();
	wallW->pMeshObj = mWallW;
	wallW->pMeshObj->position = glm::vec3(-50, 0, 0);
	wallW->pMeshObj->scale = glm::vec3(100, 100, 1);
	wallW->pMeshObj->rotation = glm::vec3(0, glm::radians(90.f), 0);
	iShape* wallWShape = new iPlaneShape(glm::vec3(1.f, 0.f, 0.f), 0.f);
	iRigidBodyDesc wallWDesc;
	wallWDesc.bStatic = true;
	wallWDesc.mass = 0;
	wallWDesc.position = glm::vec3(-50, 0, 0);
	wallWDesc.velocity = glm::vec3(0.f);
	wallW->PhysicBody = Factory->createRigidBody(wallWDesc, wallWShape);
	World->addBody(wallW->PhysicBody);

	cObject* wallS = new cObject();
	wallS->pMeshObj = mWallS;
	wallS->pMeshObj->position = glm::vec3(0, 0, 50);
	wallS->pMeshObj->scale = glm::vec3(100, 100, 1);
	wallS->pMeshObj->rotation = glm::vec3(0, glm::radians(0.f), 0);
	iShape* wallSShape = new iPlaneShape(glm::vec3(0.f, 0.f, -1.f), 0.f);
	iRigidBodyDesc wallSDesc;
	wallSDesc.bStatic = true;
	wallSDesc.mass = 0;
	wallSDesc.position = glm::vec3(0, 0, 50);
	wallSDesc.velocity = glm::vec3(0.f);
	wallS->PhysicBody = Factory->createRigidBody(wallSDesc, wallSShape);
	World->addBody(wallS->PhysicBody);

	//wallN->pMeshObj->isWireframe = true;
	//wallE->pMeshObj->isWireframe = true;
	//wallW->pMeshObj->isWireframe = true;
	//wallS->pMeshObj->isWireframe = true;

	wallN->pMeshObj->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 0.5f);
	wallE->pMeshObj->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 0.5f);
	wallW->pMeshObj->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 0.5f);
	wallS->pMeshObj->color_RGBA = glm::vec4(0.f, 0.f, 0.f, 0.5f);
	
}
#endif

void Physic::createBall(cMeshObj* mOBJ, glm::vec3 pos, float size)
{
	cObject* ball = new cObject();
	ball->pMeshObj = mOBJ;
	ball->pMeshObj->position = pos;
	ball->pMeshObj->scale = glm::vec3(size);
	ball->position = ball->pMeshObj->position;
	iShape* ballShape = new iSphereShape(size);
	iRigidBodyDesc desc;
	desc.bStatic = false;
	desc.mass = size;
	desc.position = ball->position;
	desc.velocity = glm::vec3(0.f);
	ball->PhysicBody = Factory->createRigidBody(desc, ballShape);
	World->addBody(ball->PhysicBody);
	m_ballList.push_back(ball);
}

void Physic::createBox(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size)
{
	cObject* box = new cObject();

	box->pMeshObj = mOBJ;
	box->pMeshObj->position = pos;
	//box->pMeshObj->scale = size;
	box->position = box->pMeshObj->position;
	glm::vec3 halfExtent = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
	iShape* boxShape = new iBoxShape(halfExtent);
	iRigidBodyDesc desc;
	desc.bStatic = false;
	desc.mass = 1;
	desc.position = box->position;
	desc.velocity = glm::vec3(0.f);
	box->PhysicBody = Factory->createRigidBody(desc, boxShape);
	World->addBody(box->PhysicBody);
	//m_ballList.push_back(box);
}

void Physic::createFloor(cMeshObj* mGround)
{
	float size = 32.f;
	cObject* floor = new cObject();
	floor->pMeshObj = mGround;
	//floor->pMeshObj->position = glm::vec3(0.f);
	//floor->pMeshObj->scale = glm::vec3(size);
	floor->position = floor->pMeshObj->position;
	glm::vec3 halfExtent = glm::vec3(size / 2, 1, size / 2);
	iShape* boxShape = new iBoxShape(halfExtent);
	iRigidBodyDesc desc;
	desc.bStatic = true;
	desc.mass = 0;
	desc.position = floor->position;
	desc.velocity = glm::vec3(0.f);
	floor->PhysicBody = Factory->createRigidBody(desc, boxShape);
	World->addBody(floor->PhysicBody);
}

void Physic::createCylinder(cMeshObj* mOBJ, glm::vec3 pos, glm::vec3 size)
{
	cObject* cylinder = new cObject();
	cylinder->pMeshObj = mOBJ;
	//cylinder->pMeshObj->position = pos;
	cylinder->position = cylinder->pMeshObj->position;
	glm::vec3 halfExtent = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
	iShape* cylinderShape = new iCylinderShape(halfExtent);
	iRigidBodyDesc desc;
	desc.bStatic = false;
	desc.mass = 1;
	desc.position = cylinder->position;
	desc.velocity = glm::vec3(0.f);
	cylinder->PhysicBody = Factory->createRigidBody(desc, cylinderShape);
	World->addBody(cylinder->PhysicBody);
}

