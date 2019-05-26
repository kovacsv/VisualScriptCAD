#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "TestUtils.hpp"
#include "RayTracing.hpp"

using namespace Geometry;

namespace RayTracingTest
{

TEST (RayTriangleIntersectionTest)
{
	glm::dvec3 v1 (0.0, 0.0, 0.0);
	glm::dvec3 v2 (1.0, 0.0, 0.0);
	glm::dvec3 v3 (1.0, 1.0, 0.0);

	auto HasIntersection = [&] (const glm::dvec3& from, const glm::dvec3& to)
	{
		Ray ray (from, to - from);
		RayIntersection intersection = GetRayTriangleIntersection (ray, v1, v2, v3);
		return intersection.found;
	};

	{
		Ray ray (glm::dvec3 (0.2, 0.2, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
		RayIntersection intersection = GetRayTriangleIntersection (ray, v1, v2, v3);
		ASSERT (!intersection.found);
	}

	{
		Ray ray (glm::dvec3 (0.2, 0.2, 1.0), glm::dvec3 (0.0, 0.0, -1.0));
		RayIntersection intersection = GetRayTriangleIntersection (ray, v1, v2, v3);
		ASSERT (intersection.found);
		ASSERT (IsEqualVec (intersection.position, glm::dvec3 (0.2, 0.2, 0.0)));
		ASSERT (IsEqual (intersection.distance, 1.0));
	}

	{
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0, 0, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (1, 0, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (1, 1, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0, 0, -1)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0.2, 0.2, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0.5, 0.5, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0.6, 0.4, 0)) == true);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0.0, 1.0, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0.6, 0.7, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (-1, 0, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (0, 0, 2)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, 1), glm::dvec3 (1, 1, 1)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (0, 0, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (1, 0, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (1, 1, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (0, 0, -1)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (0.2, 0.2, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (0.5, 0.5, 0)) == false);
		ASSERT (HasIntersection (glm::dvec3 (0, 0, -1), glm::dvec3 (0.6, 0.4, 0)) == false);
	}
}

}
