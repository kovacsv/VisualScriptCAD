#include "RenderScene.hpp"
#include "ShaderProgram.hpp"
#include "IncludeGLM.hpp"
#include "BoundingShapes.hpp"
#include "Geometry.hpp"

static const char* lineVertexShaderSource = R"(
#version 330 core
precision mediump float;

layout (location = 0) in vec3 aPos;

uniform mediump mat4 viewMatrix;
uniform mediump mat4 projectionMatrix;
uniform mediump mat4 modelMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (aPos, 1.0f);
}
)";

static const char* lineFragmentShaderSource = R"(
#version 330 core
precision mediump float;

uniform mediump vec3 materialColor;
out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4 (materialColor, 1.0f);
}
)";

static const char* triangleVertexShaderSource = R"(
#version 330 core
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mediump mat4 viewMatrix;
uniform mediump mat4 projectionMatrix;
uniform mediump mat4 modelMatrix;
uniform mediump mat3 normalMatrix;

varying mediump vec3 fragmentNormal;
varying mediump vec3 fragmentPosition;

void main()
{
	fragmentNormal = normalize (normalMatrix * aNormal);
	fragmentPosition = vec3 (modelMatrix * vec4 (aPos, 1.0f));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (aPos, 1.0f);
}
)";

static const char* triangleFragmentShaderSource = R"(
#version 330 core
precision mediump float;

uniform mediump vec3 viewPosition;
uniform mediump vec3 materialColor;

uniform mediump vec3 lightPosition;
uniform mediump vec3 lightColor;
uniform mediump float lightAmbientStrength;
uniform mediump float lightSpecularStrength;

varying mediump vec3 fragmentNormal;
varying mediump vec3 fragmentPosition;

out vec4 fragmentColor;

mediump float materialShininess = 32.0f;

void main()
{
	mediump vec3 ambient = lightAmbientStrength * lightColor;
  	
    mediump vec3 norm = normalize (fragmentNormal);
    mediump vec3 lightDirection = normalize (lightPosition - fragmentPosition);
    mediump float diffuseValue = max (dot (norm, lightDirection), 0.0f);
    mediump vec3 diffuse = diffuseValue * lightColor;
    
    mediump vec3 viewDirection = normalize (viewPosition - fragmentPosition);
    mediump vec3 reflectedDirection = reflect (-lightDirection, norm);  
    mediump float specularValue = pow (max (dot (viewDirection, reflectedDirection), 0.0f), materialShininess);
    mediump vec3 specular = lightSpecularStrength * specularValue * lightColor;  

	mediump vec3 result = (ambient + diffuse + specular) * materialColor;
	fragmentColor = vec4 (result, 1.0f);
}
)";

static const Modeler::Camera DefaultCamera (glm::dvec3 (-2.0, -3.0, 1.5), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, 10000.0);

RenderLineMaterial::RenderLineMaterial (const glm::vec3& color) :
	color (color)
{

}

RenderMaterial::RenderMaterial (const glm::vec3& color) :
	color (color)
{

}

RenderLight::RenderLight () :
	lightColor (0.7f, 0.7f, 0.7f),
	lightAmbientStrength (0.6f),
	lightSpecularStrength (0.1f)
{

}

RenderLineGeometry::RenderLineGeometry (const RenderLineMaterial& lineMaterial) :
	lineMaterial (lineMaterial),
	vertexArrayObject ((unsigned int) -1),
	vertexBufferObject ((unsigned int) -1)
{

}

RenderLineGeometry::~RenderLineGeometry ()
{
	if (vertexArrayObject != -1) {
		glDeleteVertexArrays (1, &vertexArrayObject);
		glDeleteBuffers (1, &vertexBufferObject);
	}
}

void RenderLineGeometry::AddLine (const glm::vec3& v1, const glm::vec3& v2)
{
	AddVertex (v1);
	AddVertex (v2);
}

const RenderLineMaterial& RenderLineGeometry::GetMaterial () const
{
	return lineMaterial;
}

void RenderLineGeometry::AddVertex (const glm::vec3& v)
{
	lineVertices.push_back (v.x);
	lineVertices.push_back (v.y);
	lineVertices.push_back (v.z);
}

