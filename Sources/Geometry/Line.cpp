#include "Line.hpp"

namespace Geometry
{

Line::Line (const glm::dvec3& origin, const glm::dvec3& direction) :
	origin (origin),
	direction (direction)
{

}

Line Line::FromTwoPoints (const glm::dvec3& beg, const glm::dvec3& end)
{
	return Line (beg, glm::normalize (end - beg));
}

}
