#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "PlaneUtils.hpp"

using namespace Geometry;

namespace PlaneUtilsTest
{
	
static bool IsEqualPlanes (const Plane& p1, const Plane& p2)
{
	return IsEqual (p1.a, p2.a) && IsEqual (p1.b, p2.b) && IsEqual (p1.c, p2.c) && IsEqual (p1.d, p2.d);
}

TEST (PlaneCreationTest)
{
	Plane p1 = Plane::FromThreePoints (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (4.0, 0.0, 0.0), glm::dvec3 (0.0, 3.0, 0.0));
	Plane p2 = Plane::FromThreePoints (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (4.0, 0.0, 1.0), glm::dvec3 (0.0, 3.0, 1.0));
	Plane p3 = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 12.0));
	Plane p4 = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 12.0));

	ASSERT (IsEqualPlanes (p1, Plane (0.0, 0.0, 1.0, 0.0)));
	ASSERT (IsEqualPlanes (p2, Plane (0.0, 0.0, 1.0, -1.0)));
	ASSERT (IsEqualPlanes (p3, Plane (0.0, 0.0, 1.0, 0.0)));
	ASSERT (IsEqualPlanes (p4, Plane (0.0, 0.0, 1.0, -1.0)));

	Plane p5 = Plane::FromThreePoints (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 3.0, 0.0), glm::dvec3 (4.0, 0.0, 0.0));
	Plane p6 = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, -12.0));

	ASSERT (IsEqualPlanes (p5, Plane (0.0, 0.0, -1.0, 0.0)));
	ASSERT (IsEqualPlanes (p6, Plane (0.0, 0.0, -1.0, 0.0)));
}

TEST (PointPlanePositionTest)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (0.0, 0.0, 0.0)) == PointPlanePosition::BackOfPlane);
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (10.0, 40.0, 0.0)) == PointPlanePosition::BackOfPlane);
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (0.0, 0.0, 2.0)) == PointPlanePosition::FrontOfPlane);
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (10.0, 50.0, 2.0)) == PointPlanePosition::FrontOfPlane);
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (0.0, 0.0, 1.0)) == PointPlanePosition::OnPlane);
	ASSERT (GetPointPlanePosition (plane, glm::dvec3 (10.0, 50.0, 1.0)) == PointPlanePosition::OnPlane);
}

TEST (LinePlaneIntersectionTest)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	LinePlaneIntersectionResult result1 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (1.0, 0.0, 0.0)));
	LinePlaneIntersectionResult result2 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0)));
	LinePlaneIntersectionResult result3 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, -1.0)));
	LinePlaneIntersectionResult result4 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 2.0), glm::dvec3 (0.0, 0.0, -1.0)));
	LinePlaneIntersectionResult result5 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 2.0), glm::dvec3 (0.0, 0.0, 1.0)));
	LinePlaneIntersectionResult result6 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, -10.0), glm::dvec3 (0.0, 0.0, 1.0)));
	LinePlaneIntersectionResult result7 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (0.0, 0.0, 10.0), glm::dvec3 (0.0, 0.0, -1.0)));
	LinePlaneIntersectionResult result8 = GetLinePlaneIntersection (plane, Line (glm::dvec3 (3.0, 4.0, 10.0), glm::dvec3 (0.0, 0.0, -1.0)));
	ASSERT (!result1.found);
	ASSERT (result2.found && IsEqual (result2.position.x, 0.0) && IsEqual (result2.position.y, 0.0) && IsEqual (result2.position.z, 1.0));
	ASSERT (result3.found && IsEqual (result3.position.x, 0.0) && IsEqual (result3.position.y, 0.0) && IsEqual (result3.position.z, 1.0));
	ASSERT (result4.found && IsEqual (result4.position.x, 0.0) && IsEqual (result4.position.y, 0.0) && IsEqual (result4.position.z, 1.0));
	ASSERT (result5.found && IsEqual (result5.position.x, 0.0) && IsEqual (result5.position.y, 0.0) && IsEqual (result5.position.z, 1.0));
	ASSERT (result6.found && IsEqual (result6.position.x, 0.0) && IsEqual (result6.position.y, 0.0) && IsEqual (result6.position.z, 1.0));
	ASSERT (result7.found && IsEqual (result7.position.x, 0.0) && IsEqual (result7.position.y, 0.0) && IsEqual (result7.position.z, 1.0));
	ASSERT (result8.found && IsEqual (result8.position.x, 3.0) && IsEqual (result8.position.y, 4.0) && IsEqual (result8.position.z, 1.0));
}

}
