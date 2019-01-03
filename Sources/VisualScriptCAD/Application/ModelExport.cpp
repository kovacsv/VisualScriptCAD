#include "ModelExport.hpp"
#include "Export.hpp"

#include <wx/filepicker.h>
#include <wx/stdpaths.h>

static const wxSize nameMinSize (100, -1);
static const wxSize controlMinSize (300, -1);

static ExportDialog::FormatId	formatValue = ExportDialog::FormatId::Obj;
static std::string				folderValue = "";
static std::string				nameValue = "";

ExportDialog::ExportDialog (wxWindow *parent, const Modeler::Model& model) :
	wxDialog (parent, wxID_ANY, L"Export Model", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	model (model),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	formatChoice (new wxChoice (this, DialogIds::FormatChoiceId, wxDefaultPosition, controlMinSize)),
	outputFolderText (new wxTextCtrl (this, DialogIds::OutputFolderId, wxEmptyString, wxDefaultPosition, controlMinSize)),
	browseFolderButton (new wxButton (this, DialogIds::BrowseFolderButtonId, L"...", wxDefaultPosition, wxSize (40, -1))),
	outputNameText (new wxTextCtrl (this, DialogIds::OutputNameId, wxEmptyString, wxDefaultPosition, controlMinSize)),
	exportButton (new wxButton (this, DialogIds::ExportButtonId, L"Export"))
{
	if (folderValue.empty ()) {
		folderValue = wxStandardPaths::Get ().GetUserDir (wxStandardPathsBase::Dir_Desktop);
	}
	if (nameValue.empty ()) {
		nameValue = "model";
	}
	outputFolderText->SetValue (folderValue);
	outputNameText->SetValue (nameValue);

	{
		formatChoice->Append (L"obj");
		formatChoice->Append (L"stl");
		formatChoice->Append (L"off");
		formatChoice->Select (formatValue);

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

void ExportDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == DialogIds::ExportButtonId) {
		formatValue = (FormatId) formatChoice->GetSelection ();
		folderValue = outputFolderText->GetValue ();
		nameValue = outputNameText->GetValue ();
		if (folderValue.empty () || nameValue.empty ()) {
			return;
		}
		if (formatValue == FormatId::Obj) {
			Modeler::ExportModel (model, Modeler::FormatId::Obj, folderValue, nameValue);
		} else if (formatValue == FormatId::Stl) {
			Modeler::ExportModel (model, Modeler::FormatId::Stl, folderValue, nameValue);
		} else if (formatValue == FormatId::Off) {
			Modeler::ExportModel (model, Modeler::FormatId::Off, folderValue, nameValue);
		} else {
			throw std::logic_error ("invalid export file format");
		}
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
