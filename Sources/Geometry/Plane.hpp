#ifndef GEOMETRY_PLANE_HPP
#define GEOMETRY_PLANE_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class Plane
{
public:
	Plane (double a, double b, double c, double d);

	static Plane FromPointAndDirection (const glm::dvec3& point, const glm::dvec3& direction);
	static Plane FromThreePoints (const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& p3);

	double a;
	double b;
	double c;
	double d;
};

}

#endif
