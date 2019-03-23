#include "PolygonEditor.hpp"
#include "TriangleUtils.hpp"

#include <sstream>
#include <iomanip>
#include <limits>

static void SetTextValidator (wxTextCtrl* textCtrl, const std::wstring& validChars)
{
	wxTextValidator validator (wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString includeList;

	for (const wchar_t& character : validChars) {
		includeList.Add (character);
	}

	validator.SetIncludes (includeList);
	textCtrl->SetValidator (validator);
}

class PointPositionDialog : public wxDialog
{
public:
	PointPositionDialog (wxWindow* parent, const glm::dvec2& position) :
		wxDialog (parent, wxID_ANY, L"Set Point Position"),
		position (position),
		gridSizer (new wxGridSizer (2, 5, 5)),
		boxSizer (new wxBoxSizer (wxVERTICAL)),
		okButton (new wxButton (this, wxID_OK, L"OK")),
		xPositionControl (new wxTextCtrl (this, wxID_ANY, std::to_wstring (position.x))),
		yPositionControl (new wxTextCtrl (this, wxID_ANY, std::to_wstring (position.y)))
	{
		SetTextValidator (xPositionControl, L"0123456789.-");
		SetTextValidator (yPositionControl, L"0123456789.-");

		gridSizer->SetRows (2);
		gridSizer->Add (new wxStaticText (this, wxID_ANY, L"Position X"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (xPositionControl, 1, wxEXPAND);
		gridSizer->Add (new wxStaticText (this, wxID_ANY, L"Position Y"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (yPositionControl, 1, wxEXPAND);

		boxSizer->Add (gridSizer, 1, wxEXPAND | wxALL, 5);
		boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 5);

		SetSizerAndFit (boxSizer);
		CenterOnParent ();
	}

	void OnOkButtonClick (wxCommandEvent&)
	{
		if (xPositionControl->IsEmpty () || yPositionControl->IsEmpty ()) {
			return;
		}
		position.x = std::stod (xPositionControl->GetValue ().ToStdWstring ());
		position.y = std::stod (yPositionControl->GetValue ().ToStdWstring ());
		EndModal (wxID_OK);
	}

	const glm::dvec2& GetPoint ()
	{
		return position;
	}

private:
	glm::dvec2		position;
		
	wxGridSizer*	gridSizer;
	wxBoxSizer*		boxSizer;
	wxButton*		okButton;
	wxTextCtrl*		xPositionControl;
	wxTextCtrl*		yPositionControl;

	DECLARE_EVENT_TABLE ();
};

BEGIN_EVENT_TABLE (PointPositionDialog, wxDialog)
EVT_BUTTON (wxID_OK, PointPositionDialog::OnOkButtonClick)
END_EVENT_TABLE ()

PolygonEditorPanel::StatusUpdater::~StatusUpdater ()
{

}

PolygonEditor::State::State (const std::vector<glm::dvec2>& polygon) :
	polygon (polygon),
	isClosed (!polygon.empty ()),
	isMoving (false),
	selectedVertex (-1)
{
}

bool PolygonEditor::State::HasPolygon () const
{
	return isClosed && !isMoving;
}

const std::vector<glm::dvec2>& PolygonEditor::State::GetPolygon () const
{
	return polygon;
}

bool PolygonEditor::State::IsClosed () const
{
	return isClosed;
}

bool PolygonEditor::State::IsVertexMoving () const
{
	return isMoving;
}

void PolygonEditor::State::StartNewPolygon (const glm::dvec2& firstVertex)
{
	polygon.clear ();
	AddNewVertex (firstVertex);
}

void PolygonEditor::State::AddNewVertex (const glm::dvec2& newVertex)
{
	polygon.push_back (newVertex);
	isClosed = false;
	isMoving = false;
	selectedVertex = -1;
}

bool PolygonEditor::State::SelectedVertexCanClose () const
{
	return VertexCanClose (selectedVertex);
}

bool PolygonEditor::State::VertexCanClose (int vertex) const
{
	return vertex == 0 && polygon.size () > 2;
}

void PolygonEditor::State::StartMoveSelectedVertex ()
{
	isMoving = true;
}

void PolygonEditor::State::StopMoveSelectedVertex ()
{
	isMoving = false;
}

void PolygonEditor::State::ClosePolygon ()
{
	isClosed = true;
}

void PolygonEditor::State::SelectVertex (int newSelectedVertex)
{
	selectedVertex = newSelectedVertex;
}

bool PolygonEditor::State::HasSelectedVertex () const
{
	return selectedVertex != -1;
}

int PolygonEditor::State::GetSelectedVertex () const
{
	return selectedVertex;
}

const glm::dvec2& PolygonEditor::State::GetSelectedVertexPosition () const
{
	return polygon[selectedVertex];
}

void PolygonEditor::State::SetSelectedVertexPosition (const glm::dvec2& position)
{
	polygon[selectedVertex] = position;
}

PolygonEditor::PolygonEditor (const std::vector<glm::dvec2>& polygon) :
	state (polygon),
	mouseScreenPosition (0, 0),
	scale (1)
{
}

void PolygonEditor::UpdateScreenSize (const wxSize& newScreenSize)
{
	screenSize = newScreenSize;
}

void PolygonEditor::UpdateScale (int change)
{
	int newScale = scale + change;
	if (newScale >= 1 && newScale <= 100) {
		scale = newScale;
	}
}

void PolygonEditor::AutoScale ()
{
	if (!state.HasPolygon ()) {
		return;
	}

	static const double INF = std::numeric_limits<double>::max ();
	double maxX = -INF;
	double maxY = -INF;
	const std::vector<glm::dvec2>& polygon = state.GetPolygon ();
	for (const glm::dvec2& point : polygon) {
		maxX = std::max (maxX, std::fabs (point.x));
		maxY = std::max (maxY, std::fabs (point.y));
	}

	double dScale = scale / 100.0;
	double neededSize = std::max (
		(maxX * 2.0) / dScale,
		(maxY * 2.0) / dScale
	);

	int newScale = std::ceil (neededSize / std::min (screenSize.x, screenSize.y));
	if (newScale < 1) {
		newScale = 1;
	} else if (newScale > 100) {
		newScale = 100;
	}
	scale = newScale;
}

glm::dvec2 PolygonEditor::GetMousePositionAsPolygonPoint () const
{
	if (state.HasSelectedVertex ()) {
		return state.GetSelectedVertexPosition ();
	}
	return MouseCoordToPolygonPoint (mouseScreenPosition);
}

void PolygonEditor::HandleMouseClick (const wxPoint& point)
{
	UpdateMousePosition (point);
	if (state.IsClosed ()) {
		if (state.IsVertexMoving ()) {
			state.StopMoveSelectedVertex ();
		} else {
			if (state.HasSelectedVertex ()) {
				state.StartMoveSelectedVertex ();
			} else {
				state.StartNewPolygon (MouseCoordToPolygonPoint (point));
			}
		}
	} else {
		if (state.SelectedVertexCanClose ()) {
			state.ClosePolygon ();
		} else if (!state.HasSelectedVertex ()) {
			state.AddNewVertex (MouseCoordToPolygonPoint (point));
		}
	}
}

void PolygonEditor::HandleMouseMove (const wxPoint& point)
{
	UpdateMousePosition (point);
}

const PolygonEditor::State& PolygonEditor::GetState () const
{
	return state;
}

bool PolygonEditor::HasPolygon () const
{
	return state.HasPolygon ();
}

const std::vector<glm::dvec2>& PolygonEditor::GetPolygon () const
{
	return state.GetPolygon ();
}

glm::dvec2 PolygonEditor::GetSelectedVertexPosition () const
{
	return state.GetSelectedVertexPosition ();
}

void PolygonEditor::SetSelectedVertexPosition (const glm::dvec2& position)
{
	state.SetSelectedVertexPosition (position);
}

int PolygonEditor::GetScale () const
{
	return scale;
}

const wxPoint& PolygonEditor::GetMouseScreenPosition () const
{
	return mouseScreenPosition;
}

glm::dvec2 PolygonEditor::MouseCoordToPolygonPoint (const wxPoint& point) const
{
	double dScale = scale / 100.0;
	wxPoint centeredPoint = MouseCoordToCenteredCoord (point);
	return glm::dvec2 (
		centeredPoint.x * dScale,
		centeredPoint.y * dScale
	);
}

wxPoint PolygonEditor::PolygonPointToMouseCoord (const glm::dvec2& point) const
{
	double dScale = scale / 100.0;
	wxPoint centeredPoint (
		point.x / dScale,
		point.y / dScale
	);
	return CenteredCoordToMouseCoord (centeredPoint);
}

void PolygonEditor::UpdateMousePosition (const wxPoint& point)
{
	mouseScreenPosition = point;
	int vertexUnderMouse = DetectVertexUnderMouse (mouseScreenPosition);
	if (state.IsVertexMoving ()) {
		state.SetSelectedVertexPosition (MouseCoordToPolygonPoint (point));
	} else if (state.IsClosed () || state.VertexCanClose (vertexUnderMouse)) {
		state.SelectVertex (vertexUnderMouse);
	}

}

wxPoint PolygonEditor::MouseCoordToCenteredCoord (const wxPoint& point) const
{
	int width = screenSize.GetWidth () - 2;
	int height = screenSize.GetHeight () - 2;
	return wxPoint (
		(point.x - width / 2),
		-(point.y - height / 2)
	);
}

wxPoint PolygonEditor::CenteredCoordToMouseCoord (const wxPoint& point) const
{
	int width = screenSize.GetWidth () - 2;
	int height = screenSize.GetHeight () - 2;
	return wxPoint (
		(point.x + width / 2),
		height - (point.y + height / 2)
	);
}

int PolygonEditor::DetectVertexUnderMouse (const wxPoint& point) const
{
	const std::vector<glm::dvec2>& polygon = state.GetPolygon ();
	for (int i = 0; i < (int) polygon.size (); i++) {
		const glm::dvec2& polygonPoint = polygon[i];
		wxPoint polygonMousePoint = PolygonPointToMouseCoord (polygonPoint);
		if (abs (polygonMousePoint.x - point.x) < 10 && abs (polygonMousePoint.y - point.y) < 10) {
			return i;
		}
	}
	return -1;
}

PolygonEditorPanel::PolygonEditorPanel (wxWindow* parent, const std::vector<glm::dvec2>& polygon, StatusUpdater* statusUpdater) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (500, 300)),
	statusUpdater (statusUpdater),
	polygonEditor (polygon),
	memoryBitmap (GetClientSize ()),
	memoryDC (memoryBitmap)
{
	polygonEditor.UpdateScreenSize (GetClientSize ());
	polygonEditor.AutoScale ();
}

void PolygonEditorPanel::OnPaint (wxPaintEvent& /*evt*/)
{
	wxRect clientRect = GetClientRect ();
	wxPaintDC dc (this);
	dc.Blit (0, 0, clientRect.GetWidth (), clientRect.GetHeight (), &memoryDC, 0, 0);
}

void PolygonEditorPanel::OnResize (wxSizeEvent&)
{
	polygonEditor.UpdateScreenSize (GetClientSize ());
	memoryBitmap = wxBitmap (GetClientSize ());
	memoryDC.SelectObject (memoryBitmap);
	Draw ();
}

void PolygonEditorPanel::OnLeftClick (wxMouseEvent& evt)
{
	polygonEditor.HandleMouseClick (evt.GetPosition ());
	Draw ();
}

void PolygonEditorPanel::OnRightClick (wxMouseEvent& evt)
{
	const PolygonEditor::State& state = polygonEditor.GetState ();
	if (state.HasPolygon () && state.HasSelectedVertex ()) {
		wxMenu popupMenu;
		popupMenu.Append (1, L"Set Point Position");
		int selectedItem = GetPopupMenuSelectionFromUser (popupMenu, evt.GetPosition ());
		if (selectedItem == 1) {
			PointPositionDialog pointPositionDialog (this, state.GetSelectedVertexPosition ());
			if (pointPositionDialog.ShowModal () == wxID_OK) {
				polygonEditor.SetSelectedVertexPosition (pointPositionDialog.GetPoint ());
			}
		}
	}
	Draw ();
}

void PolygonEditorPanel::OnMouseMove (wxMouseEvent& evt)
{
	polygonEditor.HandleMouseMove (evt.GetPosition ());
	UpdateStatus ();
	Draw ();
}

void PolygonEditorPanel::OnMouseWheel (wxMouseEvent& evt)
{
	int scaleChange = evt.GetWheelRotation () > 0 ? -1 : 1;
	polygonEditor.UpdateScale (scaleChange);
	UpdateStatus ();
	Draw ();
}

bool PolygonEditorPanel::HasPolygon () const
{
	return polygonEditor.GetState ().HasPolygon ();
}

std::vector<glm::dvec2> PolygonEditorPanel::GetPolygon () const
{
	std::vector<glm::dvec2> polygon = polygonEditor.GetState ().GetPolygon ();
	Geometry::Orientation orientation = Geometry::GetPolygonOrientation2D (polygon);
	if (orientation != Geometry::Orientation::CounterClockwise) {
		std::reverse (polygon.begin (), polygon.end ());
	}
	return polygon;
}

void PolygonEditorPanel::UpdateStatus ()
{
	statusUpdater->UpdateStatus (polygonEditor.GetScale (), polygonEditor.GetMousePositionAsPolygonPoint ());
}

void PolygonEditorPanel::Draw ()
{
	memoryDC.SetPen (*wxTRANSPARENT_PEN);
	memoryDC.SetBrush (*wxWHITE_BRUSH);
	memoryDC.DrawRectangle (GetClientRect ());

	DrawCoordSystem (memoryDC);
	DrawPolygon (memoryDC);
	Refresh (false);
}

void PolygonEditorPanel::DrawCoordSystem (wxDC& dc)
{
	wxRect clientRect = GetClientRect ();
	dc.SetPen (wxPen (wxColour (200, 200, 200)));
	dc.DrawLine (
		(clientRect.GetTopLeft () + clientRect.GetTopRight ()) / 2,
		(clientRect.GetBottomLeft () + clientRect.GetBottomRight ()) / 2
	);
	dc.DrawLine (
		(clientRect.GetTopLeft () + clientRect.GetBottomLeft ()) / 2,
		(clientRect.GetTopRight () + clientRect.GetBottomRight ()) / 2
	);	
}

void PolygonEditorPanel::DrawPolygon (wxDC& dc)
{
	const PolygonEditor::State& state = polygonEditor.GetState ();
	const std::vector<glm::dvec2>& polygon = state.GetPolygon ();
	if (polygon.empty ()) {
		return;
	}
	std::vector<wxPoint> points;
	for (const glm::dvec2& polygonPoint : polygon) {
		points.push_back (polygonEditor.PolygonPointToMouseCoord (polygonPoint));
	}
	dc.SetPen (wxPen (wxColour (0, 0, 0)));
	if (state.IsClosed ()) {
		dc.SetBrush (wxBrush (wxColour (230, 230, 230)));
		dc.DrawPolygon (points.size (), &points[0]);
	} else {
		points.push_back (polygonEditor.GetMouseScreenPosition ());
		dc.DrawLines (points.size (), &points[0]);
	}
	if (state.HasSelectedVertex ()) {
		wxPoint selPoint = points[state.GetSelectedVertex ()];
		wxColor highlightColor (150, 175, 200);
		if (state.IsVertexMoving ()) {
			highlightColor = wxColor (150, 220, 175);
		}
		dc.SetBrush (wxBrush (highlightColor));
		dc.DrawCircle (selPoint, 5);
	}
}

PolygonEditorDialog::StatusUpdater::StatusUpdater (PolygonEditorDialog* dialog) :
	dialog (dialog)
{

}

void PolygonEditorDialog::StatusUpdater::UpdateStatus (int scale, const glm::dvec2& position)
{
	{
		std::wostringstream stream;
		stream << scale << "%";
		dialog->statusBar->SetStatusText (stream.str (), 0);
	}
	{
		std::wostringstream stream;
		stream << std::fixed << std::setprecision (2) << position.x << L", " << position.y;
		dialog->statusBar->SetStatusText (stream.str (), 1);
	}
}

PolygonEditorDialog::PolygonEditorDialog (wxWindow* parent, const std::vector<glm::dvec2>& polygon) :
	wxDialog (parent, wxID_ANY, L"Edit Polygon", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	statusUpdater (this),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	editorPanel (new PolygonEditorPanel (this, polygon, &statusUpdater)),
	okButton (new wxButton (this, wxID_OK, L"OK")),
	statusBar (new wxStatusBar (this))
{
	statusBar->SetFieldsCount (2);
	editorPanel->UpdateStatus ();

	boxSizer->Add (editorPanel, 1, wxEXPAND | wxALL, 0);
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 0);
	boxSizer->Add (statusBar, 0, wxEXPAND | wxALL, 0);
	SetSizerAndFit (boxSizer);

	SetEscapeId (wxID_CANCEL);
	// CenterOnParent ();
}

void PolygonEditorDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == wxID_OK) {
		if (editorPanel->HasPolygon ()) {
			EndModal (wxID_OK);
		}
	}
}

bool PolygonEditorDialog::HasPolygon () const
{
	return editorPanel->HasPolygon ();
}

std::vector<glm::dvec2> PolygonEditorDialog::GetPolygon () const
{
	return editorPanel->GetPolygon ();
}

BEGIN_EVENT_TABLE (PolygonEditorPanel, wxPanel)
EVT_PAINT (PolygonEditorPanel::OnPaint)
EVT_SIZE (PolygonEditorPanel::OnResize)
EVT_LEFT_DOWN (PolygonEditorPanel::OnLeftClick)
EVT_RIGHT_DOWN (PolygonEditorPanel::OnRightClick)
EVT_MOTION (PolygonEditorPanel::OnMouseMove)
EVT_MOUSEWHEEL (PolygonEditorPanel::OnMouseWheel)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PolygonEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, PolygonEditorDialog::OnButtonClick)
END_EVENT_TABLE ()
