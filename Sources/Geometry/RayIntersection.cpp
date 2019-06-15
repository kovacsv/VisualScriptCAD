#include "RayIntersection.hpp"
#include "Geometry.hpp"

#include <vector>

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

bool HasRayBoundingBoxIntersection (const Ray& ray, const BoundingBox& boundingBox)
{
	enum class Quadrant
	{
		Left,
		Right,
		Middle
	};

	glm::dvec3 rayOrigin = ray.GetOrigin ();
	glm::dvec3 rayDirection = glm::normalize (ray.GetDirection ());

	const glm::dvec3& min = boundingBox.GetMin ();
	const glm::dvec3& max = boundingBox.GetMax ();

	std::vector<Quadrant> quadrant = {Quadrant::Left, Quadrant::Left, Quadrant::Left};
	glm::dvec3 candidatePlane (0.0);

	bool originInBox = true;
	for (glm::dvec3::length_type i = 0; i < 3; i++) {
		if (IsLower (rayOrigin[i], min[i])) {
			quadrant[i] = Quadrant::Left;
			candidatePlane[i] = min[i];
			originInBox = false;
		} else if (IsGreater (rayOrigin[i], max[i])) {
			quadrant[i] = Quadrant::Right;
			candidatePlane[i] = max[i];
			originInBox = false;
		} else {
			quadrant[i] = Quadrant::Middle;
		}
	}

	if (originInBox) {
		return true;
	}

	glm::dvec3::length_type whichPlane = 0;
	double maxT = -INF;
	for (glm::dvec3::length_type i = 0; i < 3; i++) {
		double t = -1.0;
		if (quadrant[i] != Quadrant::Middle || !IsZero (rayDirection[i])) {
			t = (candidatePlane[i] - rayOrigin[i]) / rayDirection[i];
		}
		if (IsGreater (t, maxT)) {
			whichPlane = i;
			maxT = t;
		}
	}

	if (IsNegative (maxT)) {
		return false;
	}

	for (glm::dvec3::length_type i = 0; i < 3; i++) {
		if (whichPlane != i) {
			double t = rayOrigin[i] + maxT * rayDirection[i];
			if (IsLower (t, min[i]) || IsGreater (t, max[i])) {
				return false;
			}
		}
	}

	return true;
}

}
