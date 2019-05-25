#ifndef GEOMETRY_RAY_HPP
#define GEOMETRY_RAY_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class Ray
{
public:
	Ray (const glm::dvec3& origin, const glm::dvec3& direction);
	~Ray ();

	const glm::dvec3&	GetOrigin () const;
	const glm::dvec3&	GetDirection () const;

private:
	glm::dvec3	origin;
	glm::dvec3	direction;
};

}

#endif
