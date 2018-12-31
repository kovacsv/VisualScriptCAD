#include "Triangle.hpp"

namespace Geometry
{

Triangle::Triangle (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3) :
	vertices ({ v1, v2, v3 })
{
}

const glm::dvec3& Triangle::operator[] (size_t index) const
{
	return vertices[index];
}

}
