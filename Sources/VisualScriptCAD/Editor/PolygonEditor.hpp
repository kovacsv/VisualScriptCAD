#ifndef POLYGONEDITOR_HPP
#define POLYGONEDITOR_HPP

#include <wx/wx.h>

class PolygonEditorPanel : public wxPanel
{
public:
	class StatusUpdater
	{
	public:
		virtual ~StatusUpdater ();

		virtual void UpdateStatus (double currX, double currY) = 0;
	};

	PolygonEditorPanel (wxWindow* parent, StatusUpdater* statusUpdater);

	void		OnPaint (wxPaintEvent& evt);
	void		OnResize (wxSizeEvent& evt);

	void		OnMouseMove (wxMouseEvent& evt);

private:
	void		DrawCoordSystem (wxPaintDC& dc);
	void		DrawPolygon (wxPaintDC& dc);

	std::vector<wxPoint>	polygon;
	StatusUpdater*			statusUpdater;

	DECLARE_EVENT_TABLE ();
};

class PolygonEditorDialog : public wxDialog
{
public:
	class StatusUpdater : public PolygonEditorPanel::StatusUpdater
	{
	public:
		StatusUpdater (PolygonEditorDialog* dialog);

		virtual void UpdateStatus (double currX, double currY) override;

	private:
		PolygonEditorDialog* dialog;
	};

	PolygonEditorDialog (wxWindow *parent);

	void					OnButtonClick (wxCommandEvent& evt);

private:
	StatusUpdater			statusUpdater;
	wxBoxSizer*				boxSizer;
	PolygonEditorPanel*		editorPanel;
	wxButton*				okButton;
	wxStatusBar*			statusBar;

	DECLARE_EVENT_TABLE ();
};

#endif
