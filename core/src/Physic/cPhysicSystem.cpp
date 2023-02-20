#include "cPhysicSystem.h"
#include "math.h"

cPhysicSystem::cPhysicSystem()
{
}

cPhysicSystem::~cPhysicSystem()
{
}


void cPhysicSystem::createObject(cMeshObj* meshObj,cModelDrawInfo* DrawInfo)
{
    cObject* obj = new cObject();
    obj->pMeshObj = meshObj;
    //pDrawInfo = DrawInfo;
    cBoundingBox* pBBox = new cBoundingBox();
    obj->pBBox = pBBox;
    //plane->setMass(1.f);
    //plane->pBBox = &boundingBox;
    //plane->position = glm::vec3(0, 100,0);
    obj->objName = meshObj->instanceName;
    obj->position = meshObj->position;
    pBBox->halfExtentOffset = glm::vec3((DrawInfo->extentX / 2.f) * meshObj->scale.x, (DrawInfo->extentY / 2.f) * meshObj->scale.y, (DrawInfo->extentZ / 2.f) * meshObj->scale.z);
    //float x = pBBox->halfExtentOffset.x + (DrawInfo->minX * meshObj->scale.x);
    //float y = pBBox->halfExtentOffset.y + (DrawInfo->minY * meshObj->scale.y);
    //float z = pBBox->halfExtentOffset.z + (DrawInfo->minZ * meshObj->scale.z);
    pBBox->centerPointOffset = glm::vec3(pBBox->halfExtentOffset.x + (DrawInfo->minX * meshObj->scale.x), pBBox->halfExtentOffset.y + (DrawInfo->minY * meshObj->scale.y), pBBox->halfExtentOffset.z + (DrawInfo->minZ * meshObj->scale.z));
    pBBox->maxPointOffset = glm::vec3(DrawInfo->maxX * meshObj->scale.x, DrawInfo->maxY * meshObj->scale.y, DrawInfo->maxZ * meshObj->scale.z);
    pBBox->minPointOffset = glm::vec3(DrawInfo->minX * meshObj->scale.x, DrawInfo->minY * meshObj->scale.y, DrawInfo->minZ * meshObj->scale.z);

    if (meshObj->meshName == "enemy")
    {
        Sphere* enemy = new Sphere(Point(0), DrawInfo->extentY/2);
        obj->pShape = enemy;
    }
    if (meshObj->meshName == "player")
    {
        float min[3] = { DrawInfo->minX,DrawInfo->minY,DrawInfo->minZ };
        float max[3] = { DrawInfo->maxX,DrawInfo->maxY,DrawInfo->maxZ };
        AABB* player = new AABB( min,max );

        obj->pShape = player;
    }
    if (meshObj->meshName == "obstacle")
    {
        float min[3] = { pBBox->minPointOffset.x + meshObj->position.x,pBBox->minPointOffset.y + meshObj->position.y,pBBox->minPointOffset.z + meshObj->position.z };
        float max[3] = { pBBox->maxPointOffset.x + meshObj->position.x,pBBox->maxPointOffset.y + meshObj->position.y,pBBox->maxPointOffset.z + meshObj->position.z };
        AABB* obstacle = new AABB(min, max);

        obj->pShape = obstacle;
    }

    mapOBJ.emplace(obj->pMeshObj->instanceName, obj);

}

