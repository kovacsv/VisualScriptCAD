#ifndef MODELCONTROL_HPP
#define MODELCONTROL_HPP

#include "Model.hpp"
#include "RenderScene.hpp"
#include "RenderModelConverter.hpp"
#include "Camera.hpp"

#include <glad/glad.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

class ModelControl : public wxGLCanvas
{
public:
	ModelControl (wxWindow *parent);
	virtual ~ModelControl ();

	void							AddMesh (const Modeler::Model& model, Modeler::MeshId meshId);
	void							RemoveMesh (Modeler::MeshId meshId);
	void							Clear ();

	void							FitToWindow (const Modeler::Model& model);
	void							ResetView ();

	void							OnPaint (wxPaintEvent& evt);
	void							OnResize (wxSizeEvent& evt);

	void							OnMouseDown (wxMouseEvent& evt);
	void							OnMouseMove (wxMouseEvent& evt);
	void							OnMouseUp (wxMouseEvent& evt);
	void							OnMouseWheel (wxMouseEvent& evt);

	const RenderSettings&		 	GetRenderSettings () const;
	void							SetRenderSettings (const RenderSettings& newSettings);

	const RenderScene&				GetRenderScene () const;

private:
	bool InitContext ();

	wxGLContext*			glContext;
	RenderModelConverter	renderModelConverter;
	RenderSettings			renderSceneSettings;
	RenderScene				renderScene;

	wxPoint					lastMousePosition;
	int						lastMouseButton;
	bool					isMouseDown;

	wxDECLARE_EVENT_TABLE ();
};

#endif
