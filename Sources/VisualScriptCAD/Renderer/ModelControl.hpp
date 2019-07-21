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
	ModelControl (wxWindow *parent, const Modeler::Model& model);
	virtual ~ModelControl ();

	void							AddMesh (Modeler::MeshId meshId);
	void							RemoveMesh (Modeler::MeshId meshId);
	void							Clear ();

	void							FitToWindow ();
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
	bool							InitContext ();
	void							SelectNodeOfMesh (const wxPoint& mousePosition);

	const Modeler::Model&	model;

	wxGLContext*			glContext;
	RenderModelConverter	renderModelConverter;
	RenderSettings			renderSceneSettings;
	RenderScene				renderScene;

	wxPoint					mouseDownPosition;
	wxPoint					lastMousePosition;
	int						lastMouseButton;
	bool					isMouseDown;

	wxDECLARE_EVENT_TABLE ();
};

#endif
