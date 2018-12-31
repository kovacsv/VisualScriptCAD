#ifndef MODELER_MESHGENERATORS_HPP
#define MODELER_MESHGENERATORS_HPP

#include "Model.hpp"
#include "IncludeGLM.hpp"

namespace Modeler
{

Mesh GenerateBox (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize);
Mesh GenerateCylinder (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth);
Mesh GenerateCone (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth);
Mesh GenerateSphere (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth);
Mesh GenerateTorus (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth);

}

#endif
