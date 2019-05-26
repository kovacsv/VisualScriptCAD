#include "RenderModelConverter.hpp"

#include <unordered_map>

MeshUniqueId::MeshUniqueId (Modeler::MeshGeometryId geometryId, Modeler::MeshMaterialsId materialsId) :
	geometryId (geometryId),
	materialsId (materialsId)
{
}

size_t MeshUniqueId::GenerateHashValue () const
{
	return std::hash<int> {} (geometryId) + 49157 * std::hash<int> {} (materialsId);
}

bool MeshUniqueId::operator== (const MeshUniqueId& rhs) const
{
	return geometryId == rhs.geometryId && materialsId == rhs.materialsId;
}

RenderModelConverter::RenderModelConverter () :
	nextRenderMeshId (0)
{
}

void RenderModelConverter::AddMeshToRenderModel (const Modeler::Model& model, Modeler::MeshId meshId, RenderModel& renderModel)
{
	const Modeler::MeshRef& meshRef = model.GetMesh (meshId);
	MeshUniqueId meshUniqueId (meshRef.GetGeometryId (), meshRef.GetMaterialsId ());
	if (meshIdToRenderMeshId.find (meshUniqueId) == meshIdToRenderMeshId.end ()) {
		RenderMesh renderMesh;
		ConvertMeshRefToRenderMesh (model, meshRef, renderMesh);
		renderModel.AddRenderMesh (nextRenderMeshId, renderMesh);
		meshIdToRenderMeshId.insert ({ meshUniqueId, nextRenderMeshId });
		renderMeshIdToMeshId.insert ({ nextRenderMeshId, meshUniqueId });
		nextRenderMeshId++;
	}
	RenderMeshId renderMeshId = meshIdToRenderMeshId.at (meshUniqueId);
	RenderMeshInstanceId instanceId = meshId;
	renderModel.AddRenderMeshInstance (renderMeshId, instanceId, RenderMeshInstance (glm::mat4 (meshRef.GetTransformation ())));
}

void RenderModelConverter::RemoveMeshFromRenderModel (Modeler::MeshId meshId, RenderModel& renderModel)
{
	RenderMeshInstanceId instanceId = meshId;
	RenderMeshId renderMeshId = renderModel.GetRenderMeshId (instanceId);
	renderModel.RemoveRenderMeshInstance (instanceId);
	if (!renderModel.ContainsRenderMesh (renderMeshId)) {
		meshIdToRenderMeshId.erase (renderMeshIdToMeshId.at (renderMeshId));
		renderMeshIdToMeshId.erase (renderMeshId);
	}
}

void RenderModelConverter::Clear ()
{
	meshIdToRenderMeshId.clear ();
	renderMeshIdToMeshId.clear ();
}

void RenderModelConverter::ConvertMeshRefToRenderMesh (const Modeler::Model& model, const Modeler::MeshRef& meshRef, RenderMesh& renderMesh)
{
	const Modeler::MeshGeometry& geometry = model.GetMeshGeometry (meshRef);
	const Modeler::MeshMaterials& materials = model.GetMeshMaterials (meshRef);

	std::unordered_map<Modeler::MaterialId, size_t> materialToRenderGeometry;
	EnumerateTrianglesByMaterial (geometry, materials, [&] (Modeler::MaterialId materialId, const std::vector<unsigned int>& triangles) {
		if (materialToRenderGeometry.find (materialId) == materialToRenderGeometry.end ()) {
			renderMesh.AddRenderGeometry (RenderGeometry (MaterialToRenderMaterial (materials.GetMaterial (materialId))));
			materialToRenderGeometry.insert ({ materialId, renderMesh.RenderGeometryCount () - 1 });
		}
		RenderGeometry& renderGeometry = renderMesh.GetRenderGeometry (materialToRenderGeometry[materialId]);
		for (unsigned int triangleId : triangles) {
			const Modeler::MeshTriangle& triangle = geometry.GetTriangle (triangleId);
			glm::vec3 v1 = geometry.GetVertex (triangle.v1);
			glm::vec3 v2 = geometry.GetVertex (triangle.v2);
			glm::vec3 v3 = geometry.GetVertex (triangle.v3);
			glm::vec3 n1 = geometry.GetNormal (triangle.n1);
			glm::vec3 n2 = geometry.GetNormal (triangle.n2);
			glm::vec3 n3 = geometry.GetNormal (triangle.n3);
			renderGeometry.AddTriangle (v1, v2, v3, n1, n2, n3);
		}
	});
}

RenderMaterial RenderModelConverter::MaterialToRenderMaterial (const Modeler::Material& material)
{
	return RenderMaterial (material.GetColor ());
}