void RenderLineGeometry::SetupBuffers () const
{
	if (vertexArrayObject != -1) {
		return;
	}

	if (lineVertices.empty () || lineVertices.empty ()) {
		return;
	}

	glGenVertexArrays (1, &vertexArrayObject);
	glGenBuffers (1, &vertexBufferObject);

	glBindVertexArray (vertexArrayObject);

	const float* lineVertexArray = &lineVertices[0];
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData (GL_ARRAY_BUFFER, lineVertices.size () * sizeof (float), lineVertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*) 0);
	glEnableVertexAttribArray (0);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}

void RenderLineGeometry::DrawBuffers () const
{
	if (vertexArrayObject == -1) {
		return;
	}

	unsigned int vertexCount = (unsigned int) lineVertices.size () / 3;
	glBindVertexArray (vertexArrayObject);
	glDrawArrays (GL_LINES, 0, vertexCount);
}

RenderGeometry::RenderGeometry (const RenderMaterial& triangleMaterial) :
	triangleMaterial (triangleMaterial),
	vertexArrayObject ((unsigned int) -1),
	vertexBufferObject ((unsigned int) -1),
	normalBufferObject ((unsigned int) -1)
{

}

RenderGeometry::~RenderGeometry ()
{
	if (vertexArrayObject != -1) {
		glDeleteVertexArrays (1, &vertexArrayObject);
		glDeleteBuffers (1, &vertexBufferObject);
		glDeleteBuffers (1, &normalBufferObject);
	}
}

void RenderGeometry::AddTriangle (	const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
									const glm::vec3& n1, const glm::vec3& n2, const glm::vec3& n3)
{
	AddVertex (v1);
	AddVertex (v2);
	AddVertex (v3);
	AddNormal (n1);
	AddNormal (n2);
	AddNormal (n3);
}

void RenderGeometry::AddVertex (const glm::vec3& v)
{
	triangleVertices.push_back (v.x);
	triangleVertices.push_back (v.y);
	triangleVertices.push_back (v.z);
}

void RenderGeometry::AddNormal (const glm::vec3& n)
{
	triangleNormals.push_back (n.x);
	triangleNormals.push_back (n.y);
	triangleNormals.push_back (n.z);
}

const RenderMaterial& RenderGeometry::GetMaterial () const
{
	return triangleMaterial;
}

void RenderGeometry::SetupBuffers () const
{
	if (vertexArrayObject != -1) {
		return;
	}

	if (triangleVertices.empty () || triangleNormals.empty ()) {
		return;
	}

	glGenVertexArrays (1, &vertexArrayObject);
	glGenBuffers (1, &vertexBufferObject);
	glGenBuffers (1, &normalBufferObject);

	glBindVertexArray (vertexArrayObject);

	const float* triangleVertexArray = &triangleVertices[0];
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData (GL_ARRAY_BUFFER, triangleVertices.size () * sizeof (float), triangleVertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*) 0);
	glEnableVertexAttribArray (0);

	const float* triangleNormalArray = &triangleNormals[0];
	glBindBuffer (GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData (GL_ARRAY_BUFFER, triangleNormals.size () * sizeof (float), triangleNormalArray, GL_STATIC_DRAW);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*) 0);
	glEnableVertexAttribArray (1);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}

void RenderGeometry::DrawBuffers () const
{
	if (vertexArrayObject == -1) {
		return;
	}

	unsigned int vertexCount = (unsigned int) triangleVertices.size () / 3;
	glBindVertexArray (vertexArrayObject);
	glDrawArrays (GL_TRIANGLES, 0, vertexCount);
}

void RenderGeometry::EnumerateVertices (const std::function<void (const glm::vec3&)>& processor) const
{
	for (size_t i = 0; i < triangleVertices.size (); i += 3) {
		processor (glm::vec3 (triangleVertices[i], triangleVertices[i + 1], triangleVertices[i + 2]));
	}
}

RenderMeshInstance::RenderMeshInstance (const glm::mat4& transformation) :
	transformation (transformation)
{
}

const glm::mat4& RenderMeshInstance::GetTransformation () const
{
	return transformation;
}

RenderMesh::RenderMesh ()
{

}

