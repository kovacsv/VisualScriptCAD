#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "TestUtils.hpp"
#include "RayIntersection.hpp"
#include "RayTracing.hpp"

using namespace Geometry;
using namespace Modeler;

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

TEST (GetScreenRayTest)
{
	double screenWidth = 200.0;
	double screenHeight = 100.0;
	double fovY = 45.0;
	double fovX = glm::degrees (2.0 * atan (tan (glm::radians (fovY) * 0.5) * (screenWidth / screenHeight)));
	Camera camera (glm::dvec3 (1.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), fovY, 0.1, 10000.0);
	glm::ivec2 screenSize (screenWidth, screenHeight);
	
	{
		Ray ray = GetScreenRay (camera, screenSize, glm::dvec2 (100.0, 50.0));
		ASSERT (IsEqualVec (ray.GetOrigin (), camera.GetEye ()));
		ASSERT (IsEqualVec (ray.GetDirection (), glm::dvec3 (-1.0, 0.0, 0.0)));
	}

	{
		Ray ray = GetScreenRay (camera, screenSize, glm::dvec2 (100.0, 0.0));
		ASSERT (IsEqualVec (ray.GetOrigin (), camera.GetEye ()));
		glm::dvec3 expectedRay = glm::normalize (glm::rotate (glm::dvec3 (-1.0, 0.0, 0.0), glm::radians (fovY / 2.0), glm::dvec3 (0.0, 1.0, 0.0)));
		ASSERT (IsEqualVec (ray.GetDirection (), expectedRay));
	}

	{
		Ray ray = GetScreenRay (camera, screenSize, glm::dvec2 (100.0, 100.0));
		ASSERT (IsEqualVec (ray.GetOrigin (), camera.GetEye ()));
		glm::dvec3 expectedRay = glm::normalize (glm::rotate (glm::dvec3 (-1.0, 0.0, 0.0), -glm::radians (fovY / 2.0), glm::dvec3 (0.0, 1.0, 0.0)));
		ASSERT (IsEqualVec (ray.GetDirection (), expectedRay));
	}

	{
		Ray ray = GetScreenRay (camera, screenSize, glm::dvec2 (0.0, 50.0));
		ASSERT (IsEqualVec (ray.GetOrigin (), camera.GetEye ()));
		glm::dvec3 expectedRay = glm::normalize (glm::rotate (glm::dvec3 (-1.0, 0.0, 0.0), glm::radians (fovX / 2.0), glm::dvec3 (0.0, 0.0, 1.0)));
		ASSERT (IsEqualVec (ray.GetDirection (), expectedRay));
	}

	{
		Ray ray = GetScreenRay (camera, screenSize, glm::dvec2 (200.0, 50.0));
		ASSERT (IsEqualVec (ray.GetOrigin (), camera.GetEye ()));
		glm::dvec3 expectedRay = glm::normalize (glm::rotate (glm::dvec3 (-1.0, 0.0, 0.0), -glm::radians (fovX / 2.0), glm::dvec3 (0.0, 0.0, 1.0)));
		ASSERT (IsEqualVec (ray.GetDirection (), expectedRay));
	}
}

}
