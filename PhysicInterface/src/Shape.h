#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

typedef glm::vec3 Vec3;
typedef glm::quat Quat;

enum class shapeType
{
	Box,
	Cylinder,
	Plane,
	Sphere,
};

class Shape
{
public:
	virtual ~Shape();
	shapeType getShapeType()
	{
		return m_shapeType;
	}

private:
	shapeType m_shapeType;
	Shape(const Shape&);
	Shape& operator=(const Shape&);

protected:
	Shape(shapeType type) 
	{
		m_shapeType = type;
	}
};
