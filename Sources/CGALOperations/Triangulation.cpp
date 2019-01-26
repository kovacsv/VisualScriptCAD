#include "Triangulation.hpp"

#pragma warning (push)
#pragma warning (disable : 4456)
#pragma warning (disable : 4457)
#pragma warning (disable : 4458)
#pragma warning (disable : 4702)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Polygon_2.h>
#pragma warning (pop)


typedef CGAL::Exact_predicates_inexact_constructions_kernel															CGAL_Kernel;
typedef CGAL::Exact_predicates_tag 																					CGAL_ExactTag;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, CGAL_Kernel>											CGAL_VertexWithInfo;
typedef CGAL::Constrained_triangulation_face_base_2<CGAL_Kernel> Fb;
typedef CGAL::Triangulation_data_structure_2<CGAL_VertexWithInfo, Fb>													CGAL_TriangulationDataStructure;
typedef CGAL::Constrained_Delaunay_triangulation_2<CGAL_Kernel, CGAL_TriangulationDataStructure, CGAL_ExactTag>		CGAL_Triangulation;
typedef CGAL::Polygon_2<CGAL_Kernel>																				CGAL_Polygon;
typedef CGAL_Triangulation::Point																					CGAL_Point;

namespace CGALOperations
{

void TriangulatePolygon (const std::vector<glm::dvec2>& points)
{
	std::vector<std::pair<CGAL_Point, size_t>> pointsWithInfo;
	CGAL_Triangulation::Face_handle hint;
	CGAL_Triangulation triangulation;
	for (size_t i = 0; i < points.size (); i++) {
		const glm::dvec2& point = points[i];
		pointsWithInfo.push_back (std::make_pair (CGAL_Point (point.x, point.y), i));
		CGAL_Triangulation::Vertex_handle v_hint;
		triangulation.insert (CGAL_Point (point.x, point.y), hint);
	}

}

}
