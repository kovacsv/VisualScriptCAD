#include "Export.hpp"
#include "Checksum.hpp"
#include "TriangleUtils.hpp"

#include <fstream>
#include <functional>

struct PairHash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1,T2>& p) const
	{
		size_t h1 = std::hash<T1>{} (p.first);
		size_t h2 = std::hash<T2>{} (p.second);
		return h1 + 49157 * h2;  
	}
};

namespace Modeler
{

class MaterialMap
{
public:
	MaterialMap ()
	{

	}

	void AddMaterial (const Material& material, MeshId origMeshId, MaterialId origMaterialId)
	{
		Checksum checksum = material.CalcCheckSum ();
		auto foundMaterial = checksumToIndex.find (checksum);
		if (foundMaterial == checksumToIndex.end ()) {
			materials.push_back (material);
			checksumToIndex.insert ({ checksum, materials.size () - 1 });
		}
		materialMap.insert ({ {origMeshId, origMaterialId}, checksumToIndex.at (checksum) });
	}

	size_t GetMaterial (MeshId origMeshId, MaterialId origMaterialId) const
	{
		return materialMap.at ({ origMeshId, origMaterialId });
	}

	void EnumerateMaterials (const std::function<void (MaterialId, const Material&)>& processor) const
	{
		for (MaterialId materialId = 0; materialId < materials.size (); materialId++) {
			processor (materialId, materials[materialId]);
		}
	}

private:
	std::vector<Material>													materials;
	std::unordered_map<Checksum, size_t>									checksumToIndex;
	std::unordered_map<std::pair<MeshId, MaterialId>, size_t, PairHash>		materialMap;
};

static void GetMaterialMap (const Model& model, MaterialMap& materialMap)
{
	model.EnumerateMeshes ([&] (MeshId meshId, const MeshRef& mesh) {
		const MeshMaterials& materials = model.GetMeshMaterials (mesh.GetMaterialsId ());
		materials.EnumerateMaterials ([&] (MaterialId materialId, const Material& material) {
			materialMap.AddMaterial (material, meshId, materialId);
		});
	});
}

void ExportModelToObj (const Model& model, const std::wstring& name, ModelWriter& writer)
{
	MaterialMap materialMap;
	GetMaterialMap (model, materialMap);

	writer.OpenFile (name + L".mtl");
	materialMap.EnumerateMaterials ([&] (MaterialId materialId, const Material& material) {
		writer.WriteLine (L"newmtl Material" + std::to_wstring (materialId));
		const glm::dvec3& color = material.GetColor ();
		writer.WriteLine (L"Kd %g %g %g", color.r, color.g, color.b);
	});
	writer.CloseFile ();

	writer.OpenFile (name + L".obj");
	writer.WriteLine (L"mtllib " + name + L".mtl");
	unsigned int vertexOffset = 1;
	unsigned int normalOffset = 1;
	unsigned int meshIndex = 1;
	model.EnumerateMeshes ([&] (MeshId meshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef.GetGeometryId ());
		const MeshMaterials& materials = model.GetMeshMaterials (meshRef.GetMaterialsId ());
		const glm::dmat4& transformation = meshRef.GetTransformation ();
		writer.WriteLine (L"g Mesh" + std::to_wstring (meshIndex++));
		geometry.EnumerateVertices (transformation, [&] (const glm::dvec3& vertex) {
			writer.WriteLine (L"v %g %g %g", vertex.x, vertex.y, vertex.z);
		});
		geometry.EnumerateNormals (transformation, [&] (const glm::dvec3& normal) {
			writer.WriteLine (L"vn %g %g %g", normal.x, normal.y, normal.z);
		});
		EnumerateTrianglesByMaterial (geometry, materials, [&] (MaterialId materialId, const std::vector<unsigned int>& triangles) {
			writer.WriteLine (L"usemtl Material" + std::to_wstring (materialMap.GetMaterial (meshId, materialId)));
			for (unsigned int triangleId : triangles) {
				const MeshTriangle& triangle = geometry.GetTriangle (triangleId);
				writer.WriteLine (
					L"f %d//%d %d//%d %d//%d",
					vertexOffset + triangle.v1, normalOffset + triangle.n1,
					vertexOffset + triangle.v2, normalOffset + triangle.n2,
					vertexOffset + triangle.v3, normalOffset + triangle.n3
				);
			}
		});
		vertexOffset += geometry.VertexCount ();
		normalOffset += geometry.NormalCount ();
	});	
	writer.CloseFile ();
}

