#include "Mesh.hpp"

#include "IncludeGLM.hpp"
#include "TriangleUtils.hpp"

#include <atomic>

namespace Modeler
{

const Material DefaultMaterial (glm::dvec3 (0.0f, 0.5f, 0.7f));

Material::Material () :
	color (0.0f, 0.0f, 0.0f)
{

}

Material::Material (const glm::dvec3& color) :
	color (color)
{

}

const glm::dvec3& Material::GetColor () const
{
	return color;
}

void Material::SetColor (const glm::dvec3& newColor)
{
	color = newColor;
}

Checksum Material::CalcCheckSum () const
{
	Checksum result;
	result.Add (color.x);
	result.Add (color.y);
	result.Add (color.z);
	return result;
}

MeshTriangle::MeshTriangle (unsigned int v1, unsigned int v2, unsigned int v3,
							unsigned int n1, unsigned int n2, unsigned int n3) :
	v1 (v1),
	v2 (v2),
	v3 (v3),
	n1 (n1),
	n2 (n2),
	n3 (n3)
{

}

Checksum MeshTriangle::CalcCheckSum () const
{
	Checksum result;
	result.Add (v1);
	result.Add (v2);
	result.Add (v3);
	result.Add (n1);
	result.Add (n2);
	result.Add (n3);
	return result;
}

MeshGeometry::MeshGeometry ()
{

}

unsigned int MeshGeometry::AddVertex (double x, double y, double z)
{
	return AddVertex (glm::dvec3 (x, y, z));
}

unsigned int MeshGeometry::AddVertex (const glm::dvec3& vertex)
{
	vertices.push_back (vertex);
	return (unsigned int) vertices.size () - 1;
}

unsigned int MeshGeometry::AddNormal (double x, double y, double z)
{
	return AddNormal (glm::dvec3 (x, y, z));
}

unsigned int MeshGeometry::AddNormal (const glm::dvec3& normal)
{
	normals.push_back (normal);
	return (unsigned int) normals.size () - 1;
}

unsigned int MeshGeometry::AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3)
{
	glm::dvec3 normal = Geometry::CalculateTriangleNormal (vertices[v1], vertices[v2], vertices[v3]);
	unsigned int normalIndex = (unsigned int) AddNormal (normal);
	return AddTriangle (v1, v2, v3, normalIndex, normalIndex, normalIndex);
}

unsigned int MeshGeometry::AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, unsigned int n1, unsigned int n2, unsigned int n3)
{
	triangles.push_back (MeshTriangle (v1, v2, v3, n1, n2, n3));
	return (unsigned int) triangles.size () - 1;
}

unsigned int MeshGeometry::VertexCount () const
{
	return (unsigned int) vertices.size ();
}

unsigned int MeshGeometry::NormalCount () const
{
	return (unsigned int) normals.size ();
}

unsigned int MeshGeometry::TriangleCount () const
{
	return (unsigned int) triangles.size ();
}

const glm::dvec3& MeshGeometry::GetVertex (unsigned int index) const
{
	return vertices[index];
}

glm::dvec3 MeshGeometry::GetVertex (unsigned int index, const glm::dmat4& transformation) const
{
	return glm::dvec3 (transformation * glm::dvec4 (vertices[index], 1.0));
}

const glm::dvec3& MeshGeometry::GetNormal (unsigned int index) const
{
	return normals[index];
}

glm::dvec3 MeshGeometry::GetNormal (unsigned int index, const glm::dmat4& transformation) const
{
	return glm::mat3 (transformation) * normals[index];
}

const MeshTriangle& MeshGeometry::GetTriangle (unsigned int index) const
{
	return triangles[index];
}

void MeshGeometry::EnumerateVertices (const glm::dmat4& transformation, const std::function<void (const glm::dvec3&)>& processor) const
{
	for (const glm::dvec3& vertex : vertices) {
		processor (glm::dvec3 (transformation * glm::dvec4 (vertex, 1.0)));
	}
}

void MeshGeometry::EnumerateNormals (const glm::dmat4& transformation, const std::function<void (const glm::dvec3&)>& processor) const
{
	glm::mat3 normalTransformation (transformation);
	for (const glm::dvec3& normal : normals) {
		processor (normalTransformation * normal);
	}
}

void MeshGeometry::EnumerateTriangles (const std::function<void (const MeshTriangle&)>& processor) const
{
	for (const MeshTriangle& triangle : triangles) {
		processor (triangle);
	}
}

