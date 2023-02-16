#pragma once
#include "Shape.h"

class iPlaneShape : public Shape
{
public:
	iPlaneShape(const Vec3& norm, float dotProd);//todo
	virtual ~iPlaneShape();

	const Vec3& getNormal() const;
	float getDotProd() const;

	static iPlaneShape* cast(Shape* shape);

protected:
	iPlaneShape(shapeType type) : Shape(type) {}

private:
	Vec3 m_Normal;
	float m_DotProd;

	iPlaneShape(const iPlaneShape&) : Shape(shapeType::Plane) {}

	iPlaneShape& operator=(const iPlaneShape&)
	{
		return *this;
	}
};