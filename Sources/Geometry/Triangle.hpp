#ifndef GEOMETRY_TRIANGLE_HPP
#define GEOMETRY_TRIANGLE_HPP

#include "IncludeGLM.hpp"

#include <array>

namespace Geometry
{

class Triangle
{
public:
	Triangle (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3);

	const glm::dvec3& operator[] (size_t index) const;

	std::array<glm::dvec3, 3> vertices;
};

}

#endif
