#ifndef POLYGONEDITOR_HPP
#define POLYGONEDITOR_HPP

#include "IncludeGLM.hpp"
#include <wx/wx.h>

class PolygonEditorPanel : public wxPanel
{
public:
	class StatusUpdater
	{
	public:
		virtual ~StatusUpdater ();

		virtual void UpdateStatus (const glm::dvec2& position) = 0;
	};

	PolygonEditorPanel (wxWindow* parent, StatusUpdater* statusUpdater);

	void		OnPaint (wxPaintEvent& evt);
	void		OnResize (wxSizeEvent& evt);

	void		OnLeftClick (wxMouseEvent& evt);
	void		OnMouseMove (wxMouseEvent& evt);

private:
	void		Draw ();
	void		DrawCoordSystem (wxDC& dc);
	void		DrawPolygon (wxDC& dc);

	glm::dvec2	MouseCoordToPolygonPoint (const wxPoint& point);
	wxPoint		PolygonPointToMouseCoord (const glm::dvec2& point);

	wxPoint		MouseCoordToCenteredCoord (const wxPoint& point);
	wxPoint		CenteredCoordToMouseCoord (const wxPoint& point);

	StatusUpdater*				statusUpdater;

	std::vector<glm::dvec2>		polygon;
	bool						closed;

	wxBitmap					memoryBitmap;
	wxMemoryDC					memoryDC;
	wxPoint						mousePos;
	double						scale;

	DECLARE_EVENT_TABLE ();
};

class PolygonEditorDialog : public wxDialog
{
public:
	class StatusUpdater : public PolygonEditorPanel::StatusUpdater
	{
	public:
		StatusUpdater (PolygonEditorDialog* dialog);

		virtual void UpdateStatus (const glm::dvec2& position) override;

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
