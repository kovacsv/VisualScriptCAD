#include "PolygonEditor.hpp"

#include <sstream>
#include <iomanip>

PolygonEditorPanel::StatusUpdater::~StatusUpdater ()
{

}

PolygonEditorPanel::PolygonEditorPanel (wxWindow* parent, StatusUpdater* statusUpdater) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (300, 300)),
	polygon (),
	statusUpdater (statusUpdater)
{
	SetBackgroundColour (wxColour (240, 240, 240));
}

void PolygonEditorPanel::OnPaint (wxPaintEvent& /*evt*/)
{
	wxPaintDC dc (this);
	DrawCoordSystem (dc);
	DrawPolygon (dc);
}

void PolygonEditorPanel::OnResize (wxSizeEvent&)
{
	Update ();
	Refresh ();
}

void PolygonEditorPanel::OnMouseMove (wxMouseEvent& evt)
{
	statusUpdater->UpdateStatus (evt.GetX (), evt.GetY ());
}

void PolygonEditorPanel::DrawCoordSystem (wxPaintDC& dc)
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

void PolygonEditorPanel::DrawPolygon (wxPaintDC& dc)
{
	std::vector<wxPoint> points;
	points.push_back (wxPoint (10, 10));
	points.push_back (wxPoint (100, 100));
	points.push_back (wxPoint (100, 200));
	dc.SetPen (wxPen (wxColour (0, 0, 0)));
	dc.SetBrush (wxBrush (wxColour (230, 230, 230)));
	dc.DrawPolygon (points.size (), &points[0]);
}

PolygonEditorDialog::StatusUpdater::StatusUpdater (PolygonEditorDialog* dialog) :
	dialog (dialog)
{

}

void PolygonEditorDialog::StatusUpdater::UpdateStatus (double currX, double currY)
{
	std::wostringstream stream;
	stream << std::fixed << std::setprecision (2) << currX << L", " << currY;
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
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 5);
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
EVT_MOTION (PolygonEditorPanel::OnMouseMove)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PolygonEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, PolygonEditorDialog::OnButtonClick)
END_EVENT_TABLE ()
