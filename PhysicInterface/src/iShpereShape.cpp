#include "iShpereShape.h"

iShpereShape::iShpereShape(float rad)
	: iShape(shapeType::Sphere)
	, m_Rad(rad)
{

}

iShpereShape::~iShpereShape()
{
}

float iShpereShape::getRad() const
{
	return m_Rad;
}

iShpereShape* iShpereShape::cast(iShape* shape)
{
	return dynamic_cast<iShpereShape*>(shape);
}
