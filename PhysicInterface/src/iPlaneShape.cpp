#include "iPlaneShape.h"

iPlaneShape::iPlaneShape(const Vec3& norm, float dotProd)
	: Shape(shapeType::Plane)
	, m_Normal(norm)
	, m_DotProd(dotProd)
{
}

iPlaneShape::~iPlaneShape()
{
}

const Vec3& iPlaneShape::getNormal() const
{
	return m_Normal;
}

float iPlaneShape::getDotProd() const
{
	return m_DotProd;
}

iPlaneShape* iPlaneShape::cast(Shape* shape)
{
	return dynamic_cast<iPlaneShape*>(shape);
}
