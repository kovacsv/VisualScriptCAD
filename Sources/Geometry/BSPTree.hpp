#ifndef GEOMETRY_BSPTREE_HPP
#define GEOMETRY_BSPTREE_HPP

#include "Triangle.hpp"
#include "Plane.hpp"

#include <vector>
#include <memory>

namespace Geometry
{

class BSPTriangleClipper
{
public:
	BSPTriangleClipper ();
	virtual ~BSPTriangleClipper ();

	virtual void	FrontTrianglesFound (const std::vector<Triangle>& triangles) = 0;
	virtual void	BackTrianglesFound (const std::vector<Triangle>& triangles) = 0;
	virtual void	PlaneTrianglesFound (const std::vector<Triangle>& triangles) = 0;
};

class BSPTreeNode
{
public:
	BSPTreeNode (const Triangle& triangle);
	BSPTreeNode (const std::vector<Triangle>& triangles);

	void	AddTriangle (const Triangle& triangle);
	void	ClipTriangle (const Triangle& triangle, BSPTriangleClipper& clipper) const;

private:
	Plane							plane;
	std::vector<Triangle>			planeTriangles;
	std::unique_ptr<BSPTreeNode>	frontNode;
	std::unique_ptr<BSPTreeNode>	backNode;
};

class BSPTree
{
public:
	BSPTree ();

	void	AddTriangle (const Triangle& triangle);
	void	ClipTriangle (const Triangle& triangle, BSPTriangleClipper& clipper) const;

private:
	std::unique_ptr<BSPTreeNode>	rootNode;
};

}

#endif
