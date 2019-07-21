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

void MeshRef::SetUserData (const std::string& key, const UserDataConstPtr& data)
{
	userData.Set (key, data);
}

UserDataConstPtr MeshRef::GetUserData (const std::string & key) const
{
	return userData.Get (key);
}

Model::Model () :
	nextMeshId (0),
	nextMaterialId (0)
{
	Clear ();
}

const MeshGeometry& Model::GetMeshGeometry (const MeshRef& meshRef) const
{
	return geometries.GetData (meshRef.GetGeometryId ());
}

const MeshMaterials& Model::GetMeshMaterials (const MeshRef& meshRef) const
{
	return materials.GetData (meshRef.GetMaterialsId ());
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

void Model::SetMeshUserData (MeshId meshId, const std::string& key, const UserDataConstPtr& data)
{
	MeshRef& meshRef = meshRefs.at (meshId);
	meshRef.SetUserData (key, data);
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

ModelInfo Model::GetInfo () const
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

Geometry::BoundingBox Model::GetBoundingBox () const
{
	Geometry::BoundingBox boundingBox;
	EnumerateMeshes ([&] (MeshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = GetMeshGeometry (meshRef);
		const Geometry::BoundingBox& geometryBoundingBox = geometry.GetBoundingBox ();
		const glm::dmat4& transformation = meshRef.GetTransformation ();
		Geometry::BoundingBox transformedBoundingBox = geometryBoundingBox.Transform (transformation);
		transformedBoundingBox.EnumerateBoundingPoints ([&] (const glm::dvec3& point) {
			boundingBox.AddPoint (point);
		});
	});
	return boundingBox;
}

Geometry::BoundingSphere Model::GetBoundingSphere () const
{
	Geometry::BoundingBox boundingBox = GetBoundingBox ();
	if (!boundingBox.IsValid ()) {
		return Geometry::InvalidBoundingSphere;
	}

	glm::dvec3 center = boundingBox.GetCenter ();
	double radius = glm::distance (center, boundingBox.GetMax ());
	return Geometry::BoundingSphere (center, radius);
}

}
