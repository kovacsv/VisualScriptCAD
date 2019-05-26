#include "TriangleUtils.hpp"
#include "PlaneUtils.hpp"
#include "Line.hpp"
#include "Geometry.hpp"

#include <array>

namespace Geometry
{

TrianglePlaneCutResult::TrianglePlaneCutResult ()
{
}

glm::dvec3 CalculateTriangleNormal (const Triangle& triangle)
{
	return glm::triangleNormal (triangle[0], triangle[1], triangle[2]);
}

glm::dvec3 CalculateTriangleNormal (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3)
{
	return glm::triangleNormal (v1, v2, v3);
}

Plane GetTrianglePlane (const Triangle& triangle)
{
	return Plane::FromThreePoints (triangle[0], triangle[1], triangle[2]);
}

TrianglePlaneCutResult CutTriangleWithPlane (const Plane& plane, const Triangle& triangle)
{
	TrianglePlaneCutResult result;

	std::array<PointPlanePosition, 3> vertexPos = {
		GetPointPlanePosition (plane, triangle[0]),
		GetPointPlanePosition (plane, triangle[1]),
		GetPointPlanePosition (plane, triangle[2])
	};

	bool hasFront = false;
	bool hasBack = false;
	bool hasOn = false;
	for (size_t i = 0; i < 3; i++) {
		PointPlanePosition pos = vertexPos[i];
		if (pos == PointPlanePosition::FrontOfPlane) {
			hasFront = true;
		} else if (pos == PointPlanePosition::BackOfPlane) {
			hasBack = true;
		} else if (pos == PointPlanePosition::OnPlane) {
			hasOn = true;
		}
	}

	if (hasFront && !hasBack) {
		result.frontTriangles.push_back (triangle);
		return result;
	} else if (!hasFront && hasBack) {
		result.backTriangles.push_back (triangle);
		return result;
	} else if (hasOn && !hasFront && !hasBack) {
		result.planeTriangles.push_back (triangle);
		return result;
	}

	if (hasOn) {
		size_t cutVertex = (size_t) -1;
		for (size_t i = 0; i < 3; i++) {
			if (vertexPos[i] == PointPlanePosition::OnPlane) {
				cutVertex = i;
				break;
			}
		}
		size_t prev = cutVertex > 0 ? cutVertex - 1 : 2;
		size_t next = cutVertex < 2 ? cutVertex + 1 : 0;
		LinePlaneIntersectionResult intersection = GetLinePlaneIntersection (plane, Line::FromTwoPoints (triangle[prev], triangle[next]));
		if (vertexPos[next] == PointPlanePosition::FrontOfPlane) {
			result.frontTriangles.push_back (Triangle (triangle[cutVertex], triangle[next], intersection.position));
			result.backTriangles.push_back (Triangle (triangle[cutVertex], intersection.position, triangle[prev]));
		} else if (vertexPos[next] == PointPlanePosition::BackOfPlane) {
			result.backTriangles.push_back (Triangle (triangle[cutVertex], triangle[next], intersection.position));
			result.frontTriangles.push_back (Triangle (triangle[cutVertex], intersection.position, triangle[prev]));
		} else {
			throw std::logic_error ("failed to cut triangle");
		}
	} else {
		size_t cutVertex = (size_t) -1;
		for (size_t i = 0; i < 3; i++) {
			size_t curr = i;
			size_t prev = i > 0 ? i - 1 : 2;
			size_t next = i < 2 ? i + 1 : 0;
			if (vertexPos[curr] != vertexPos[prev] && vertexPos[curr] != vertexPos[next]) {
				cutVertex = curr;
				break;
			}
		}
		size_t prev = cutVertex > 0 ? cutVertex - 1 : 2;
		size_t next = cutVertex < 2 ? cutVertex + 1 : 0;
		LinePlaneIntersectionResult prevIntersection = GetLinePlaneIntersection (plane, Line::FromTwoPoints (triangle[cutVertex], triangle[prev]));
		LinePlaneIntersectionResult nextIntersection = GetLinePlaneIntersection (plane, Line::FromTwoPoints (triangle[cutVertex], triangle[next]));
		if (vertexPos[cutVertex] == PointPlanePosition::FrontOfPlane) {
			result.frontTriangles.push_back (Triangle (triangle[cutVertex], nextIntersection.position, prevIntersection.position));
			result.backTriangles.push_back (Triangle (triangle[next], prevIntersection.position, nextIntersection.position));
			result.backTriangles.push_back (Triangle (triangle[prev], prevIntersection.position, triangle[next]));
		} else if (vertexPos[cutVertex] == PointPlanePosition::BackOfPlane) {
			result.backTriangles.push_back (Triangle (triangle[cutVertex], nextIntersection.position, prevIntersection.position));
			result.frontTriangles.push_back (Triangle (triangle[next], prevIntersection.position, nextIntersection.position));
			result.frontTriangles.push_back (Triangle (triangle[prev], prevIntersection.position, triangle[next]));
		} else {
			throw std::logic_error ("failed to cut triangle");
		}
	}

	return result;
}

double CalculateTriangleArea (double a, double b, double c)
{
	double s = (a + b + c) / 2.0;
	double areaSquare = s * (s - a) * (s - b) * (s - c);
	if (areaSquare < 0.0) {
		return 0.0;
	}
	return sqrt (areaSquare);
}

glm::dvec3 BarycentricInterpolation (const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3, const glm::dvec3& val1, const glm::dvec3& val2, const glm::dvec3& val3, const glm::dvec3& position)
{
	double edgeLength1 = glm::distance (v1, v2);
	double edgeLength2 = glm::distance (v2, v3);
	double edgeLength3 = glm::distance (v3, v1);

	double positionDistance1 = glm::distance (v1, position);
	double positionDistance2 = glm::distance (v2, position);
	double positionDistance3 = glm::distance (v3, position);

	double area = CalculateTriangleArea (edgeLength1, edgeLength2, edgeLength3);
	if (IsEqual (area, 0.0)) {
		return val1;
	}

	double area1 = CalculateTriangleArea (edgeLength1, positionDistance2, positionDistance1);
	double area2 = CalculateTriangleArea (edgeLength2, positionDistance3, positionDistance2);
	double area3 = CalculateTriangleArea (edgeLength3, positionDistance1, positionDistance3);

	glm::dvec3 interpolated1 = val1 * area2;
	glm::dvec3 interpolated2 = val2 * area3;
	glm::dvec3 interpolated3 = val3 * area1;

	glm::dvec3 interpolated = (interpolated1 + interpolated2 + interpolated3) / area;
	return interpolated;
}

Orientation GetTriangleOrientation2D (const glm::dvec2& v1, const glm::dvec2& v2, const glm::dvec2& v3)
{
	double m00 = v1.x;
	double m01 = v1.y;
	double m10 = v2.x;
	double m11 = v2.y;
	double m20 = v3.x;
	double m21 = v3.y;

	double determinant = m00 * m11 + m01 * m20 + m10 * m21 - m11 * m20 - m01 * m10 - m00 * m21;
	if (IsGreater (determinant, 0.0)) {
		return Orientation::CounterClockwise;
	} else if (IsLower (determinant, 0.0)) {
		return Orientation::Clockwise;
	}

	return Orientation::Invalid;
}

static double GetPolygonSignedArea (const std::vector<glm::dvec2>& points)
{
	if (points.size () < 3) {
		return 0.0;
	}
	
	double area = 0.0;
	for (size_t i = 0; i < points.size (); i++) {
		const glm::dvec2& current = points[i];
		const glm::dvec2& next = points[(i + 1) % points.size ()];
		area += current.x * next.y - next.x * current.y;
	}
	area *= 0.5;

	return area;
}

Orientation GetPolygonOrientation2D (const std::vector<glm::dvec2>& points)
{
	double area = GetPolygonSignedArea (points);
	if (Geometry::IsGreater (area, 0.0)) {
		return Orientation::CounterClockwise;
	} else if (Geometry::IsLower (area, 0.0)) {
		return Orientation::Clockwise;
	}
	return Orientation::Invalid;
}

}
