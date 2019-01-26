#include "Triangulation.hpp"

#pragma warning (push)
#pragma warning (disable : 4456)
#pragma warning (disable : 4457)
#pragma warning (disable : 4458)
#pragma warning (disable : 4702)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#pragma warning (pop)

#include <queue>

struct FaceInfo
{
	FaceInfo () :
		nestingLevel (-1)
	{
	
	}

	bool IsInDomain ()
	{ 
		return nestingLevel % 2 == 1;
	}

	int nestingLevel;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel																	CGAL_Kernel;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, CGAL_Kernel>													CGAL_VertexWithInfo;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, CGAL_Kernel>													CGAL_FaceWithInfo;
typedef CGAL::Constrained_triangulation_face_base_2<CGAL_Kernel, CGAL_FaceWithInfo>											CGAL_ConstrainedFace;
typedef CGAL::Triangulation_data_structure_2<CGAL_VertexWithInfo, CGAL_ConstrainedFace>										CGAL_TriangulationDataStructure;
typedef CGAL::Exact_predicates_tag																							CGAL_ExactPredicatesTag;
typedef CGAL::Constrained_Delaunay_triangulation_2<CGAL_Kernel, CGAL_TriangulationDataStructure, CGAL_ExactPredicatesTag>	CGAL_Triangulation;
typedef CGAL_Triangulation::Point													                                        CGAL_TriangulationPoint;

namespace CGALOperations
{

static void MarkDomains (const CGAL_Triangulation& triangulation, const CGAL_Triangulation::Face_handle& start, int index, std::queue<CGAL_Triangulation::Edge>& border)
{
	if (start->info ().nestingLevel != -1) {
		return;
	}

	std::queue<CGAL_Triangulation::Face_handle> queue;
	queue.push (start);
	while (!queue.empty ()) {
		CGAL_Triangulation::Face_handle face = queue.front ();
		queue.pop ();
		if (face->info ().nestingLevel == -1) {
			face->info ().nestingLevel = index;
			for (int i = 0; i < 3; i++) {
				CGAL_Triangulation::Edge edge (face, i);
				CGAL_Triangulation::Face_handle neigFace = face->neighbor (i);
				if (neigFace == nullptr) {
					throw std::logic_error ("invalid face handle found");
				}
				if (neigFace->info ().nestingLevel == -1) {
					if (triangulation.is_constrained (edge)) {
						border.push (edge);
					} else {
						queue.push (neigFace);
					}
				}
			}
		}

	}
}

static void MarkDomains (const CGAL_Triangulation& triangulation)
{
	std::queue <CGAL_Triangulation::Edge> border;
	MarkDomains (triangulation, triangulation.infinite_face (), 0, border);
	while (!border.empty ()) {
		CGAL_Triangulation::Edge edge = border.front ();
		border.pop ();
		CGAL_Triangulation::Face_handle neigFace = edge.first->neighbor (edge.second);
		if (neigFace->info ().nestingLevel == -1) {
			MarkDomains (triangulation, neigFace, edge.first->info ().nestingLevel + 1, border);
		}
	}
}

void TriangulatePolygon (const std::vector<glm::dvec2>& points, const std::function<void (size_t, size_t, size_t)>& processor)
{
	std::vector<CGAL_TriangulationPoint> cgalPoints;
	for (const glm::dvec2& point : points) {
		cgalPoints.push_back (CGAL_TriangulationPoint (point.x, point.y));
	}

	CGAL_Triangulation triangulation;
	triangulation.insert_constraint (cgalPoints.begin (), cgalPoints.end (), true);
	
	{
		size_t vertexIndex = 0;
		for (auto it = triangulation.vertices_begin (); it != triangulation.vertices_end (); ++it) {
			it->info () = vertexIndex;
			vertexIndex++;
		}
		if (vertexIndex != points.size ()) {
			throw std::logic_error ("invalid triangulation data structure");
		}
	}

	MarkDomains (triangulation);

	for (CGAL_Triangulation::Finite_faces_iterator it = triangulation.finite_faces_begin (); it != triangulation.finite_faces_end (); ++it) {
		if (!it->info ().IsInDomain ()) {
			continue;
		}
		processor (it->vertex (0)->info (), it->vertex (1)->info (), it->vertex (2)->info ());
	}
}

bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result)
{
	bool success = true;
	try {
		TriangulatePolygon (points, [&] (size_t a, size_t b, size_t c) {
			result.push_back ({ a, b, c });
		});
	} catch (...) {
		success = false;
	}
	return success;
}

}
