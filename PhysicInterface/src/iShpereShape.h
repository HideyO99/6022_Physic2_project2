#pragma once
#include "Shape.h"

class iShpereShape : public Shape
{
public:
	iShpereShape(float rad);
	virtual ~iShpereShape();

	float getRad() const;

	static iShpereShape* cast(Shape* shape);

protected:
	iShpereShape(shapeType type) : Shape(type) {}

private:
	float m_Rad;

	iShpereShape(const iShpereShape&) : Shape(shapeType::Sphere) {}

	iShpereShape& operator=(const iShpereShape&)
	{
		return *this;
	}
};