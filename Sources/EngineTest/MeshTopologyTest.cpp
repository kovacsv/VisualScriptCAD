#include "SimpleTest.hpp"
#include "MeshTopology.hpp"

using namespace Modeler;

namespace MeshTopologyTest
{

TEST (MeshTopologyTest_OneTriangle)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (topology.GetTriangles ().size () == 1);
	ASSERT (topology.GetEdges ().size () == 3);
	ASSERT (!topology.IsEmpty ());
	ASSERT (!topology.IsClosed ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Valid);
}

TEST (MeshTopologyTest_TwoTriangles)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (1, 3, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (topology.GetTriangles ().size () == 2);
	ASSERT (topology.GetEdges ().size () == 5);
	ASSERT (!topology.IsEmpty ());
	ASSERT (!topology.IsClosed ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Valid);

	const std::vector<MeshTopology::Triangle>& triangles = topology.GetTriangles ();
	const std::vector<MeshTopology::Edge>& edges = topology.GetEdges ();
	ASSERT (triangles[0].edge1.edge == 0 && triangles[0].edge1.reversed == false);
	ASSERT (triangles[0].edge2.edge == 1 && triangles[0].edge2.reversed == false);
	ASSERT (triangles[0].edge3.edge == 2 && triangles[0].edge3.reversed == false);
	ASSERT (triangles[1].edge1.edge == 3 && triangles[1].edge1.reversed == false);
	ASSERT (triangles[1].edge2.edge == 4 && triangles[1].edge2.reversed == false);
	ASSERT (triangles[1].edge3.edge == 1 && triangles[1].edge3.reversed == true);
	ASSERT (edges[0].beg == 0 && edges[0].end == 1 && edges[0].triangle1 == 0 && edges[0].triangle2 == NoTriangle);
	ASSERT (edges[1].beg == 1 && edges[1].end == 2 && edges[1].triangle1 == 0 && edges[1].triangle2 == 1);
	ASSERT (edges[2].beg == 2 && edges[2].end == 0 && edges[2].triangle1 == 0 && edges[2].triangle2 == NoTriangle);
	ASSERT (edges[3].beg == 1 && edges[3].end == 3 && edges[3].triangle1 == 1 && edges[3].triangle2 == NoTriangle);
	ASSERT (edges[4].beg == 3 && edges[4].end == 2 && edges[4].triangle1 == 1 && edges[4].triangle2 == NoTriangle);
}

TEST (MeshTopologyTest_TwoTrianglesNonManifold1)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (1, 2, 3) == MeshTopologyBuilder::Result::NonManifoldEdgeFound);
	ASSERT (topology.IsEmpty ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Invalid);
}

TEST (MeshTopologyTest_TwoTrianglesNonManifold2)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (1, 3, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (4, 1, 2) == MeshTopologyBuilder::Result::NonManifoldEdgeFound);
	ASSERT (topology.IsEmpty ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Invalid);
}

TEST (MeshTopologyTest_TwoTrianglesNonManifold3)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (1, 3, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (4, 2, 1) == MeshTopologyBuilder::Result::NonManifoldEdgeFound);
	ASSERT (topology.IsEmpty ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Invalid);

	ASSERT (builder.AddTriangle (4, 2, 1) == MeshTopologyBuilder::Result::InvalidTopology);
	ASSERT (topology.IsEmpty ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Invalid);
}

TEST (MeshTopologyTest_TetrahedronTest)
{
	MeshTopology topology;
	MeshTopologyBuilder builder (topology);

	ASSERT (builder.AddTriangle (0, 1, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (3, 1, 0) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (3, 2, 1) == MeshTopologyBuilder::Result::NoError);
	ASSERT (builder.AddTriangle (3, 0, 2) == MeshTopologyBuilder::Result::NoError);
	ASSERT (!topology.IsEmpty ());
	ASSERT (topology.IsClosed ());
	ASSERT (topology.GetStatus () == MeshTopology::Status::Valid);
}

}
