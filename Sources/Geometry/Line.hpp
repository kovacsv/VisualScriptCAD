#ifndef GEOMETRY_LINE_HPP
#define GEOMETRY_LINE_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class Line
{
public:
	Line (const glm::dvec3& origin, const glm::dvec3& direction);

	static Line FromTwoPoints (const glm::dvec3& beg, const glm::dvec3& end);

	glm::dvec3 origin;
	glm::dvec3 direction;
};

}

#endif
