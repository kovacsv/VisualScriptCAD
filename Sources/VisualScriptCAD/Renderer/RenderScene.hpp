#ifndef RENDERSCENE_HPP
#define RENDERSCENE_HPP

#include "Camera.hpp"
#include "UserSettings.hpp"

#include <glad/glad.h>
#include <vector>
#include <functional>
#include <unordered_map>

typedef int RenderMeshId;
typedef int RenderMeshInstanceId;

class RenderLineMaterial
{
public:
	RenderLineMaterial (const glm::vec3& color);

	glm::vec3 color;
};

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

class RenderLineGeometry
{
public:
	RenderLineGeometry (const RenderLineMaterial& lineMaterial);
	~RenderLineGeometry ();

	void						AddLine (const glm::vec3& v1, const glm::vec3& v2);

	const RenderLineMaterial&	GetMaterial () const;

	void						SetupBuffers () const;
	void						DrawBuffers () const;

private:
	void						AddVertex (const glm::vec3& v);

	std::vector<float>			lineVertices;
	RenderLineMaterial			lineMaterial;

	mutable unsigned int		vertexArrayObject;
	mutable unsigned int		vertexBufferObject;
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

class RenderLineModel
{
public:
	RenderLineModel ();

	void				AddRenderLineGeometry (const RenderLineGeometry& lineGeometry);
	void				EnumerateRenderLineGeometries (const std::function<void (const RenderLineGeometry&)>& processor) const;

	void				Clear ();

private:
	std::vector<RenderLineGeometry>		renderLineGeometries;
};

class RenderPixels
{
public:
	RenderPixels (int width, int height);
	RenderPixels (const RenderPixels& rhs) = delete;
	RenderPixels& operator= (const RenderPixels& rhs) const = delete;
	~RenderPixels ();

	int				GetWidth () const;
	int				GetHeight () const;
	unsigned char*	GetPixels () const;
	void			GetPixel (int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const;

private:
	int				width;
	int				height;
	unsigned char*	pixels;
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

	RenderScene ();

	bool					Init ();
	void					InitAxisLines (double gridSize, int gridCount);

	void					Draw (int width, int height, const RenderSettings& settings) const;
	void					DrawOffscreen (const RenderSettings& settings, RenderPixels& pixels) const;
	const Modeler::Camera&	GetCamera () const;
	RenderModel&			GetModel ();

	void					OnMouseMove (MouseButton mouseButton, int diffX, int diffY);
	void					OnMouseWheel (int rotation);

	void					FitToWindow (int width, int height, const Geometry::BoundingSphere& boundingSphere);
	void					ResetView ();

	void					Clear ();

private:
	void					DrawModel (int width, int height, ViewMode drawMode) const;
	void					DrawLines (int width, int height, AxisMode axisMode) const;

	int						lineShader;
	int						triangleShader;
	RenderModel				model;
	RenderLineModel			axisModel;
	RenderLight				light;
	Modeler::Camera			camera;
};

#endif
