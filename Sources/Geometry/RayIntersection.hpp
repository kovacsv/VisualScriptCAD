#ifndef GEOMETRY_RAYINTERSECTION_HPP
#define GEOMETRY_RAYINTERSECTION_HPP

#include "IncludeGLM.hpp"
#include "Ray.hpp"
#include "BoundingShapes.hpp"

namespace Geometry
{

class RayIntersection
{
public:
	RayIntersection ();
	RayIntersection (const glm::dvec3& position, double distance);

	glm::dvec3	position;
	double		distance;
};

class RayIntersectionResult
{
public:
	RayIntersectionResult ();
	RayIntersectionResult (const RayIntersection& intersection);

	bool				found;
	RayIntersection		intersection;
};

extern const RayIntersectionResult NoIntersection;

RayIntersectionResult	GetRayTriangleIntersection (const Ray& ray, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3);

}

#endif
