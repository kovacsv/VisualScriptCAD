#include "PolygonEditor.hpp"

#include <sstream>
#include <iomanip>

PolygonEditorPanel::StatusUpdater::~StatusUpdater ()
{

}

PolygonEditorPanel::PolygonEditorPanel (wxWindow* parent, StatusUpdater* statusUpdater) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (300, 300)),
	statusUpdater (statusUpdater),
	polygon (),
	closed (false),
	memoryBitmap (GetClientSize ()),
	memoryDC (memoryBitmap),
	mousePos (0, 0),
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
		polygon.clear ();
		closed = false;
	}
	polygon.push_back (MouseCoordToPolygonPoint (evt.GetPosition ()));
	Draw ();
}

void PolygonEditorPanel::OnMouseMove (wxMouseEvent& evt)
{
	mousePos = evt.GetPosition ();
	statusUpdater->UpdateStatus (MouseCoordToPolygonPoint (mousePos));
	Draw ();
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

PolygonEditorDialog::PolygonEditorDialog (wxWindow* parent) :
	wxDialog (parent, wxID_ANY, L"Edit Polygon", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	statusUpdater (this),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	editorPanel (new PolygonEditorPanel (this, &statusUpdater)),
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

BEGIN_EVENT_TABLE (PolygonEditorPanel, wxPanel)
EVT_PAINT (PolygonEditorPanel::OnPaint)
EVT_SIZE (PolygonEditorPanel::OnResize)
EVT_LEFT_UP (PolygonEditorPanel::OnLeftClick)
EVT_MOTION (PolygonEditorPanel::OnMouseMove)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PolygonEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, PolygonEditorDialog::OnButtonClick)
END_EVENT_TABLE ()