Checksum MeshGeometry::CalcCheckSum () const
{
	Checksum result;
	for (const glm::dvec3& vec : vertices) {
		result.Add (vec.x);
		result.Add (vec.y);
		result.Add (vec.z);
	}
	for (const glm::dvec3& vec : normals) {
		result.Add (vec.x);
		result.Add (vec.y);
		result.Add (vec.z);
	}
	for (const MeshTriangle& tri : triangles) {
		result.Add (tri.CalcCheckSum ());
	}
	return result;
}

MeshMaterials::MeshMaterials ()
{

}

MaterialId MeshMaterials::AddMaterial (const Material& material)
{
	materials.push_back (material);
	return (MaterialId) (materials.size () - 1);
}

const Material& MeshMaterials::GetMaterial (MaterialId materialId) const
{
	return materials[materialId];
}

void MeshMaterials::EnumerateMaterials (const std::function<void (MaterialId, const Material&)>& processor) const
{
	for (MaterialId materialId = 0; materialId < materials.size (); materialId++) {
		processor (materialId, materials[materialId]);
	}
}

unsigned int MeshMaterials::MaterialCount () const
{
	return (unsigned int) materials.size ();
}

void MeshMaterials::AddTriangleMaterial (MaterialId materialId)
{
	triangleMaterials.push_back (materialId);
}

MaterialId MeshMaterials::GetTriangleMaterial (unsigned int triangleIndex) const
{
	return triangleMaterials[triangleIndex];
}

Checksum MeshMaterials::CalcCheckSum () const
{
	Checksum result;
	for (const Material& material : materials) {
		result.Add (material.CalcCheckSum ());
	}
	for (MaterialId materialId : triangleMaterials) {
		result.Add (materialId);
	}
	return result;
}

Mesh::Mesh () :
	geometry (),
	materials (),
	transformation (1.0)
{

}

MaterialId Mesh::AddMaterial (const Material& material)
{
	return materials.AddMaterial (material);
}

unsigned int Mesh::AddVertex (double x, double y, double z)
{
	return geometry.AddVertex (glm::dvec3 (x, y, z));
}

unsigned int Mesh::AddVertex (const glm::dvec3& vertex)
{
	return geometry.AddVertex (vertex);
}

unsigned int Mesh::AddNormal (double x, double y, double z)
{
	return geometry.AddNormal (glm::dvec3 (x, y, z));
}

unsigned int Mesh::AddNormal (const glm::dvec3& normal)
{
	return geometry.AddNormal (normal);
}

unsigned int Mesh::AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, MaterialId mat)
{
	materials.AddTriangleMaterial (mat);
	return geometry.AddTriangle (v1, v2, v3);
}

unsigned int Mesh::AddTriangle (unsigned int v1, unsigned int v2, unsigned int v3, unsigned int n1, unsigned int n2, unsigned int n3, MaterialId mat)
{
	materials.AddTriangleMaterial (mat);
	return geometry.AddTriangle (v1, v2, v3, n1, n2, n3);
}

const MeshGeometry& Mesh::GetGeometry () const
{
	return geometry;
}

const MeshMaterials& Mesh::GetMaterials () const
{
	return materials;
}

const glm::dmat4& Mesh::GetTransformation () const
{
	return transformation;
}

void Mesh::SetTransformation (const glm::dmat4& newTransformation)
{
	transformation = newTransformation;
}

void EnumerateTrianglesByMaterial (const MeshGeometry& geometry, const MeshMaterials& materials, const std::function<void (MaterialId, const std::vector<unsigned int>&)>& processor)
{
	std::vector<MaterialId> materialVector;
	std::unordered_map<MaterialId, std::vector<unsigned int>> trianglesByMaterial;
	for (MaterialId materialId = 0; materialId < (int) materials.MaterialCount (); materialId++) {
		materialVector.push_back (materialId);
		trianglesByMaterial.insert ({ materialId, std::vector<unsigned int> ()});
	}
	for (unsigned int i = 0; i < geometry.TriangleCount (); i++) {
		MaterialId materialId = materials.GetTriangleMaterial (i);
		trianglesByMaterial[materialId].push_back (i);
	};
	std::sort (materialVector.begin (), materialVector.end ());
	for (MaterialId materialId : materialVector) {
		processor (materialId, trianglesByMaterial[materialId]);
	}
}

}
