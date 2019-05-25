#include "Ray.hpp"

namespace Geometry
{

Ray::Ray (const glm::dvec3& origin, const glm::dvec3& direction) :
	origin (origin),
	direction (direction)
{
}

Ray::~Ray ()
{
}

const glm::dvec3& Ray::GetOrigin () const
{
	return origin;
}

const glm::dvec3& Ray::GetDirection () const
{
	return direction;
}

}
