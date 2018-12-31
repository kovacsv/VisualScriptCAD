#include "BSPTree.hpp"
#include "TriangleUtils.hpp"

namespace Geometry
{

BSPTriangleClipper::BSPTriangleClipper ()
{
}

BSPTriangleClipper::~BSPTriangleClipper ()
{
}

BSPTreeNode::BSPTreeNode (const Triangle& triangle) :
	plane (GetTrianglePlane (triangle))
{
	planeTriangles.push_back (triangle);
}

BSPTreeNode::BSPTreeNode (const std::vector<Triangle>& triangles) :
	plane (GetTrianglePlane (triangles[0]))
{
	for (const Triangle& triangle : triangles) {
		planeTriangles.push_back (triangle);
	}
}

void BSPTreeNode::AddTriangle (const Triangle& triangle)
{
	TrianglePlaneCutResult cutResult = CutTriangleWithPlane (plane, triangle);
	if (!cutResult.frontTriangles.empty ()) {
		if (frontNode == nullptr) {
			frontNode.reset (new BSPTreeNode (cutResult.frontTriangles));
		} else {
			for (const Triangle& cutTriangle : cutResult.frontTriangles) {
				frontNode->AddTriangle (cutTriangle);
			}
		}
	}
	if (!cutResult.backTriangles.empty ()) {
		if (backNode == nullptr) {
			backNode.reset (new BSPTreeNode (cutResult.backTriangles));
		} else {
			for (const Triangle& cutTriangle : cutResult.backTriangles) {
				backNode->AddTriangle (cutTriangle);
			}
		}
	}
	if (!cutResult.planeTriangles.empty ()) {
		planeTriangles.push_back (triangle);
	}
}

void BSPTreeNode::ClipTriangle (const Triangle& triangle, BSPTriangleClipper& clipper) const
{
	TrianglePlaneCutResult cutResult = CutTriangleWithPlane (plane, triangle);
	if (!cutResult.frontTriangles.empty ()) {
		if (frontNode == nullptr) {
			clipper.FrontTrianglesFound (cutResult.frontTriangles);
		} else {
			for (const Triangle& cutTriangle : cutResult.frontTriangles) {
				frontNode->ClipTriangle (cutTriangle, clipper);
			}
		}
	}
	if (!cutResult.backTriangles.empty ()) {
		if (backNode == nullptr) {
			clipper.BackTrianglesFound (cutResult.backTriangles);
		} else {
			for (const Triangle& cutTriangle : cutResult.backTriangles) {
				backNode->ClipTriangle (cutTriangle, clipper);
			}
		}
	}
	if (!cutResult.planeTriangles.empty ()) {
		clipper.PlaneTrianglesFound (cutResult.planeTriangles);
	}
}

BSPTree::BSPTree ()
{
}

void BSPTree::AddTriangle (const Triangle& triangle)
{
	if (rootNode == nullptr) {
		rootNode.reset (new BSPTreeNode (triangle));
	} else {
		rootNode->AddTriangle (triangle);
	}
}

void BSPTree::ClipTriangle (const Triangle& triangle, BSPTriangleClipper& clipper) const
{
	if (rootNode == nullptr) {
		return;
	}
	rootNode->ClipTriangle (triangle, clipper);
}

}
