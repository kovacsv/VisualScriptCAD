#ifndef EXPORTDIALOG_HPP
#define EXPORTDIALOG_HPP

#include "Model.hpp"
#include "RenderScene.hpp"

#include <wx/wx.h>

class ExportDialog : public wxDialog
{
public:
	enum DialogIds
	{
		FormatChoiceId = 1001,
		FormatSettingsButtonId = 1002,
		OutputFolderId = 1003,
		BrowseFolderButtonId = 1004,
		OutputNameId = 1005,
		ExportButtonId = 1100
	};

	ExportDialog (wxWindow* parent, const Modeler::Model& model, const RenderScene& scene, const ExportSettings& exportSettings, const RenderSettings& renderSettings);

	const ExportSettings&	GetExportSettings () const;
	void					OnChoice (wxCommandEvent& evt);
	void					OnButtonClick (wxCommandEvent& evt);

private:
	void					UpdateControls ();

	const Modeler::Model&	model;
	const RenderScene&		scene;
	ExportSettings			exportSettings;
	RenderSettings			renderSettings;

	wxBoxSizer*				boxSizer;
	wxChoice*				formatChoice;
	wxButton*				formatSettingsButton;
	wxTextCtrl*				outputFolderText;
	wxButton*				browseFolderButton;
	wxTextCtrl*				outputNameText;
	wxButton*				exportButton;

	DECLARE_EVENT_TABLE ();
};

#endif
