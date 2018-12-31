#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "TriangleUtils.hpp"

using namespace Geometry;

namespace TriangleUtilsTest
{

static bool IsEqualVec (const glm::dvec3& a, const glm::dvec3& b)
{
	return IsEqual (a.x, b.x) && IsEqual (a.y, b.y) && IsEqual (a.z, b.z);
}

static bool IsEqualTriangle (const Triangle& a, const Triangle& b)
{
	return IsEqualVec (a[0], b[0]) && IsEqualVec (a[1], b[1]) && IsEqualVec (a[2], b[2]);
}

TEST (CutTriangleWithPlane_TriangleNotCrossingPlane)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	Triangle t1 (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (4.0, 0.0, 0.0), glm::dvec3 (0.0, 3.0, 0.0));
	Triangle t2 (glm::dvec3 (0.0, 0.0, 2.0), glm::dvec3 (4.0, 0.0, 2.0), glm::dvec3 (0.0, 3.0, 2.0));
	Triangle t3 (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (4.0, 0.0, 1.0), glm::dvec3 (0.0, 3.0, 1.0));
	TrianglePlaneCutResult result1 = CutTriangleWithPlane (plane, t1);
	TrianglePlaneCutResult result2 = CutTriangleWithPlane (plane, t2);
	TrianglePlaneCutResult result3 = CutTriangleWithPlane (plane, t3);
	ASSERT (result1.frontTriangles.size () == 0 && result1.backTriangles.size () == 1 && result1.planeTriangles.size () == 0);
	ASSERT (result2.frontTriangles.size () == 1 && result2.backTriangles.size () == 0 && result2.planeTriangles.size () == 0);
	ASSERT (result3.frontTriangles.size () == 0 && result3.backTriangles.size () == 0 && result3.planeTriangles.size () == 1);
}

TEST (CutTriangleWithPlane_TriangleOnPlaneWithOneVertex)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	Triangle t1 (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (4.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0));
	Triangle t2 (glm::dvec3 (0.0, 0.0, 2.0), glm::dvec3 (4.0, 0.0, 2.0), glm::dvec3 (0.0, 0.0, 1.0));
	TrianglePlaneCutResult result1 = CutTriangleWithPlane (plane, t1);
	TrianglePlaneCutResult result2 = CutTriangleWithPlane (plane, t2);
	ASSERT (result1.frontTriangles.size () == 0 && result1.backTriangles.size () == 1 && result1.planeTriangles.size () == 0);
	ASSERT (result2.frontTriangles.size () == 1 && result2.backTriangles.size () == 0 && result2.planeTriangles.size () == 0);
}

TEST (CutTriangleWithPlane_TriangleOnPlaneWithTwoVertices)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	Triangle t1 (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (4.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	Triangle t2 (glm::dvec3 (0.0, 0.0, 2.0), glm::dvec3 (4.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 1.0));
	TrianglePlaneCutResult result1 = CutTriangleWithPlane (plane, t1);
	TrianglePlaneCutResult result2 = CutTriangleWithPlane (plane, t2);
	ASSERT (result1.frontTriangles.size () == 0 && result1.backTriangles.size () == 1 && result1.planeTriangles.size () == 0);
	ASSERT (result2.frontTriangles.size () == 1 && result2.backTriangles.size () == 0 && result2.planeTriangles.size () == 0);
}

TEST (CutTriangleWithPlane_OnVertexCut)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 1.0, 0.0));
	Triangle triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0));
	TrianglePlaneCutResult result = CutTriangleWithPlane (plane, triangle);
	ASSERT (result.frontTriangles.size () == 1 && result.backTriangles.size () == 1);
	ASSERT (IsEqualTriangle (result.backTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 0.0, 0.0))));
	ASSERT (IsEqualTriangle (result.frontTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, 0.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0))));
}

