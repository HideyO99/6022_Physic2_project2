#pragma once
#include "Shape.h"


class iBoxShape : public Shape
{
public:
	iBoxShape(const Vec3& halfExtent  );
	virtual ~iBoxShape();

	const Vec3& getHalfExtents() const;

	static iBoxShape* cast(Shape* shape);

protected:
	iBoxShape(shapeType type) : Shape(type) {}

private:
	Vec3 m_halfExtent;

	iBoxShape(const iBoxShape&) : Shape(shapeType::Box) {}
	
	iBoxShape& operator=(const iBoxShape&)
	{
		return *this;
	}
};

