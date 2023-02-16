#include "iBoxShape.h"

iBoxShape::iBoxShape(const Vec3& halfExtent)
    : Shape(shapeType::Box)
    , m_halfExtent(halfExtent)
{
}

iBoxShape::~iBoxShape()
{
}

const Vec3& iBoxShape::getHalfExtents() const
{
    return m_halfExtent;
}

iBoxShape* iBoxShape::cast(Shape* shape)
{
    return dynamic_cast<iBoxShape*>(shape);
}