TEST (CutTriangleWithPlane_OnVertexCut2)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, -1.0, 0.0));
	Triangle triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0));
	TrianglePlaneCutResult result = CutTriangleWithPlane (plane, triangle);
	ASSERT (result.frontTriangles.size () == 1 && result.backTriangles.size () == 1);
	ASSERT (IsEqualTriangle (result.frontTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 0.0, 0.0))));
	ASSERT (IsEqualTriangle (result.backTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, 0.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0))));
}

TEST (CutTriangleWithPlane_GeneralCut)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (-1.0, 0.0, 0.0));
	Triangle triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0));
	TrianglePlaneCutResult result = CutTriangleWithPlane (plane, triangle);
	ASSERT (result.frontTriangles.size () == 1 && result.backTriangles.size () == 2);
	ASSERT (IsEqualTriangle (result.frontTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (0.0, -0.5, 0.0), glm::dvec3 (0.0, 0.5, 0.0))));
	ASSERT (IsEqualTriangle (result.backTriangles[0], Triangle (glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (0.0, 0.5, 0.0), glm::dvec3 (0.0, -0.5, 0.0))));
	ASSERT (IsEqualTriangle (result.backTriangles[1], Triangle (glm::dvec3 (1.0, 1.0, 0.0), glm::dvec3 (0.0, 0.5, 0.0), glm::dvec3 (1.0, -1.0, 0.0))));
}

TEST (CutTriangleWithPlane_GeneralCut2)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (-1.0, 0.0, 0.0));
	Triangle triangle (glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (1.0, 1.0, 0.0), glm::dvec3 (-1.0, 0.0, 0.0));
	TrianglePlaneCutResult result = CutTriangleWithPlane (plane, triangle);
	ASSERT (result.frontTriangles.size () == 1 && result.backTriangles.size () == 2);
	ASSERT (IsEqualTriangle (result.frontTriangles[0], Triangle (glm::dvec3 (-1.0, 0.0, 0.0), glm::dvec3 (0.0, -0.5, 0.0), glm::dvec3 (0.0, 0.5, 0.0))));
	ASSERT (IsEqualTriangle (result.backTriangles[0], Triangle (glm::dvec3 (1.0, -1.0, 0.0), glm::dvec3 (0.0, 0.5, 0.0), glm::dvec3 (0.0, -0.5, 0.0))));
	ASSERT (IsEqualTriangle (result.backTriangles[1], Triangle (glm::dvec3 (1.0, 1.0, 0.0), glm::dvec3 (0.0, 0.5, 0.0), glm::dvec3 (1.0, -1.0, 0.0))));
}

TEST (CutTriangleWithPlane_GeneralCut3)
{
	Plane plane = Plane::FromPointAndDirection (glm::dvec3 (0.5, -0.5, 0.5), glm::dvec3 (1.0, 0.0, 0.0));
	Triangle triangle (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (2.0, 0.0, 0.0), glm::dvec3 (2.0, 1.0, 0.0));
	TrianglePlaneCutResult result = CutTriangleWithPlane (plane, triangle);
	ASSERT (result.frontTriangles.size () == 2 && result.backTriangles.size () == 1);
}


TEST (BarycentricInterpolationTest)
{
	glm::dvec3 v1 (0.0, 0.0, 0.0);
	glm::dvec3 v2 (1.0, 0.0, 0.0);
	glm::dvec3 v3 (0.0, 1.0, 0.0);

	glm::dvec3 val1 (1.0, 2.0, 3.0);
	glm::dvec3 val2 (4.0, 5.0, 6.0);
	glm::dvec3 val3 (7.0, 8.0, 9.0);

	ASSERT (IsEqualVec (BarycentricInterpolation (v1, v2, v3, val1, val2, val3, v1), val1));
	ASSERT (IsEqualVec (BarycentricInterpolation (v1, v2, v3, val1, val2, val3, v2), val2));
	ASSERT (IsEqualVec (BarycentricInterpolation (v1, v2, v3, val1, val2, val3, v3), val3));
}

}
