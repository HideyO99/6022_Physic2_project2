#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
}

CollisionHandler::~CollisionHandler()
{
}

glm::vec3 CollisionHandler::ClosestPointPlain(const glm::vec3& point, const glm::vec3& planNormal, float dotProd)
{
	float cal = glm::dot(planNormal, point) - dotProd;
	glm::vec3 result = point - cal * planNormal;
	return result;
}

bool CollisionHandler::SphereVsSphere(float dt, RigidBody* body1, iSphereShape* sphere1, RigidBody* body2, iSphereShape* sphere2)
{
	if (body1->isStatic() && body2->isStatic())
	{
		return false;
	}

	Vec3 v0 = body1->m_position - body1->m_prevPosition;
	Vec3 v1 = body2->m_position - body2->m_prevPosition;
	float t = 0.f;

	float sphere1_rad = sphere1->getRad();
	float sphere2_rad = sphere2->getRad();
	bool bResult = testMovingSphereVsSphere(body1->m_prevPosition, sphere1_rad, body2->m_prevPosition, sphere2_rad, v0, v1, t);

	if (!bResult)
	{
		//no collision now
		return false;
	}

	if (t > dt)
	{
		//no collision in this step of time
		return false;
	}

	//todo

	//momentum



}

bool CollisionHandler::testMovingSphereVsSphere(const Vec3& s0_center, const float s0_rad, const Vec3& s1_center, const float s1_rad, const Vec3& v0, const Vec3& v1, float& t)
{
	Vec3 s = s1_center - s0_center;
	Vec3 v = v1 - v0;
	float r = s1_rad + s0_rad;
	float c = glm::dot(s, s) - r * r;

	if (c < 0.f)
	{
		t = 0.f;
		return true;
	}

	float a = glm::dot(v, v);
	if (a < std::numeric_limits<float>::epsilon())
	{
		return false;
	}

	float b = glm::dot(v, s);
	if (b >= 0.f)
	{
		return false;
	}

	float d = (b * b) - (a * c);
	if (d < 0.f)
	{
		return false;
	}

	t = (-b - sqrt(d)) / a;

	return true;
}

bool CollisionHandler::testMovingSphereVsPlane(const Vec3& prevPos, const Vec3& currentPos, float rad, const Vec3& normal, float dotprod)
{
	float distanceA = glm::dot(prevPos, normal) - dotprod;
	float distanceB = glm::dot(currentPos, normal) - dotprod;

	if (distanceA * distanceB < 0.f)
	{
		return true;
	}

	if ((abs(distanceA) < rad) || (abs(distanceB) <= rad))
	{
		return true;
	}

	return false;
}
