#include "Plane.hpp"
#include "Geometry.hpp"

namespace Geometry
{

Plane::Plane (double a, double b, double c, double d) :
	a (a),
	b (b),
	c (c),
	d (d)
{
}

Plane Plane::FromPointAndDirection (const glm::dvec3& point, const glm::dvec3& direction)
{
	glm::dvec3 normal = glm::normalize (direction);
	double a = normal.x;
	double b = normal.y;
	double c = normal.z;
	double d = -glm::dot (point, normal);
	return Plane (a, b, c, d);
}

Plane Plane::FromThreePoints (const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& p3)
{
	glm::dvec3 direction = glm::cross (p2 - p1, p3 - p1);
	return FromPointAndDirection (p1, direction);
}

}
