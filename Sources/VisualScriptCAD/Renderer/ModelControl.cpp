#include "ModelControl.hpp"
#include "ModelEvaluationData.hpp"
#include "ShaderProgram.hpp"
#include "RenderModelConverter.hpp"
#include "RayTracing.hpp"
#include "WXAS_ControlUtilities.hpp"

static const int canvasAttributes[] = {
	WX_GL_RGBA,
	WX_GL_DOUBLEBUFFER,
	WX_GL_MIN_RED,			8,
	WX_GL_MIN_GREEN,		8,
	WX_GL_MIN_BLUE,			8,
	WX_GL_MIN_ALPHA,		8,
	WX_GL_SAMPLE_BUFFERS,	GL_TRUE,
	WX_GL_SAMPLES,			4,
	WX_GL_DEPTH_SIZE,		16,
	WX_GL_STENCIL_SIZE,		8,
	0
};

SelectionUpdater::~SelectionUpdater ()
{

}

ModelControl::ModelControl (wxWindow *parent, const Modeler::Model& model, SelectionUpdater& selectionUpdater) :
	wxGLCanvas (parent, wxID_ANY, canvasAttributes, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
	model (model),
	selectionUpdater (selectionUpdater),
	glContext (nullptr),
	renderModelConverter (),
	renderSceneSettings (),
	renderScene (),
	mouseDownPosition (0, 0),
	lastMousePosition (0, 0),
	lastMouseButton (-1),
	isMouseDown (false)
{
	if (!InitContext ()) {
		throw std::logic_error ("failed to init context");
	}

	if (!renderScene.Init ()) {
		throw std::logic_error ("failed to init scene");
	}
}

ModelControl::~ModelControl ()
{
	if (glContext != nullptr) {
		delete glContext;
	}
}

void ModelControl::AddMesh (Modeler::MeshId meshId)
{
	renderModelConverter.AddMeshToRenderModel (model, meshId, renderScene.GetModel ());
	Refresh ();
}

void ModelControl::RemoveMesh (Modeler::MeshId meshId)
{
	renderModelConverter.RemoveMeshFromRenderModel (meshId, renderScene.GetModel ());
	Refresh ();
}

void ModelControl::Clear ()
{
	renderModelConverter.Clear ();
	renderScene.Clear ();
	Refresh ();
}

void ModelControl::FitToWindow ()
{
	WXAS::BusyCursorGuard busyCursor;
	wxSize clientSize = GetClientSize ();
	Geometry::BoundingSphere boundingSphere = model.GetBoundingSphere ();
	renderScene.FitToWindow (clientSize.x, clientSize.y, boundingSphere);
	Refresh ();
}

void ModelControl::ResetView ()
{
	renderScene.ResetView ();
	Refresh ();
}

void ModelControl::OnPaint (wxPaintEvent&)
{
	wxPaintDC dc (this);

	wxSize clientSize = GetClientSize ();
	renderScene.Draw (clientSize.x, clientSize.y, renderSceneSettings);

	SwapBuffers ();
}

void ModelControl::OnMouseDown (wxMouseEvent& evt)
{
	if (isMouseDown) {
		return;
	}

	CaptureMouse ();
	mouseDownPosition = evt.GetPosition ();
	lastMousePosition = evt.GetPosition ();
	lastMouseButton = evt.GetButton ();
	isMouseDown = true;
}

void ModelControl::OnMouseMove (wxMouseEvent& evt)
{
	if (isMouseDown) {
		wxPoint diff = evt.GetPosition () - lastMousePosition;
		RenderScene::MouseButton mouseButton = RenderScene::MouseButton::Undefined;
		switch (lastMouseButton) {
			case 1: mouseButton = RenderScene::MouseButton::Left; break;
			case 3: mouseButton = RenderScene::MouseButton::Right; break;
			default: break;
		}
		renderScene.OnMouseMove (mouseButton, diff.x, diff.y);
		lastMousePosition = evt.GetPosition ();
		Refresh ();
	}
}

void ModelControl::OnMouseUp (wxMouseEvent& evt)
{
	if (!isMouseDown) {
		return;
	}

	wxPoint mousePosition = evt.GetPosition ();
	
	bool isMouseClick = (evt.GetButton () == 1 && mousePosition == mouseDownPosition);
	bool isControlPressed = wxGetKeyState (wxKeyCode::WXK_CONTROL);
	if (isMouseClick && isControlPressed) {
		SelectNodeOfMesh (mousePosition);
	}

	ReleaseMouse ();
	isMouseDown = false;
}

void ModelControl::OnMouseWheel (wxMouseEvent& evt)
{
	int rotation = evt.GetWheelRotation ();
	renderScene.OnMouseWheel (rotation);
	Refresh ();
}

const RenderSettings& ModelControl::GetRenderSettings () const
{
	return renderSceneSettings;
}

void ModelControl::SetRenderSettings (const RenderSettings& newSettings)
{
	renderSceneSettings = newSettings;
	renderScene.InitAxisLines (renderSceneSettings.gridSize, renderSceneSettings.gridCount);
	Refresh ();
}

const RenderScene& ModelControl::GetRenderScene () const
{
	return renderScene;
}

bool ModelControl::InitContext ()
{
	if (glContext != nullptr) {
		return false;
	}

	wxGLContextAttrs contextAttrs;
	contextAttrs.PlatformDefaults ().CoreProfile ().OGLVersion (GLVersion.major, GLVersion.minor).EndList ();
	glContext = new wxGLContext (this, NULL, &contextAttrs);
	glContext->SetCurrent (*this);

	return true;
}

void ModelControl::SelectNodeOfMesh (const wxPoint& mousePosition)
{
	wxSize clientSize = GetClientSize ();
	glm::dvec2 screenSize (clientSize.x, clientSize.y);
	glm::dvec2 screenPos (mousePosition.x + 0.5, mousePosition.y + 0.5);
	Geometry::Ray ray = Modeler::GetScreenRay (renderScene.GetCamera (), screenSize, screenPos);
	
	NE::NodeCollection nodesToSelect;
	std::vector<Modeler::RayModelIntersection> intersections = Modeler::GetRayModelRayIntersections (model, ray);
	if (!intersections.empty ()) {
		const Modeler::RayModelIntersection& firstIntersection = intersections.front ();
		const Modeler::MeshRef& meshRef = model.GetMesh (firstIntersection.meshId);
		Modeler::UserDataConstPtr userData = meshRef.GetUserData ("nodeid");
		if (userData == nullptr) {
			throw std::logic_error ("no user data in mesh");
		}
		std::shared_ptr<const NodeIdUserData> nodeIdUserData = std::dynamic_pointer_cast<const NodeIdUserData> (userData);
		nodesToSelect.Insert (nodeIdUserData->GetNodeId ());
	}

	selectionUpdater.UpdateSelection (nodesToSelect);
}

wxBEGIN_EVENT_TABLE (ModelControl, wxGLCanvas)
EVT_PAINT (ModelControl::OnPaint)
EVT_LEFT_DOWN (ModelControl::OnMouseDown)
EVT_RIGHT_DOWN (ModelControl::OnMouseDown)
EVT_MOTION (ModelControl::OnMouseMove)
EVT_LEFT_UP (ModelControl::OnMouseUp)
EVT_RIGHT_UP (ModelControl::OnMouseUp)
EVT_MOUSEWHEEL (ModelControl::OnMouseWheel)
wxEND_EVENT_TABLE()
