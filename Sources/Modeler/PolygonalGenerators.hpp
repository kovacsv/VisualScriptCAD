#ifndef MODELER_POLYGONALGENERATORS_HPP
#define MODELER_POLYGONALGENERATORS_HPP

#include "Model.hpp"
#include "IncludeGLM.hpp"

#include <array>

namespace Modeler
{

class Triangulator
{
public:
	Triangulator ();
	virtual ~Triangulator ();

	virtual bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result) = 0;
};

typedef std::shared_ptr<Triangulator> TriangulatorPtr;

class NaiveTriangulator : public Triangulator
{
public:
	NaiveTriangulator ();

	virtual bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result) override;
};

class PrismGenerator
{
public:
	enum class VertexType
	{
		Sharp,
		Soft
	};

	PrismGenerator (const Material& material, const glm::dmat4& transformation, double height);
	virtual ~PrismGenerator ();

	void						AddVertex (const glm::dvec2& position, VertexType vertexType);
	Mesh						Generate () const;

protected:
	bool						AddTopAndBottomVertices (Mesh& mesh) const;
	bool						AddSideTriangles (Mesh& mesh, MaterialId materialId) const;
	virtual bool				AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const = 0;

	glm::dvec3					CalculateNormal (unsigned int edgeIndex) const;
	glm::dvec3					CalculateSharpNormal (unsigned int edgeIndex) const;

	unsigned int				GetBottomVertex (unsigned int vertexIndex) const;
	unsigned int				GetTopVertex (unsigned int vertexIndex) const;

	Material					material;
	glm::dmat4					transformation; 
	double						height;

	std::vector<glm::dvec2>		basePolygon;
	std::vector<VertexType>		basePolygonVertexTypes;
	unsigned int				vertexCount;
};

class TriangulatedPrismGenerator : public PrismGenerator
{
public:
	TriangulatedPrismGenerator (const Material& material, const glm::dmat4& transformation, double height, Triangulator& triangulator);

private:
	virtual bool	AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const override;

	Triangulator&	triangulator;
};

class CenterPointTriangulatedPrismGenerator : public PrismGenerator
{
public:
	CenterPointTriangulatedPrismGenerator (const Material& material, const glm::dmat4& transformation, const glm::dvec2& center, double height);

private:
	virtual bool	AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const override;

	glm::dvec2		center;
};

}

#endif
