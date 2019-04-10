#ifndef MODELER_MESHGENERATORS_HPP
#define MODELER_MESHGENERATORS_HPP

#include "Model.hpp"
#include "IncludeGLM.hpp"
#include "PolygonalGenerators.hpp"

#include <array>

namespace Modeler
{

enum class PlatonicSolidType
{
	Tetrahedron,
	Octahedron,
	Hexahedron,
	Dodecahedron,
	Icosahedron
};

Mesh GenerateBox (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize);
Mesh GenerateBoxShell (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize, double thickness);
Mesh GenerateCylinder (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth);
Mesh GenerateCylinderShell (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth, double thickness);
Mesh GenerateCone (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth);
Mesh GenerateSphere (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth);
Mesh GenerateTorus (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth);
Mesh GeneratePrism (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, Triangulator& triangulator);
Mesh GeneratePrismShell (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, double thickness);
Mesh GeneratePlatonicSolid (const Material& material, const glm::dmat4& transformation, PlatonicSolidType type, double radius);

}

#endif
