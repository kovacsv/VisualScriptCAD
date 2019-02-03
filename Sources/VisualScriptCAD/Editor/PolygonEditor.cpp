#include "PolygonEditor.hpp"

#include <sstream>
#include <iomanip>

PolygonEditorPanel::StatusUpdater::~StatusUpdater ()
{

}

PolygonEditor::PolygonEditor (const std::vector<glm::dvec2>& polygon) :
	polygon (polygon),
	closed (!polygon.empty ()),
	mouseScreenPosition (0, 0),
	selectedVertex (-1),
	scale (1)
{
}

void PolygonEditor::UpdateScreenSize (const wxSize& newScreenSize)
{
	screenSize = newScreenSize;
}

void PolygonEditor::UpdateMousePosition (const wxPoint& point)
{
	mouseScreenPosition = point;
	selectedVertex = DetectVertexUnderMouse (mouseScreenPosition);
}

void PolygonEditor::UpdateScale (int change)
{
	int newScale = scale + change;
	if (newScale >= 1 && newScale <= 100) {
		scale = newScale;
	}
}

glm::dvec2 PolygonEditor::GetMousePositionAsPolygonPoint () const
{
	if (selectedVertex != -1) {
		return polygon[selectedVertex];
	}
	return MouseCoordToPolygonPoint (mouseScreenPosition);
}

void PolygonEditor::AddVertex (const wxPoint& point)
{
	if (closed) {
		if (selectedVertex == -1) {
			polygon.clear ();
			polygon.push_back (MouseCoordToPolygonPoint (point));
			closed = false;
		}
	} else {
		if (selectedVertex == 0 && polygon.size () > 2) {
			closed = true;
		} else if (selectedVertex == -1) {
			polygon.push_back (MouseCoordToPolygonPoint (point));
		}
	}
	UpdateMousePosition (point);
}

bool PolygonEditor::HasPolygon () const
{
	return closed;
}

const std::vector<glm::dvec2>& PolygonEditor::GetPolygon () const
{
	return polygon;
}

bool PolygonEditor::HasSelectedVertex () const
{
	return selectedVertex != -1;
}

int PolygonEditor::GetSelectedVertex () const
{
	return selectedVertex;
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
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (300, 300)),
	statusUpdater (statusUpdater),
	polygonEditor (polygon),
	memoryBitmap (GetClientSize ()),
	memoryDC (memoryBitmap)
{
	polygonEditor.UpdateScreenSize (GetClientSize ());
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
	polygonEditor.AddVertex (evt.GetPosition ());
	Draw ();
}

void PolygonEditorPanel::OnMouseMove (wxMouseEvent& evt)
{
	polygonEditor.UpdateMousePosition (evt.GetPosition ());
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
	return polygonEditor.HasPolygon ();
}

const std::vector<glm::dvec2>& PolygonEditorPanel::GetPolygon () const
{
	return polygonEditor.GetPolygon ();
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
	const std::vector<glm::dvec2>& polygon = polygonEditor.GetPolygon ();
	if (polygon.empty ()) {
		return;
	}
	std::vector<wxPoint> points;
	for (const glm::dvec2& polygonPoint : polygon) {
		points.push_back (polygonEditor.PolygonPointToMouseCoord (polygonPoint));
	}
	dc.SetPen (wxPen (wxColour (0, 0, 0)));
	if (polygonEditor.HasPolygon ()) {
		dc.SetBrush (wxBrush (wxColour (230, 230, 230)));
		dc.DrawPolygon (points.size (), &points[0]);
	} else {
		points.push_back (polygonEditor.GetMouseScreenPosition ());
		dc.DrawLines (points.size (), &points[0]);
	}
	if (polygonEditor.HasSelectedVertex ()) {
		wxPoint selPoint = points[polygonEditor.GetSelectedVertex ()];
		dc.SetBrush (wxBrush (wxColour (150, 175, 200)));
		dc.DrawCircle (selPoint, 5);
	}
}

void PolygonEditorPanel::UpdateStatus ()
{
	statusUpdater->UpdateStatus (polygonEditor.GetScale (), polygonEditor.GetMousePositionAsPolygonPoint ());
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

	boxSizer->Add (editorPanel, 1, wxEXPAND | wxALL, 0);
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 0);
	boxSizer->Add (statusBar, 0, wxEXPAND | wxALL, 0);
	SetSizerAndFit (boxSizer);

	
	// CenterOnParent ();
}

void PolygonEditorDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == wxID_OK) {
		EndModal (wxID_OK);
	}
}

void PolygonEditorDialog::OnClose (wxCloseEvent&)
{
	EndModal (wxID_CANCEL);
}

bool PolygonEditorDialog::HasPolygon () const
{
	return editorPanel->HasPolygon ();
}

const std::vector<glm::dvec2>& PolygonEditorDialog::GetPolygon () const
{
	return editorPanel->GetPolygon ();
}

BEGIN_EVENT_TABLE (PolygonEditorPanel, wxPanel)
EVT_PAINT (PolygonEditorPanel::OnPaint)
EVT_SIZE (PolygonEditorPanel::OnResize)
EVT_LEFT_DOWN (PolygonEditorPanel::OnLeftClick)
EVT_MOTION (PolygonEditorPanel::OnMouseMove)
EVT_MOUSEWHEEL (PolygonEditorPanel::OnMouseWheel)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PolygonEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, PolygonEditorDialog::OnButtonClick)
EVT_CLOSE (PolygonEditorDialog::OnClose)
END_EVENT_TABLE ()
