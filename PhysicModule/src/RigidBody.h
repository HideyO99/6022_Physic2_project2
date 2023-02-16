#pragma once
#include <iRigidBody.h>
#include <iRigidBodyDesc.h>


class RigidBody : public iRigidBody
{
public:
	RigidBody(const iRigidBodyDesc& desc, iShape* shape);
	~RigidBody();

	static RigidBody* cast(iCollision* body);
	iShape* getShape();
	
	bool isStatic();
	virtual Vec3 getPosition() override;
	virtual void setPosition(const Vec3& pos) override;

	virtual Quat getRotation() override;
	virtual void setRotation(const Quat& rotate) override;

	virtual void addForce(const Vec3& force) override;
	virtual void addForceAP(const Vec3& force, const Vec3& at) override;

	virtual void addTorque(const Vec3& torque) override;

	void setGravity(const Vec3& gravity);
	void updateAcc();

	void killForce();
	void addDamping(float dt);

	void setRenderPosition(Vec3* pos);
	void update(float step);

private:
	float m_mass;
	float m_inverseMass;
	float m_friction;
	float m_restitution;
	float m_linearDamping;
	float m_angularDamping;

	bool m_bStatic;

	Vec3 m_position;
	Vec3 m_prevPosition;
	Vec3 m_velocity;
	Vec3 m_acceleration;

	Vec3 m_force;
	Vec3 m_torque;
	Vec3 m_gravity;

	Quat m_rotation;
	Vec3 m_angularVelocity;
	Vec3 m_angularAcceleration;

	iShape* m_shape;

	Vec3* m_renderPosition;

	RigidBody(const RigidBody&) {}
	RigidBody& operator=(const RigidBody&)
	{
		return *this;
	}
};

