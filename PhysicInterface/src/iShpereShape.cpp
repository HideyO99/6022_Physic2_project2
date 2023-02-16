#include "iShpereShape.h"

iShpereShape::iShpereShape(float rad)
	: Shape(shapeType::Sphere)
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

iShpereShape* iShpereShape::cast(Shape* shape)
{
	return dynamic_cast<iShpereShape*>(shape);
}
