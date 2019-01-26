#ifndef CGAL_TRIANGULATION_HPP
#define CGAL_TRIANGULATION_HPP

#include "IncludeGLM.hpp"
#include <functional>
#include <vector>
#include <array>

namespace CGALOperations
{

void TriangulatePolygon (const std::vector<glm::dvec2>& points, const std::function<void (size_t, size_t, size_t)>& processor);
bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result);


}

#endif
