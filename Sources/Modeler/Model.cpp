#include "Model.hpp"

#include "IncludeGLM.hpp"
#include "TriangleUtils.hpp"

#include <atomic>

namespace Modeler
{

MeshRef::MeshRef (MeshGeometryId geometryId, MeshMaterialsId& materialsId, const glm::dmat4& transformation) :
	geometryId (geometryId),
	materialsId (materialsId),
	transformation (transformation)
{
}

MeshGeometryId MeshRef::GetGeometryId () const
{
	return geometryId;
}

MeshMaterialsId MeshRef::GetMaterialsId () const
{
	return materialsId;
}

const glm::dmat4& MeshRef::GetTransformation () const
{
	return transformation;
}

Model::Model () :
	nextMeshId (0),
	nextMaterialId (0)
{
	Clear ();
}

const MeshGeometry& Model::GetMeshGeometry (MeshGeometryId geometryId) const
{
	return geometries.GetData (geometryId);
}

const MeshMaterials& Model::GetMeshMaterials (MeshMaterialsId materialsId) const
{
	return materials.GetData (materialsId);
}

const MeshRef& Model::GetMesh (MeshId meshId) const
{
	return meshRefs.at (meshId);
}

void Model::EnumerateMeshes (const std::function<void (MeshId, const MeshRef&)>& processor) const
{
	for (const auto& it : meshRefs) {
		processor (it.first, it.second);
	}
}

MeshId Model::AddMesh (const Mesh& mesh)
{
	const MeshGeometry& meshGeometry = mesh.GetGeometry ();
	const MeshMaterials& meshMaterials = mesh.GetMaterials ();
	const glm::dmat4& transformation = mesh.GetTransformation ();

	MeshGeometryId meshGeometryId = geometries.AddReference (meshGeometry, meshGeometry.CalcCheckSum ());
	MeshGeometryId meshMaterialsId = materials.AddReference (meshMaterials, meshMaterials.CalcCheckSum ());

	MeshRef meshRef (meshGeometryId, meshMaterialsId, transformation);
	MeshId meshId = nextMeshId++;
	meshRefs.insert ({ meshId, meshRef });
	return meshId;
}

void Model::RemoveMesh (MeshId meshId)
{
	const MeshRef& meshRef = meshRefs.at (meshId);
	MeshGeometryId meshGeometryId = meshRef.GetGeometryId ();
	MeshGeometryId meshMaterialsId = meshRef.GetMaterialsId ();
	geometries.RemoveReference (meshGeometryId);
	materials.RemoveReference (meshMaterialsId);
	meshRefs.erase (meshId);
}

void Model::Clear ()
{
	geometries.Clear ();
	materials.Clear ();
	meshRefs.clear ();
	nextMeshId = 0;
	nextMaterialId = 0;
}

ModelInfo Model::GetModelInfo () const
{
	ModelInfo modelInfo;
	modelInfo.meshGeometryCount = (unsigned int) geometries.DataCount ();
	modelInfo.meshMaterialsCount = (unsigned int) materials.DataCount ();
	modelInfo.meshCount = (unsigned int) meshRefs.size ();
	modelInfo.vertexCount = 0;
	modelInfo.triangleCount = 0;
	for (const auto& it : meshRefs) {
		const MeshRef& meshRef = it.second;
		const MeshGeometry& geometry = geometries.GetData (meshRef.GetGeometryId ());
		modelInfo.vertexCount += geometry.VertexCount ();
		modelInfo.triangleCount += geometry.TriangleCount ();
	}
	return modelInfo;
}

}
