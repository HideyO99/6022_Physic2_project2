#pragma once
#include "../MeshObj/cMeshObj.h"
#include <iCollision.h>


#define SEEK_COLOR		glm::vec4(1,0,0,1)
#define FLEE_COLOR		glm::vec4(0,0,1,1)
#define PURSUE_COLOR	glm::vec4(1,1,0,1)
#define EVADE_COLOR		glm::vec4(0,1,1,1)
#define APPROCH_COLOR	glm::vec4(1,0,1,1)

class cObject
{
public:
	cObject();
	~cObject();
	
	//void setMass(float mass);
	//void integrate(float dT);
	//void applyForce(const glm::vec3& f_dir);
	//void killAllForces();
	//void update();
	//void AI_update(cObject* playerObj);

	cMeshObj* pMeshObj;

	iCollision* PhysicBody;

	std::string objName;
	glm::vec3 position;
	//glm::vec3 prevPosition;
	glm::vec3 direction;
	glm::vec3 velocity;
	//glm::vec3 acceleration;
	//glm::vec3 force;
	glm::quat qRotation;
	//float damping;
	//float mass;
	bool isCTRL;
	//float yRotation;
	//bool isHover;
};

