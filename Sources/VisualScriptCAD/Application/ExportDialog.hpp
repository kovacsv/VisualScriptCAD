#ifndef EXPORTDIALOG_HPP
#define EXPORTDIALOG_HPP

#include "Model.hpp"
#include "UserSettings.hpp"

#include <wx/wx.h>

class ExportDialog : public wxDialog
{
public:
	enum DialogIds
	{
		FormatChoiceId = 1001,
		OutputFolderId = 1002,
		BrowseFolderButtonId = 1003,
		OutputNameId = 1004,
		ExportButtonId = 1100
	};

	ExportDialog (wxWindow *parent, const Modeler::Model& model, const ExportSettings& exportSettings);

	const ExportSettings&	GetExportSettings () const;
	void					OnButtonClick (wxCommandEvent& evt);

private:
	const Modeler::Model&	model;
	ExportSettings			exportSettings;

	wxBoxSizer*				boxSizer;
	wxChoice*				formatChoice;
	wxTextCtrl*				outputFolderText;
	wxButton*				browseFolderButton;
	wxTextCtrl*				outputNameText;
	wxButton*				exportButton;

	DECLARE_EVENT_TABLE ();
};

#endif
