#ifndef RENDERMODELCONVERTER_HPP
#define RENDERMODELCONVERTER_HPP

#include "Model.hpp"
#include "RenderScene.hpp"

class MeshUniqueId
{
public:
	MeshUniqueId (Modeler::MeshGeometryId geometryId, Modeler::MeshMaterialsId materialsId);

	size_t	GenerateHashValue () const;
	bool	operator== (const MeshUniqueId& rhs) const;

private:
	Modeler::MeshGeometryId		geometryId;
	Modeler::MeshMaterialsId	materialsId;
};

namespace std
{
	template <>
	struct hash<MeshUniqueId>
	{
		size_t operator() (const MeshUniqueId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

class RenderModelConverter
{
public:
	RenderModelConverter ();

	void			AddMeshToRenderModel (const Modeler::Model& model, Modeler::MeshId meshId, RenderModel& renderModel);
	void			RemoveMeshFromRenderModel (Modeler::MeshId meshId, RenderModel& renderModel);

	void			Clear ();

private:
	void			ConvertMeshRefToRenderMesh (const Modeler::Model& model, const Modeler::MeshRef& meshRef, RenderMesh& renderMesh);
	RenderMaterial	MaterialToRenderMaterial (const Modeler::Material& material);

	std::unordered_map<MeshUniqueId, RenderMeshId>	meshIdToRenderMeshId;
	std::unordered_map<RenderMeshId, MeshUniqueId>	renderMeshIdToMeshId;
	RenderMeshId									nextRenderMeshId;
};

#endif
