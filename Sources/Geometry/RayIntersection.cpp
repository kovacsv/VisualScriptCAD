#include "RayIntersection.hpp"
#include "Geometry.hpp"

#include <vector>
#include <array>

namespace Geometry
{

RayIntersection::RayIntersection () :
	position (glm::dvec3 (0.0)),
	distance (0.0)
{
}

RayIntersection::RayIntersection (const glm::dvec3& position, double distance) :
	position (position),
	distance (distance)
{
}

RayIntersectionResult::RayIntersectionResult () :
	found (false),
	intersection ()
{
}

RayIntersectionResult::RayIntersectionResult (const RayIntersection& intersection) :
	found (true),
	intersection (intersection)
{
}

const RayIntersectionResult NoIntersection;

RayIntersectionResult GetRayTriangleIntersection (const Ray& ray, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3)
{
	const glm::dvec3& rayOrigin = ray.GetOrigin ();
	const glm::dvec3& rayDirection = glm::normalize (ray.GetDirection ());

	glm::dvec3 edgeDir1 = v2 - v1;
	glm::dvec3 edgeDir2 = v3 - v1;

	glm::dvec3 pVector = glm::cross (rayDirection, edgeDir2);
	double determinant = glm::dot (edgeDir1, pVector);
	if (!IsPositive (determinant)) {
		return NoIntersection;
	}

	double invDeterminant = 1.0 / determinant;
	glm::dvec3 tVector = rayOrigin - v1;
	double u = glm::dot (tVector, pVector) * invDeterminant;
	if (IsLower (u, 0.0) || IsGreater (u, 1.0)) {
		return NoIntersection;
	}

	glm::dvec3 qVector = glm::cross (tVector, edgeDir1);
	double v = glm::dot (rayDirection, qVector) * invDeterminant;
	if (IsLower (v, 0.0) || IsGreater (u + v, 1.0)) {
		return NoIntersection;
	}

	double distance = glm::dot (edgeDir2, qVector) * invDeterminant;
	if (!IsPositive (distance)) {
		return NoIntersection;
	}

	glm::dvec3 directionVector = rayDirection * distance;
	return RayIntersectionResult (RayIntersection (rayOrigin + directionVector, distance));
}

RayIntersectionResult GetRayBoundingBoxIntersection (const Geometry::Ray& ray, const Geometry::BoundingBox& boundingBox)
{
	// from Graphic Gems I.

	enum class Quadrant
	{
		Left,
		Right,
		Middle
	};

	glm::dvec3 minB = boundingBox.GetMin ();
	glm::dvec3 maxB = boundingBox.GetMax ();
	glm::dvec3 origin = ray.GetOrigin ();
	glm::dvec3 dir = glm::normalize (ray.GetDirection ());

	std::array<Quadrant, 3> quadrant = { Quadrant::Left, Quadrant::Left, Quadrant::Left };
	std::array<double, 3> candidatePlane = { 0.0, 0.0, 0.0 };

	bool inside = true;
	for (int i = 0; i < 3; i++) {
		if (origin[i] < minB[i]) {
			quadrant[i] = Quadrant::Left;
			candidatePlane[i] = minB[i];
			inside = false;
		} else if (origin[i] > maxB[i]) {
			quadrant[i] = Quadrant::Right;
			candidatePlane[i] = maxB[i];
			inside = false;
		} else {
			quadrant[i] = Quadrant::Middle;
		}
	}

	if (inside) {
		RayIntersection intersection (origin, 0.0);
		return RayIntersectionResult (intersection);
	}

	std::array<double, 3> maxT = { 0.0, 0.0, 0.0 };
	for (int i = 0; i < 3; i++) {
		if (quadrant[i] != Quadrant::Middle && !Geometry::IsZero (dir[i])) {
			maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
		} else {
			maxT[i] = -1.0;
		}
	}

	int whichPlane = 0;
	for (int i = 1; i < 3; i++) {
		if (maxT[i] > maxT[whichPlane]) {
			whichPlane = i;
		}
	}

	if (Geometry::IsNegative (maxT[whichPlane])) {
		return NoIntersection;
	}

	glm::dvec3 coord (0.0);
	for (int i = 1; i < 3; i++) {
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] * dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i]) {
				return NoIntersection;
			}
		} else {
			coord[i] = candidatePlane[i];
		}
	}

	RayIntersection intersection (coord, glm::distance (origin, coord));
	return RayIntersectionResult (intersection);
}

bool HasRayTriangleIntersection (const Ray& ray, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3)
{
	RayIntersectionResult result = GetRayTriangleIntersection (ray, v1, v2, v3);
	return result.found;
}

bool HasRayBoundingBoxIntersection (const Geometry::Ray& ray, const Geometry::BoundingBox& boundingBox)
{
	RayIntersectionResult result = GetRayBoundingBoxIntersection (ray, boundingBox);
	return result.found;
}

}
