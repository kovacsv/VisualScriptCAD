#ifndef GEOMETRY_TRIANGLEUTILS_HPP
#define GEOMETRY_TRIANGLEUTILS_HPP

#include "Plane.hpp"
#include "Triangle.hpp"

#include <vector>

namespace Geometry
{

class TrianglePlaneCutResult
{
public:
	TrianglePlaneCutResult ();

	std::vector<Triangle> frontTriangles;
	std::vector<Triangle> backTriangles;
	std::vector<Triangle> planeTriangles;
};

glm::dvec3						CalculateTriangleNormal (const Triangle& triangle);
glm::dvec3						CalculateTriangleNormal (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3);
Plane							GetTrianglePlane (const Triangle& triangle);
TrianglePlaneCutResult			CutTriangleWithPlane (const Plane& plane, const Triangle& triangle);

double							CalculateTriangleArea (double a, double b, double c);
glm::dvec3						BarycentricInterpolation (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3, const glm::dvec3& val1, const glm::dvec3& val2, const glm::dvec3& val3, const glm::dvec3& position);

}

#endif
