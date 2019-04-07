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

class PolygonalGenerator
{
public:
	enum class VertexType
	{
		Sharp,
		Soft
	};

	PolygonalGenerator (const Material& material, const glm::dmat4& transformation, double height);
	virtual ~PolygonalGenerator ();

	void						AddVertex (const glm::dvec2& position, VertexType vertexType);
	Mesh						Generate () const;

protected:
	glm::dvec3					CalculateNormal (unsigned int vertexIndex) const;
	glm::dvec3					CalculateSharpNormal (unsigned int vertexIndex) const;

	virtual bool				GenerateInternal (Mesh& mesh, MaterialId materialId) const = 0;

	Material					material;
	glm::dmat4					transformation; 
	double						height;

	std::vector<glm::dvec2>		basePolygon;
	std::vector<VertexType>		basePolygonVertexTypes;
	unsigned int				vertexCount;
};

class PrismGenerator : public PolygonalGenerator
{
public:
	PrismGenerator (const Material& material, const glm::dmat4& transformation, double height);
	virtual ~PrismGenerator ();

protected:
	virtual bool	GenerateInternal (Mesh& mesh, MaterialId materialId) const;

	bool			AddTopAndBottomVertices (Mesh& mesh) const;
	bool			AddSideTriangles (Mesh& mesh, MaterialId materialId) const;
	virtual bool	AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const = 0;

	unsigned int	GetBottomVertex (unsigned int vertexIndex) const;
	unsigned int	GetTopVertex (unsigned int vertexIndex) const;
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
