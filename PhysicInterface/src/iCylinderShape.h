#pragma once
#include "Shape.h"

class iCylinderShape : public Shape
{
public:
	iCylinderShape(const Vec3& halfExtent);
	virtual ~iCylinderShape();

	const Vec3& getHalfExtents() const;

	static iCylinderShape* cast(Shape* shape);

protected:
	iCylinderShape(shapeType type) : Shape(type) {}

private:
	Vec3 m_halfExtent;

	iCylinderShape(const iCylinderShape&) : Shape(shapeType::Cylinder) {}

	iCylinderShape& operator=(const iCylinderShape&)
	{
		return *this;
	}
};

