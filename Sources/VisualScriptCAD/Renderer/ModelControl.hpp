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

	void							FitToWindow ();

	void							OnPaint (wxPaintEvent& evt);
	void							OnResize (wxSizeEvent& evt);

	void							OnMouseDown (wxMouseEvent& evt);
	void							OnMouseMove (wxMouseEvent& evt);
	void							OnMouseUp (wxMouseEvent& evt);
	void							OnMouseWheel (wxMouseEvent& evt);

	const RenderScene::Settings& 	GetRenderSettings () const;
	void							SetRenderSettings (const RenderScene::Settings& newSettings);

private:
	bool InitContext ();

	wxGLContext*			glContext;
	RenderModelConverter	renderModelConverter;
	RenderScene::Settings	renderSceneSettings;
	RenderScene				renderScene;

	// TODO: separate class
	wxPoint					lastMousePosition;
	int						lastMouseButton;
	bool					isMouseDown;

	wxDECLARE_EVENT_TABLE ();
};

#endif
