#ifndef GEOMETRY_BOUNDINGBOX_HPP
#define GEOMETRY_BOUNDINGBOX_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class BoundingBox
{
public:
	BoundingBox (const glm::dvec3& min, const glm::dvec3& max);

	glm::dvec3 min;
	glm::dvec3 max;
};

}

#endif
