#ifndef RENDERSCENE_HPP
#define RENDERSCENE_HPP

#include "Camera.hpp"

#include <glad/glad.h>
#include <vector>
#include <functional>
#include <unordered_map>

typedef int RenderMeshId;
typedef int RenderMeshInstanceId;

class RenderMaterial
{
public:
	RenderMaterial (const glm::vec3& color);

	glm::vec3 color;
};

class RenderLight
{
public:
	RenderLight ();

	glm::vec3	lightColor;
	float		lightAmbientStrength;
	float		lightSpecularStrength;
};

class RenderGeometry
{
public:
	RenderGeometry (const RenderMaterial& triangleMaterial);
	~RenderGeometry ();

	void AddTriangle (	const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
						const glm::vec3& n1, const glm::vec3& n2, const glm::vec3& n3);

	const RenderMaterial&		GetMaterial () const;

	void						SetupBuffers () const;
	void						DrawBuffers () const;

	void						EnumerateVertices (const std::function<void (const glm::vec3&)>& processor) const;

private:
	void						AddVertex (const glm::vec3& v);
	void						AddNormal (const glm::vec3& n);

	std::vector<float>			triangleVertices;
	std::vector<float>			triangleNormals;
	RenderMaterial				triangleMaterial;

	mutable unsigned int		vertexArrayObject;
	mutable unsigned int		vertexBufferObject;
	mutable unsigned int		normalBufferObject;
};

class RenderMeshInstance
{
public:
	RenderMeshInstance (const glm::mat4& transformation);

	const glm::mat4& GetTransformation () const;

private:
	glm::mat4 transformation;
};

class RenderMesh
{
public:
	RenderMesh ();

	void				AddRenderGeometry (const RenderGeometry& geometry);
	size_t				RenderGeometryCount () const;
	RenderGeometry&		GetRenderGeometry (size_t index);

	bool				ContainsInstance (RenderMeshInstanceId instanceId) const;
	size_t				InstanceCount () const;
	void				AddInstance (RenderMeshInstanceId instanceId, const RenderMeshInstance& instance);
	void				RemoveInstance (RenderMeshInstanceId instanceId);

	void				EnumerateRenderGeometries (const std::function<void (const RenderGeometry&)>& processor) const;
	void				EnumerateInstances (const std::function<void (const RenderMeshInstance&)>& processor) const;

private:
	std::vector<RenderGeometry>										geometries;
	std::unordered_map<RenderMeshInstanceId, RenderMeshInstance>	instances;
};

class RenderModel
{
public:
	RenderModel ();

	void				AddRenderMesh (RenderMeshId meshId, const RenderMesh& renderMesh);
	void				RemoveRenderMesh (RenderMeshId meshId);

	void				AddRenderMeshInstance (RenderMeshId meshId, RenderMeshInstanceId instanceId, const RenderMeshInstance& instance);
	void				RemoveRenderMeshInstance (RenderMeshInstanceId instanceId);

	RenderMeshId		GetRenderMeshId (RenderMeshInstanceId instanceId) const;
	const RenderMesh&	GetRenderMesh (RenderMeshId meshId) const;
	bool				ContainsRenderMesh (RenderMeshId meshId) const;
	void				EnumerateRenderMeshes (const std::function<void (const RenderMesh&)>& processor) const;

	void				Clear ();

private:
	std::unordered_map<RenderMeshId, RenderMesh>			renderMeshes;
	std::unordered_map<RenderMeshInstanceId, RenderMeshId>	instanceIdToMeshId;
};

class RenderScene
{
public:
	enum class MouseButton
	{
		Undefined,
		Left,
		Right
	};

	enum class ViewMode
	{
		Lines,
		Polygons
	};

	RenderScene ();

	bool			Init ();
	void			Draw (int width, int height, ViewMode drawMode);
	RenderModel&	GetModel ();

	void			OnMouseMove (MouseButton mouseButton, int diffX, int diffY);
	void			OnMouseWheel (int rotation);

	void			FitToWindow (int width, int height);

private:
	int				shader;
	RenderModel		model;
	RenderLight		light;
	Camera			camera;
};

#endif