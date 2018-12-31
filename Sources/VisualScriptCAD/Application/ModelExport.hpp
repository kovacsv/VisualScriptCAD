#ifndef MODELEXPORT_HPP
#define MODELEXPORT_HPP

#include "Model.hpp"

#include <wx/wx.h>

class ExportDialog : public wxDialog
{
public:
	enum FormatId
	{
		Obj = 0,
		Stl = 1,
		Off = 2
	};

	enum DialogIds
	{
		FormatChoiceId = 1001,
		OutputFolderId = 1002,
		BrowseFolderButtonId = 1003,
		OutputNameId = 1004,
		ExportButtonId = 1100
	};

	ExportDialog (wxWindow *parent, const Modeler::Model& model);

	void	OnButtonClick (wxCommandEvent& evt);

private:
	const Modeler::Model&	model;

	wxBoxSizer*				boxSizer;
	wxChoice*				formatChoice;
	wxTextCtrl*				outputFolderText;
	wxButton*				browseFolderButton;
	wxTextCtrl*				outputNameText;
	wxButton*				exportButton;

	DECLARE_EVENT_TABLE ();
};

#endif