void ExportModelToStl (const Model& model, const std::wstring& name, ModelWriter& writer)
{
	writer.OpenFile (name + L".stl");
	writer.WriteLine (L"solid " + name);
	model.EnumerateMeshes ([&] (MeshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef.GetGeometryId ());
		const glm::dmat4& transformation = meshRef.GetTransformation ();
		geometry.EnumerateTriangles ([&] (const MeshTriangle& triangle) {
			glm::dvec3 v1 = geometry.GetVertex (triangle.v1, transformation);
			glm::dvec3 v2 = geometry.GetVertex (triangle.v2, transformation);
			glm::dvec3 v3 = geometry.GetVertex (triangle.v3, transformation);
			glm::dvec3 n = Geometry::CalculateTriangleNormal (v1, v2, v3);
			writer.WriteLine (L"facet normal %g %g %g", n.x, n.y, n.z);
			writer.WriteLine (L"    outer loop");
			writer.WriteLine (L"        vertex %g %g %g", v1.x, v1.y, v1.z);
			writer.WriteLine (L"        vertex %g %g %g", v2.x, v2.y, v2.z);
			writer.WriteLine (L"        vertex %g %g %g", v3.x, v3.y, v3.z);
			writer.WriteLine (L"    endloop");
			writer.WriteLine (L"endfacet");
		});
	});
	writer.WriteLine (L"endsolid " + name);
	writer.CloseFile ();
}

void ExportModelToOff (const Model& model, const std::wstring& name, ModelWriter& writer)
{
	unsigned int vertexCount = 0;
	unsigned int triangleCount = 0;
	model.EnumerateMeshes ([&] (MeshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef.GetGeometryId ());
		vertexCount += geometry.VertexCount ();
		triangleCount += geometry.TriangleCount ();
	});

	writer.OpenFile (name + L".off");
	writer.WriteLine (L"OFF");
	writer.WriteLine (L"%d %d %d", vertexCount, triangleCount, 0);
	model.EnumerateMeshes ([&] (MeshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef.GetGeometryId ());
		const glm::dmat4& transformation = meshRef.GetTransformation ();
		geometry.EnumerateVertices (transformation, [&] (const glm::dvec3& vertex) {
			writer.WriteLine (L"%g %g %g", vertex.x, vertex.y, vertex.z);
		});
	});	

	unsigned int vertexOffset = 0;
	model.EnumerateMeshes ([&] (MeshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef.GetGeometryId ());
		geometry.EnumerateTriangles ([&] (const MeshTriangle& triangle) {
			writer.WriteLine (
				L"3 %d %d %d",
				vertexOffset + triangle.v1,
				vertexOffset + triangle.v2,
				vertexOffset + triangle.v3
			);
		});
		vertexOffset += geometry.VertexCount ();
	});

	writer.CloseFile ();
}

bool ExportModel (const Model& model, FormatId formatId, const std::wstring& name, ModelWriter& writer)
{
	if (formatId == FormatId::Obj) {
		ExportModelToObj (model, name, writer);
		return true;
	} else if (formatId == FormatId::Stl) {
		ExportModelToStl (model, name, writer);
		return true;
	} else if (formatId == FormatId::Off) {
		ExportModelToOff (model, name, writer);
		return true;
	}
	return false;
}

bool ExportMesh (const Mesh& mesh, FormatId formatId, const std::wstring& name, ModelWriter& writer)
{
	Model model;
	model.AddMesh (mesh);
	return ExportModel (model, formatId, name, writer);
}

}
