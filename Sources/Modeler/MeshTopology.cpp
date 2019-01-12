#include "MeshTopology.hpp"

namespace Modeler
{

const unsigned int NoTriangle = (unsigned int) -1;

MeshTopology::MeshTopology () :
	isValid (true)
{

}

const std::vector<MeshTopology::Triangle>& MeshTopology::GetTriangles () const
{
	return triangles;
}

const std::vector<MeshTopology::Edge>& MeshTopology::GetEdges () const
{
	return edges;
}

bool MeshTopology::IsValid () const
{
	return isValid;
}

bool MeshTopology::IsEmpty () const
{
	return triangles.empty () && edges.empty ();
}

void MeshTopology::Clear ()
{
	triangles.clear ();
	edges.clear ();
}

bool MeshTopology::IsClosed () const
{
	for (const Edge& edge : edges) {
		if (edge.triangle1 == NoTriangle || edge.triangle2 == NoTriangle) {
			return false;
		}
	}
	return true;
}

bool MeshTopologyBuilder::EdgeKey::operator== (const EdgeKey& rhs) const
{
	return (beg == rhs.beg && end == rhs.end) || (beg == rhs.end && end == rhs.beg);
}

MeshTopologyBuilder::MeshTopologyBuilder (MeshTopology& topology) :
	topology (topology)
{

}

MeshTopologyBuilder::Result MeshTopologyBuilder::AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3)
{
	if (!topology.isValid) {
		return Result::InvalidTopology;
	}

	MeshTopology::Triangle triangle;
	Result result = AddTriangleEdges (triangle, v1, v2, v3);
	if (result != Result::NoError) {
		topology.isValid = false;
		topology.Clear ();
		return result;
	}

	topology.triangles.push_back (triangle);
	return Result::NoError;
}

MeshTopologyBuilder::Result MeshTopologyBuilder::AddTriangleEdges (MeshTopology::Triangle& triangle, unsigned int v1, unsigned int v2, unsigned int v3)
{
	unsigned int triangleIndex = (unsigned int) topology.triangles.size ();
	Result edgeResult1 = AddEdge (v1, v2, triangleIndex, triangle.edge1);
	if (edgeResult1 != Result::NoError) {
		return edgeResult1;
	}
	Result edgeResult2 = AddEdge (v2, v3, triangleIndex, triangle.edge2);
	if (edgeResult2 != Result::NoError) {
		return edgeResult2;
	}
	Result edgeResult3 = AddEdge (v3, v1, triangleIndex, triangle.edge3);
	if (edgeResult3 != Result::NoError) {
		return edgeResult3;
	}
	return Result::NoError;
}

MeshTopologyBuilder::Result MeshTopologyBuilder::AddEdge (unsigned int v1, unsigned int v2, unsigned int triangle, MeshTopology::TriangleEdge& triEdge)
{
	EdgeKey key {v1, v2};

	auto found = edgeMap.find (key);
	if (found == edgeMap.end ()) {
		topology.edges.push_back (MeshTopology::Edge { v1, v2, NoTriangle, NoTriangle });
		edgeMap.insert ({ key, (unsigned int) topology.edges.size () - 1 });
	}

	triEdge.edge = edgeMap[key];
	triEdge.reversed = false;
	MeshTopology::Edge& edge = topology.edges[triEdge.edge];

	if (edge.beg == v1 && edge.end == v2) {
		if (edge.triangle1 != NoTriangle) {
			return Result::NonManifoldEdgeFound;
		}
		edge.triangle1 = triangle;
		triEdge.reversed = false;
	} else if (edge.beg == v2 && edge.end == v1) {
		if (edge.triangle2 != NoTriangle) {
			return Result::NonManifoldEdgeFound;
		}
		edge.triangle2 = triangle;
		triEdge.reversed = true;
	} else {
		throw std::logic_error ("internal error");
	}

	return Result::NoError;
}

}
