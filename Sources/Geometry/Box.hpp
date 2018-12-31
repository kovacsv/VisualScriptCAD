#ifndef GEOMETRY_BOX_HPP
#define GEOMETRY_BOX_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class Box
{
public:
	Box (const glm::dvec3& min, const glm::dvec3& max);

	glm::dvec3 min;
	glm::dvec3 max;
};

}

#endif
