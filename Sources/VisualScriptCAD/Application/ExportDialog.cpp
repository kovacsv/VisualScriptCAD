#include "ExportDialog.hpp"
#include "Export.hpp"

#include <wx/filepicker.h>

static const wxSize nameMinSize (100, -1);
static const wxSize controlMinSize (300, -1);

class wxFileWriter : public Modeler::ModelWriter
{
public:
	wxFileWriter (const std::wstring& folder) :
		folder (folder),
		file (nullptr)
	{

	}

	virtual void OpenFile (const std::wstring& fileName) override
	{
		wxFileName filePath (folder, fileName);
		file = new wxFile (filePath.GetFullPath (), wxFile::OpenMode::write);
		if (!file->IsOpened ()) {
			return;
		}
	}

	virtual void CloseFile () override
	{
		file->Close ();
		delete file;
	}

	virtual void WriteLine (const std::wstring& text) override
	{
		file->Write (wxString (text + L"\n"));
	}

private:
	std::wstring	folder;
	wxFile*			file;
};

ExportDialog::ExportDialog (wxWindow *parent, const Modeler::Model& model, const ExportSettings& exportSettings) :
	wxDialog (parent, wxID_ANY, L"Export Model", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	model (model),
	exportSettings (exportSettings),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	formatChoice (new wxChoice (this, DialogIds::FormatChoiceId, wxDefaultPosition, controlMinSize)),
	outputFolderText (new wxTextCtrl (this, DialogIds::OutputFolderId, wxEmptyString, wxDefaultPosition, controlMinSize)),
	browseFolderButton (new wxButton (this, DialogIds::BrowseFolderButtonId, L"...", wxDefaultPosition, wxSize (40, -1))),
	outputNameText (new wxTextCtrl (this, DialogIds::OutputNameId, wxEmptyString, wxDefaultPosition, controlMinSize)),
	exportButton (new wxButton (this, DialogIds::ExportButtonId, L"Export"))
{
	outputFolderText->SetValue (exportSettings.folder);
	outputNameText->SetValue (exportSettings.name);

	{
		formatChoice->Append (L"obj");
		formatChoice->Append (L"stl");
		formatChoice->Append (L"off");
		formatChoice->Select ((int) exportSettings.format);

		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Format", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		horizontalSizer->Add (formatChoice, 1, wxEXPAND | wxALL, 5);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	{
		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Folder", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		horizontalSizer->Add (outputFolderText, 1, wxEXPAND | wxALL, 5);
		horizontalSizer->Add (browseFolderButton, 0, wxALL, 5);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	{
		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Name", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		horizontalSizer->Add (outputNameText, 1, wxEXPAND | wxALL, 5);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	boxSizer->Add (exportButton, 1, wxEXPAND | wxALL, 5);
	SetSizerAndFit (boxSizer);
}

const ExportSettings& ExportDialog::GetExportSettings () const
{
	return exportSettings;
}

void ExportDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == DialogIds::ExportButtonId) {
		exportSettings.format = (Modeler::FormatId) formatChoice->GetSelection ();
		exportSettings.folder = outputFolderText->GetValue ();
		exportSettings.name = outputNameText->GetValue ();
		if (exportSettings.folder.empty () || exportSettings.name.empty ()) {
			return;
		}
		wxFileWriter writer (exportSettings.folder);
		Modeler::ExportModel (model, exportSettings.format, exportSettings.name, writer);
		EndModal (DialogIds::ExportButtonId);
	} else if (evt.GetId () == DialogIds::BrowseFolderButtonId) {
		wxDirDialog dirDialog (this);
		if (dirDialog.ShowModal () == wxID_OK) {
			outputFolderText->SetValue (dirDialog.GetPath ());
		}
	}
}

BEGIN_EVENT_TABLE (ExportDialog, wxDialog)
EVT_BUTTON (wxID_ANY, ExportDialog::OnButtonClick)
END_EVENT_TABLE ()