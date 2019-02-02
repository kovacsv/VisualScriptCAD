#include "PolygonEditor.hpp"

#include <sstream>
#include <iomanip>

PolygonEditorPanel::StatusUpdater::~StatusUpdater ()
{

}

PolygonEditorPanel::PolygonEditorPanel (wxWindow* parent, const std::vector<glm::dvec2>& polygon, StatusUpdater* statusUpdater) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (300, 300)),
	statusUpdater (statusUpdater),
	polygon (polygon),
	closed (!polygon.empty ()),
	memoryBitmap (GetClientSize ()),
	memoryDC (memoryBitmap),
	mousePos (0, 0),
	selVertex (-1),
	scale (0.01)
{

}

void PolygonEditorPanel::OnPaint (wxPaintEvent& /*evt*/)
{
	wxRect clientRect = GetClientRect ();
	wxPaintDC dc (this);
	dc.Blit (0, 0, clientRect.GetWidth (), clientRect.GetHeight (), &memoryDC, 0, 0);
}

void PolygonEditorPanel::OnResize (wxSizeEvent&)
{
	memoryBitmap = wxBitmap (GetClientSize ());
	memoryDC.SelectObject (memoryBitmap);
	Draw ();
}

void PolygonEditorPanel::OnLeftClick (wxMouseEvent& evt)
{
	if (closed) {
		if (selVertex == -1) {
			polygon.clear ();
			polygon.push_back (MouseCoordToPolygonPoint (evt.GetPosition ()));
			closed = false;
		}
	} else {
		if (selVertex == 0 && polygon.size () > 2) {
			closed = true;
		} else if (selVertex == -1) {
			polygon.push_back (MouseCoordToPolygonPoint (evt.GetPosition ()));
		}
	}
	DetectVertexUnderMouse (evt.GetPosition ());
	Draw ();
}

void PolygonEditorPanel::OnMouseMove (wxMouseEvent& evt)
{
	mousePos = evt.GetPosition ();
	DetectVertexUnderMouse (evt.GetPosition ());
	statusUpdater->UpdateStatus (MouseCoordToPolygonPoint (evt.GetPosition ()));
	Draw ();
}

bool PolygonEditorPanel::HasPolygon () const
{
	return closed;
}

const std::vector<glm::dvec2>& PolygonEditorPanel::GetPolygon () const
{
	return polygon;
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
	if (polygon.empty ()) {
		return;
	}
	std::vector<wxPoint> points;
	for (const glm::dvec2& polygonPoint : polygon) {
		points.push_back (PolygonPointToMouseCoord (polygonPoint));
	}
	dc.SetPen (wxPen (wxColour (0, 0, 0)));
	if (closed) {
		dc.SetBrush (wxBrush (wxColour (230, 230, 230)));
		dc.DrawPolygon (points.size (), &points[0]);
	} else {
		points.push_back (mousePos);
		dc.DrawLines (points.size (), &points[0]);
	}
	if (selVertex != -1) {
		wxPoint selPoint = points[selVertex];
		dc.SetBrush (wxBrush (wxColour (150, 175, 200)));
		dc.DrawCircle (selPoint, 5);
	}
}

glm::dvec2 PolygonEditorPanel::MouseCoordToPolygonPoint (const wxPoint& point)
{
	wxPoint centeredPoint = MouseCoordToCenteredCoord (point);
	return glm::dvec2 (
		centeredPoint.x * scale,
		centeredPoint.y * scale
	);
}

wxPoint PolygonEditorPanel::PolygonPointToMouseCoord (const glm::dvec2& point)
{
	wxPoint centeredPoint (
		point.x / scale,
		point.y / scale
	);
	return CenteredCoordToMouseCoord (centeredPoint);
}

wxPoint PolygonEditorPanel::MouseCoordToCenteredCoord (const wxPoint& point)
{
	wxRect clientRect = GetClientRect ();
	int width = clientRect.GetWidth () - 2;
	int height = clientRect.GetHeight () - 2;
	return wxPoint (
		(point.x - width / 2),
		-(point.y - height / 2)
	);
}

wxPoint PolygonEditorPanel::CenteredCoordToMouseCoord (const wxPoint& point)
{
	wxRect clientRect = GetClientRect ();
	int width = clientRect.GetWidth () - 2;
	int height = clientRect.GetHeight () - 2;
	return wxPoint (
		(point.x + width / 2),
		height - (point.y + height / 2)
	);
}

void PolygonEditorPanel::DetectVertexUnderMouse (const wxPoint& point)
{
	selVertex = -1;
	for (int i = 0; i < (int) polygon.size (); i++) {
		const glm::dvec2& polygonPoint = polygon[i];
		wxPoint polygonMousePoint = PolygonPointToMouseCoord (polygonPoint);
		if (abs (polygonMousePoint.x - point.x) < 10 && abs (polygonMousePoint.y - point.y) < 10) {
			selVertex = i;
		}
	}
}

PolygonEditorDialog::StatusUpdater::StatusUpdater (PolygonEditorDialog* dialog) :
	dialog (dialog)
{

}

void PolygonEditorDialog::StatusUpdater::UpdateStatus (const glm::dvec2& position)
{
	std::wostringstream stream;
	stream << std::fixed << std::setprecision (2) << position.x << L", " << position.y;
	dialog->statusBar->SetStatusText (stream.str ());
}

PolygonEditorDialog::PolygonEditorDialog (wxWindow* parent, const std::vector<glm::dvec2>& polygon) :
	wxDialog (parent, wxID_ANY, L"Edit Polygon", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	statusUpdater (this),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	editorPanel (new PolygonEditorPanel (this, polygon, &statusUpdater)),
	okButton (new wxButton (this, wxID_OK, L"OK")),
	statusBar (new wxStatusBar (this))
{
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
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PolygonEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, PolygonEditorDialog::OnButtonClick)
EVT_CLOSE (PolygonEditorDialog::OnClose)
END_EVENT_TABLE ()