void RenderMesh::AddRenderGeometry (const RenderGeometry& geometry)
{
	geometries.push_back (geometry);
}

size_t RenderMesh::RenderGeometryCount () const
{
	return geometries.size ();
}

RenderGeometry& RenderMesh::GetRenderGeometry (size_t index)
{
	return geometries[index];
}

bool RenderMesh::ContainsInstance (RenderMeshInstanceId instanceId) const
{
	return instances.find (instanceId) != instances.end ();
}

size_t RenderMesh::InstanceCount () const
{
	return instances.size ();
}

void RenderMesh::AddInstance (RenderMeshInstanceId instanceId, const RenderMeshInstance& instance)
{
	instances.insert ({ instanceId, instance });
}

void RenderMesh::RemoveInstance (RenderMeshInstanceId instanceId)
{
	instances.erase (instanceId);
}

void RenderMesh::EnumerateRenderGeometries (const std::function<void (const RenderGeometry&)>& processor) const
{
	for (const RenderGeometry& mesh : geometries) {
		processor (mesh);
	}
}

void RenderMesh::EnumerateInstances (const std::function<void (const RenderMeshInstance&)>& processor) const
{
	for (const auto& it : instances) {
		processor (it.second);
	}
}

RenderModel::RenderModel ()
{

}

void RenderModel::AddRenderMesh (RenderMeshId meshId, const RenderMesh& renderMesh)
{
	renderMeshes.insert ({ meshId, renderMesh });
}

void RenderModel::RemoveRenderMesh (RenderMeshId meshId)
{
	renderMeshes.erase (meshId);
}

void RenderModel::AddRenderMeshInstance (RenderMeshId meshId, RenderMeshInstanceId instanceId, const RenderMeshInstance& instance)
{
	RenderMesh& renderMesh = renderMeshes.at (meshId);
	renderMesh.AddInstance (instanceId, instance);
	instanceIdToMeshId.insert ({ instanceId, meshId });
}

void RenderModel::RemoveRenderMeshInstance (RenderMeshInstanceId instanceId)
{
	RenderMeshId renderMeshId = instanceIdToMeshId.at (instanceId);
	RenderMesh& renderMesh = renderMeshes.at (renderMeshId);
	renderMesh.RemoveInstance (instanceId);
	instanceIdToMeshId.erase (instanceId);
	if (renderMesh.InstanceCount () == 0) {
		RemoveRenderMesh (renderMeshId);
	}
}

RenderMeshId RenderModel::GetRenderMeshId (RenderMeshInstanceId instanceId) const
{
	return instanceIdToMeshId.at (instanceId);
}

const RenderMesh& RenderModel::GetRenderMesh (RenderMeshId meshId) const
{
	return renderMeshes.at (meshId);
}

bool RenderModel::ContainsRenderMesh (RenderMeshId meshId) const
{
	return renderMeshes.find (meshId) != renderMeshes.end ();
}

void RenderModel::EnumerateRenderMeshes (const std::function<void (const RenderMesh&)>& processor) const
{
	for (const auto& it : renderMeshes) {
		processor (it.second);
	}
}

void RenderModel::Clear ()
{
	renderMeshes.clear ();
	instanceIdToMeshId.clear ();
}

RenderLineModel::RenderLineModel ()
{

}

void RenderLineModel::AddRenderLineGeometry (const RenderLineGeometry& lineGeometry)
{
	renderLineGeometries.push_back (lineGeometry);
}

void RenderLineModel::EnumerateRenderLineGeometries (const std::function<void (const RenderLineGeometry&)>& processor) const
{
	for (const RenderLineGeometry& lineGeometry : renderLineGeometries) {
		processor (lineGeometry);
	}
}

void RenderLineModel::Clear ()
{
	renderLineGeometries.clear ();
}

RenderPixels::RenderPixels (int width, int height) :
	width (width),
	height (height),
	pixels (new unsigned char[width * height * 4])
{
}

RenderPixels::~RenderPixels ()
{
	delete[] pixels;
}

int RenderPixels::GetWidth () const
{
	return width;
}

int RenderPixels::GetHeight () const
{
	return height;
}

unsigned char* RenderPixels::GetPixels () const
{
	return pixels;
}

