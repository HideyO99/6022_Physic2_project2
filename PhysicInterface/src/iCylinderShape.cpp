#include "iCylinderShape.h"

iCylinderShape::iCylinderShape(const Vec3& halfExtent)
    : Shape(shapeType::Cylinder)
    , m_halfExtent(halfExtent)
{
}

iCylinderShape::~iCylinderShape()
{
}

const Vec3& iCylinderShape::getHalfExtents() const
{
    return m_halfExtent;
}

iCylinderShape* iCylinderShape::cast(Shape* shape)
{
	return dynamic_cast<iCylinderShape*>(shape);
}
