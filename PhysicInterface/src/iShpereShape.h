#pragma once
#include "iShape.h"

class iShpereShape : public iShape
{
public:
	iShpereShape(float rad);
	virtual ~iShpereShape();

	float getRad() const;

	static iShpereShape* cast(iShape* shape);

protected:
	iShpereShape(shapeType type) : iShape(type) {}

private:
	float m_Rad;

	iShpereShape(const iShpereShape&) : iShape(shapeType::Sphere) {}

	iShpereShape& operator=(const iShpereShape&)
	{
		return *this;
	}
};