void RenderPixels::GetPixel (int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const
{
	int firstIndex = width * y * 4 + x * 4;
	r = pixels[firstIndex + 0];
	g = pixels[firstIndex + 1];
	b = pixels[firstIndex + 2];
	a = pixels[firstIndex + 3];
}

RenderScene::RenderScene () :
	lineShader (-1),
	triangleShader (-1),
	model (),
	axisModel (),
	light (),
	camera (DefaultCamera)
{

}

bool RenderScene::Init ()
{
	if (!gladLoadGL ()) {
		return false;
	}

	lineShader = CreateShaderProgram (lineVertexShaderSource, lineFragmentShaderSource);
	if (lineShader < 0) {
		return false;
	}

	triangleShader = CreateShaderProgram (triangleVertexShaderSource, triangleFragmentShaderSource);
	if (triangleShader < 0) {
		return false;
	}

	InitAxisLines (0.0, 0);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

void RenderScene::InitAxisLines (double gridSize, int gridCount)
{
	axisModel.Clear ();
	if (gridCount == 0 || !Geometry::IsGreater (gridSize, 0.0)) {
		return;
	}

	double axisSize = gridSize * gridCount;

	RenderLineGeometry mainAxisLines (RenderLineMaterial (glm::vec3 (0.5f, 0.5f, 0.5f)));
	mainAxisLines.AddLine (glm::vec3 (-axisSize, 0.0f, 0.0f), glm::vec3 (axisSize, 0.0f, 0.0f));
	mainAxisLines.AddLine (glm::vec3 (0.0f, -axisSize, 0.0f), glm::vec3 (0.0f, axisSize, 0.0f));
	mainAxisLines.AddLine (glm::vec3 (0.0f, 0.0f, -axisSize), glm::vec3 (0.0f, 0.0f, axisSize));
	axisModel.AddRenderLineGeometry (mainAxisLines);

	RenderLineGeometry secondaryAxisLines (RenderLineMaterial (glm::vec3 (0.8f, 0.8f, 0.8f)));
	for (int i = -gridCount; i <= gridCount; i++) {
		if (i == 0) {
			continue;
		}
		secondaryAxisLines.AddLine (glm::vec3 (-axisSize, i * gridSize, 0.0f), glm::vec3 (axisSize, i * gridSize, 0.0f));
		secondaryAxisLines.AddLine (glm::vec3 (i * gridSize, -axisSize, 0.0f), glm::vec3 (i * gridSize, axisSize, 0.0f));
	}
	axisModel.AddRenderLineGeometry (secondaryAxisLines);
}

void RenderScene::Draw (int width, int height, const RenderSettings& settings) const
{
	glViewport (0, 0, width, height);

	glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawModel (width, height, settings.viewMode);
	DrawLines (width, height, settings.axisMode);
}

void RenderScene::DrawOffscreen (const RenderSettings& settings, RenderPixels& pixels) const
{
	int width = pixels.GetWidth ();
	int height = pixels.GetHeight ();

	GLuint frameBuffer = 0;
	glGenFramebuffers (1, &frameBuffer);
	glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer);

	GLuint targetTexture;
	glGenTextures (1, &targetTexture);
	glBindTexture (GL_TEXTURE_2D, targetTexture);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint depthRenderBuffer;
	glGenRenderbuffers (1, &depthRenderBuffer);
	glBindRenderbuffer (GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture, 0);

	Draw (width, height, settings);

	glReadBuffer (GL_COLOR_ATTACHMENT0);
	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.GetPixels ());

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

const Modeler::Camera& RenderScene::GetCamera () const
{
	return camera;
}

void RenderScene::SetCamera (const Modeler::Camera& newCamera)
{
	camera = newCamera;
}

RenderModel& RenderScene::GetModel ()
{
	return model;
}

void RenderScene::OnMouseMove (MouseButton mouseButton, int diffX, int diffY)
{
	if (mouseButton == MouseButton::Left) {
		camera.Orbit ((float) -diffX, (float) -diffY);
	} else if (mouseButton == MouseButton::Right) {
		camera.Pan ((float) -diffX, (float) diffY);
	}
}

void RenderScene::OnMouseWheel (int rotation)
{
	float ratio = (rotation > 0 ? 0.1f : -0.1f);
	camera.Zoom (ratio);
}

void RenderScene::FitToWindow (int width, int height, const Geometry::BoundingSphere& boundingSphere)
{
	if (!boundingSphere.IsValid ()) {
		return;
	}
	camera.ZoomToSphere (boundingSphere.GetCenter (), boundingSphere.GetRadius (), width, height);
}

void RenderScene::ResetView ()
{
	camera = DefaultCamera;
}

void RenderScene::Clear ()
{
	model.Clear ();
	camera = DefaultCamera;
}

void RenderScene::DrawModel (int width, int height, ViewMode drawMode) const
{
	glUseProgram (triangleShader);

	if (drawMode == ViewMode::Lines) {
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	} else if (drawMode == ViewMode::Polygons) {
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	} else {
		throw std::logic_error ("invalid view mode");
	}

	glm::vec3 lightPosition = camera.GetEye ();
	glUniform3fv (glGetUniformLocation (triangleShader, "lightPosition"), 1, &lightPosition[0]);
	glUniform3fv (glGetUniformLocation (triangleShader, "lightColor"), 1, &light.lightColor[0]);
	glUniform1f (glGetUniformLocation (triangleShader, "lightAmbientStrength"), light.lightAmbientStrength);
	glUniform1f (glGetUniformLocation (triangleShader, "lightSpecularStrength"), light.lightSpecularStrength);

	glm::vec3 viewPosition = camera.GetEye ();
	glm::mat4 viewMatrix = camera.GetViewMatrix ();
	glm::mat4 projectionMatrix = camera.GetProjectionMatrix (width, height);
	glUniform3fv (glGetUniformLocation (triangleShader, "viewPosition"), 1, &viewPosition[0]);
	glUniformMatrix4fv (glGetUniformLocation (triangleShader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr (viewMatrix));
	glUniformMatrix4fv (glGetUniformLocation (triangleShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr (projectionMatrix));

	GLint materialColorLocation = glGetUniformLocation (triangleShader, "materialColor");
	GLint modelMatrixLocation = glGetUniformLocation (triangleShader, "modelMatrix");
	GLint normalMatrixLocation = glGetUniformLocation (triangleShader, "normalMatrix");
	model.EnumerateRenderMeshes ([&] (const RenderMesh& renderMesh) {
		renderMesh.EnumerateRenderGeometries ([&] (const RenderGeometry& renderGeometry) {
			const RenderMaterial& material = renderGeometry.GetMaterial ();
			glUniform3fv (materialColorLocation, 1, &material.color[0]);
			renderGeometry.SetupBuffers ();
			renderMesh.EnumerateInstances ([&] (const RenderMeshInstance& meshInstance) {
				glm::mat3 normalMatrix = glm::transpose (glm::inverse (glm::mat3 (meshInstance.GetTransformation ())));
				glUniformMatrix4fv (modelMatrixLocation, 1, GL_FALSE, glm::value_ptr (meshInstance.GetTransformation ()));
				glUniformMatrix3fv (normalMatrixLocation, 1, GL_FALSE, glm::value_ptr (normalMatrix));
				renderGeometry.DrawBuffers ();
			});
		});
	});
}

void RenderScene::DrawLines (int width, int height, AxisMode axisMode) const
{
	if (axisMode == AxisMode::On) {
		glUseProgram (lineShader);

		glm::mat4 viewMatrix = camera.GetViewMatrix ();
		glm::mat4 projectionMatrix = camera.GetProjectionMatrix (width, height);

		glUniformMatrix4fv (glGetUniformLocation (lineShader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr (viewMatrix));
		glUniformMatrix4fv (glGetUniformLocation (lineShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr (projectionMatrix));
		glUniformMatrix4fv (glGetUniformLocation (lineShader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr (glm::mat4 (1.0f)));

		axisModel.EnumerateRenderLineGeometries ([&] (const RenderLineGeometry& lineGeometry) {
			const RenderLineMaterial& material = lineGeometry.GetMaterial ();
			glUniform3fv (glGetUniformLocation (lineShader, "materialColor"), 1, &material.color[0]);
			lineGeometry.SetupBuffers ();
			lineGeometry.DrawBuffers ();
		});
	}
}
