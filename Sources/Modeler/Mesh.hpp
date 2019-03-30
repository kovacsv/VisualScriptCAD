#ifndef MODELER_MESH_HPP
#define MODELER_MESH_HPP

#include "Checksum.hpp"
#include "IncludeGLM.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

namespace Modeler
{

using MaterialId = int;

class Material
{
public:
	Material ();
	Material (const glm::dvec3& color);

	const glm::dvec3&	GetColor () const;
	void				SetColor (const glm::dvec3& newColor);

	Checksum			CalcCheckSum () const;

private:
	glm::dvec3 color;
};

extern const Material DefaultMaterial;

class MeshTriangle
{
public:
	MeshTriangle (	unsigned int v1, unsigned int v2, unsigned int v3,
					unsigned int n1, unsigned int n2, unsigned int n3);

	Checksum CalcCheckSum () const;

	unsigned int	v1;
	unsigned int	v2;
	unsigned int	v3;
	unsigned int	n1;
	unsigned int	n2;
	unsigned int	n3;
};

class MeshGeometry
{
public:
	MeshGeometry ();
	MeshGeometry (const MeshGeometry& rhs) = default;
	MeshGeometry (MeshGeometry&& rhs) = default;

	MeshGeometry&			operator= (const MeshGeometry& rhs) = default;
	MeshGeometry&			operator= (MeshGeometry&& rhs) = default;

	unsigned int			AddVertex (double x, double y, double z);
	unsigned int			AddVertex (const glm::dvec3& vertex);

	unsigned int			AddNormal (double x, double y, double z);
	unsigned int			AddNormal (const glm::dvec3& vertex);

	unsigned int			AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3);
	unsigned int			AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, unsigned int n1, unsigned int n2, unsigned int n3);

	unsigned int			VertexCount () const;
	unsigned int			NormalCount () const;
	unsigned int			TriangleCount () const;

	const glm::dvec3&		GetVertex (unsigned int index) const;
	glm::dvec3				GetVertex (unsigned int index, const glm::dmat4& transformation) const;

	const glm::dvec3&		GetNormal (unsigned int index) const;
	glm::dvec3				GetNormal (unsigned int index, const glm::dmat4& transformation) const;

	const MeshTriangle&		GetTriangle (unsigned int index) const;

	void					EnumerateVertices (const glm::dmat4& transformation, const std::function<void (const glm::dvec3&)>& processor) const;
	void					EnumerateNormals (const glm::dmat4& transformation, const std::function<void (const glm::dvec3&)>& processor) const;
	void					EnumerateTriangles (const std::function<void (const MeshTriangle&)>& processor) const;

	Checksum				CalcCheckSum () const;
	void					Clear ();

private:
	std::vector<glm::dvec3>		vertices;
	std::vector<glm::dvec3>		normals;
	std::vector<MeshTriangle>	triangles;
};

class MeshMaterials
{
public:
	MeshMaterials ();
	MeshMaterials (const MeshMaterials& rhs) = default;
	MeshMaterials (MeshMaterials&& rhs) = default;

	MeshMaterials&			operator= (const MeshMaterials& rhs) = default;
	MeshMaterials&			operator= (MeshMaterials&& MeshMaterials) = default;

	MaterialId				AddMaterial (const Material& material);
	const Material&			GetMaterial (MaterialId materialId) const;
	void					EnumerateMaterials (const std::function<void (MaterialId, const Material&)>& processor) const;
	unsigned int			MaterialCount () const;

	void					AddTriangleMaterial (MaterialId materialId);
	MaterialId				GetTriangleMaterial (unsigned int triangleIndex) const;

	Checksum				CalcCheckSum () const;
	void					Clear ();

private:
	std::vector<Material>		materials;
	std::vector<MaterialId>		triangleMaterials;
};

class Mesh
{
public:
	Mesh ();
	Mesh (const Mesh& rhs) = default;
	Mesh (Mesh&& rhs) = default;
	
	Mesh&					operator= (const Mesh& rhs) = default;
	Mesh&					operator= (Mesh&& rhs) = default;

	MaterialId				AddMaterial (const Material& material);

	unsigned int			AddVertex (double x, double y, double z);
	unsigned int			AddVertex (const glm::dvec3& vertex);

	unsigned int			AddNormal (double x, double y, double z);
	unsigned int			AddNormal (const glm::dvec3& vertex);

	unsigned int			AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, MaterialId mat);
	unsigned int			AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, unsigned int n1, unsigned int n2, unsigned int n3, MaterialId mat);

	const MeshGeometry&		GetGeometry () const;
	const MeshMaterials&	GetMaterials () const;

	const glm::dmat4&		GetTransformation () const;
	void					SetTransformation (const glm::dmat4& newTransformation);
	void					AddTransformation (const glm::dmat4& newTransformation);

	void					Clear ();

private:
	MeshGeometry			geometry;
	MeshMaterials			materials;
	glm::dmat4				transformation;
};

void EnumerateTrianglesByMaterial (const MeshGeometry& geometry, const MeshMaterials& materials, const std::function<void (MaterialId, const std::vector<unsigned int>&)>& processor);

}

#endif
