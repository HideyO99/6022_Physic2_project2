#include "Physic.h"

Physic::Physic()
{
	Factory = nullptr;
	World = nullptr;
}

Physic::~Physic()
{
}

void Physic::init()
{
	Factory = new PhysicFactory();
	World = Factory->createWorld();

	World->setGravity(glm::vec3(0, -9.8f, 0));

#if !SHOWWALL
	createPlane();
#endif
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

	wallN->pMeshObj->isWireframe = true;
	wallE->pMeshObj->isWireframe = true;
	wallW->pMeshObj->isWireframe = true;
	wallS->pMeshObj->isWireframe = true;
}
#endif

void Physic::createBall(cMeshObj* mOBJ)
{
	cObject* ball = new cObject();
	ball->pMeshObj = mOBJ;
	ball->pMeshObj->position.y = 50;
	ball->pMeshObj->scale = glm::vec3(100, 100, 1);
}