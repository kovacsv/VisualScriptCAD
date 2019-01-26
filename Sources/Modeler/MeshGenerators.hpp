#ifndef MODELER_MESHGENERATORS_HPP
#define MODELER_MESHGENERATORS_HPP

#include "Model.hpp"
#include "IncludeGLM.hpp"

#include <array>

namespace Modeler
{

class Triangulator
{
public:
	virtual ~Triangulator ();

	virtual bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result) = 0;
};

Mesh GenerateBox (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize);
Mesh GenerateCylinder (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth);
Mesh GenerateCone (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth);
Mesh GenerateSphere (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth);
Mesh GenerateTorus (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth);
Mesh GeneratePrism (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, Triangulator& triangulator);

}

#endif