//void cPhysicSystem::createObject(cMeshObj* meshObj, cModelDrawInfo* DrawInfo, cObject::AI_type AI_type)
//{
//    cObject* obj = new cObject();
//    obj->pMeshObj = meshObj;
//    cBoundingBox* pBBox = new cBoundingBox();
//    obj->pBBox = pBBox;
//
//    obj->objName = meshObj->instanceName;
//    obj->position = meshObj->position;
//    pBBox->halfExtentOffset = glm::vec3((DrawInfo->extentX / 2.f) * meshObj->scale.x, (DrawInfo->extentY / 2.f) * meshObj->scale.y, (DrawInfo->extentZ / 2.f) * meshObj->scale.z);
//
//    pBBox->centerPointOffset = glm::vec3(pBBox->halfExtentOffset.x + (DrawInfo->minX * meshObj->scale.x), pBBox->halfExtentOffset.y + (DrawInfo->minY * meshObj->scale.y), pBBox->halfExtentOffset.z + (DrawInfo->minZ * meshObj->scale.z));
//    pBBox->maxPointOffset = glm::vec3(DrawInfo->maxX * meshObj->scale.x, DrawInfo->maxY * meshObj->scale.y, DrawInfo->maxZ * meshObj->scale.z);
//    pBBox->minPointOffset = glm::vec3(DrawInfo->minX * meshObj->scale.x, DrawInfo->minY * meshObj->scale.y, DrawInfo->minZ * meshObj->scale.z);
//
//    if (meshObj->meshName == "enemy")
//    {
//        Sphere* enemy = new Sphere(Point(0), DrawInfo->extentY / 2);
//        obj->pShape = enemy;
//        obj->pSteering = new cSteering(&obj->position, &obj->direction,&obj->velocity);
//        obj->AI_Type = AI_type;
//    }
//    if (meshObj->meshName == "player")
//    {
//        float min[3] = { DrawInfo->minX,DrawInfo->minY,DrawInfo->minZ };
//        float max[3] = { DrawInfo->maxX,DrawInfo->maxY,DrawInfo->maxZ };
//        AABB* player = new AABB(min, max);
//
//        obj->pShape = player;
//    }
//    if (meshObj->meshName == "obstacle")
//    {
//        float min[3] = { pBBox->minPointOffset.x + meshObj->position.x,pBBox->minPointOffset.y + meshObj->position.y,pBBox->minPointOffset.z + meshObj->position.z };
//        float max[3] = { pBBox->maxPointOffset.x + meshObj->position.x,pBBox->maxPointOffset.y + meshObj->position.y,pBBox->maxPointOffset.z + meshObj->position.z };
//        AABB* obstacle = new AABB(min, max);
//
//        obj->pShape = obstacle;
//    }
//
//    mapOBJ.emplace(obj->pMeshObj->instanceName, obj);
//
//}

void cPhysicSystem::updateSystem(float dt)
{
    objPosUpdate();
   
}

bool cPhysicSystem::collisionCheck(cObject* pObjA, cObject* pObjB)
{
    bool isCollision = false;

    //Sphere* pEnemy = dynamic_cast<Sphere*>(pObjB->pShape);
    //
    //Vector3 posB = pEnemy->Center + Vector3(pObjB->position.x, pObjB->position.y, pObjB->position.z);
    //Vector3 posA = Vector3(pObjA->position.x, pObjA->position.y, pObjA->position.z);
    //isCollision = TestSphereSphere(posB, pEnemy->Radius, posA,1); //hack assume player is sphere

    return isCollision;
}

bool cPhysicSystem::gameUpdate()
{

    for (std::map<std::string, cObject*>::iterator obj_it = mapOBJ.begin(); obj_it != mapOBJ.end(); obj_it++)
    {
        if (obj_it->second->pMeshObj->meshName == "enemy")
        {
            if (!obj_it->second->pMeshObj->isVisible)
            {
                //respawn
                obj_it->second->position = glm::vec3(0.f,1.f,0.f);
                obj_it->second->pMeshObj->isVisible = true;
            }
            if (obj_it->second->isHover)
            {
                obj_it->second->position = glm::vec3(-10000.f, -100000.f, -10000.f);
                obj_it->second->pMeshObj->isVisible = false;
                obj_it->second->isHover = false;
            }
            int dirX = (rand() % 100) - 50;
            int dirZ = (rand() % 100) - 50;
            obj_it->second->direction = glm::normalize(obj_it->second->direction + glm::vec3(dirX, 0, dirZ));
        }
    }

    return true;
}

bool cPhysicSystem::objPosUpdate()
{
    float step = 0.1f;
    std::map<std::string, cObject*>::iterator playerObj = mapOBJ.find("Player");

    for (std::map<std::string, cObject*>::iterator obj_it = mapOBJ.begin(); obj_it != mapOBJ.end(); obj_it++)
    {
        if (obj_it->second->pMeshObj->meshName == "enemy")
        {
            obj_it->second->AI_update(playerObj->second);
            obj_it->second->position += obj_it->second->direction * step;
            obj_it->second->yRotation = asin(obj_it->second->direction.x);
            obj_it->second->update();

        }
        if (obj_it->second->pMeshObj->meshName == "bullet")
        {
            obj_it->second->position += obj_it->second->direction * 10.f;
            obj_it->second->update();
        }
    }

    return true;
}










