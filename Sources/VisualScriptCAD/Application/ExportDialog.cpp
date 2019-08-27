#include "ExportDialog.hpp"
#include "Export.hpp"
#include "WXAS_ControlUtilities.hpp"

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

class ImageSettingsDialog : public wxDialog
{
public:
	enum DialogIds
	{
		WidthId = 1001,
		HeightId = 1002,
		MultisamplingId = 1003,
		SaveButtonId = 1100
	};

	ImageSettingsDialog (wxWindow* parent, const ImageSettings& imageSettings) :
		wxDialog (parent, wxID_ANY, L"Image Settings", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
		imageSettings (imageSettings),
		boxSizer (new wxBoxSizer (wxVERTICAL)),
		widthText (new wxTextCtrl (this, DialogIds::WidthId, wxEmptyString, wxDefaultPosition, controlMinSize)),
		heightText (new wxTextCtrl (this, DialogIds::HeightId, wxEmptyString, wxDefaultPosition, controlMinSize)),
		multisamplingSlider (new wxSlider (this, DialogIds::HeightId, 1, 1, 8, wxDefaultPosition, controlMinSize, wxSL_LABELS)),
		saveButton (new wxButton (this, DialogIds::SaveButtonId, L"Save"))
	{
		WXAS::SetTextControlValidator (widthText, L"0123456789");
		WXAS::SetTextControlValidator (heightText, L"0123456789");

		widthText->SetValue (std::to_wstring (imageSettings.width));
		heightText->SetValue (std::to_wstring (imageSettings.height));
		multisamplingSlider->SetValue (imageSettings.multisampling);

		{
			wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
			horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Width", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
			horizontalSizer->Add (widthText, 1, wxEXPAND | wxALL, 5);
			boxSizer->Add (horizontalSizer, 0, wxEXPAND);
		}

		{
			wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
			horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Height", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
			horizontalSizer->Add (heightText, 1, wxEXPAND | wxALL, 5);
			boxSizer->Add (horizontalSizer, 0, wxEXPAND);
		}

		{
			wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
			horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Multisampling", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
			horizontalSizer->Add (multisamplingSlider, 1, wxEXPAND | wxALL, 5);
			boxSizer->Add (horizontalSizer, 0, wxEXPAND);
		}

		boxSizer->Add (saveButton, 1, wxEXPAND | wxALL, 5);
		SetSizerAndFit (boxSizer);
	}

	const ImageSettings& GetImageSettings () const
	{
		return imageSettings;
	}

	void OnButtonClick (wxCommandEvent& evt)
	{
		if (evt.GetId () == DialogIds::SaveButtonId) {
			imageSettings.width = std::stoi (widthText->GetValue ().ToStdWstring ());
			imageSettings.height = std::stoi (heightText->GetValue ().ToStdWstring ());
			imageSettings.multisampling = multisamplingSlider->GetValue ();
			
			if (imageSettings.IsValid ()) {
				EndModal (wxID_OK);
			} else {
				wxMessageDialog messageDialog (this, L"Invalid image settings.", L"Error!", wxICON_ERROR | wxOK);
				messageDialog.ShowModal ();
			}
		}
	}

private:
	ImageSettings			imageSettings;

	wxBoxSizer*				boxSizer;
	wxTextCtrl*				widthText;
	wxTextCtrl*				heightText;
	wxSlider*				multisamplingSlider;
	wxButton*				saveButton;

	DECLARE_EVENT_TABLE ();
};

BEGIN_EVENT_TABLE (ImageSettingsDialog, wxDialog)
EVT_BUTTON (wxID_ANY, ImageSettingsDialog::OnButtonClick)
END_EVENT_TABLE ()

ExportDialog::ExportDialog (wxWindow* parent, const Modeler::Model& model, const RenderScene& scene, const ExportSettings& exportSettings) :
	wxDialog (parent, wxID_ANY, L"Export Model", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	model (model),
	scene (scene),
	exportSettings (exportSettings),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	exportButton (new wxButton (this, DialogIds::ExportButtonId, L"Export")),
	formatChoice (new wxChoice (this, DialogIds::FormatChoiceId, wxDefaultPosition, controlMinSize)),
	formatSettingsButton (new wxButton (this, DialogIds::FormatSettingsButtonId, L"...", wxDefaultPosition, wxSize (40, -1))),
	outputFolderText (new wxTextCtrl (this, DialogIds::OutputFolderId, wxEmptyString, wxDefaultPosition, controlMinSize)),
	browseFolderButton (new wxButton (this, DialogIds::BrowseFolderButtonId, L"...", wxDefaultPosition, wxSize (40, -1))),
	outputNameText (new wxTextCtrl (this, DialogIds::OutputNameId, wxEmptyString, wxDefaultPosition, controlMinSize))
{
	outputFolderText->SetValue (exportSettings.folder);
	outputNameText->SetValue (exportSettings.name);

	{
		formatChoice->Append (L"obj");
		formatChoice->Append (L"stl");
		formatChoice->Append (L"off");
		formatChoice->Append (L"png");
		formatChoice->Select ((int) exportSettings.format);

		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Format", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		horizontalSizer->Add (formatChoice, 1, wxEXPAND | wxALL, 5);
		horizontalSizer->Add (formatSettingsButton, 0, wxALL, 5);
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

	UpdateControls ();
}

const ExportSettings& ExportDialog::GetExportSettings () const
{
	return exportSettings;
}

void ExportDialog::OnChoice (wxCommandEvent& evt)
{
	if (evt.GetId () == DialogIds::FormatChoiceId) {
		UpdateControls ();
	}
}

void ExportDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == DialogIds::ExportButtonId) {
		bool successfulExport = false;

		WXAS::BusyCursorGuard busyCursor;
		exportSettings.format = (ExportSettings::FormatId) formatChoice->GetSelection ();
		exportSettings.folder = outputFolderText->GetValue ();
		exportSettings.name = outputNameText->GetValue ();
		if (exportSettings.folder.empty () || exportSettings.name.empty ()) {
			return;
		}
		if (exportSettings.format == ExportSettings::FormatId::Obj || exportSettings.format == ExportSettings::FormatId::Stl || exportSettings.format == ExportSettings::FormatId::Off) {
			Modeler::FormatId modelerFormat = Modeler::FormatId::Obj;
			switch (exportSettings.format) {
				case ExportSettings::FormatId::Obj:
					modelerFormat = Modeler::FormatId::Obj;
					break;
				case ExportSettings::FormatId::Stl:
					modelerFormat = Modeler::FormatId::Stl;
					break;
				case ExportSettings::FormatId::Off:
					modelerFormat = Modeler::FormatId::Off;
					break;
			}
			wxFileWriter writer (exportSettings.folder);
			Modeler::ExportModel (model, modelerFormat, exportSettings.name, writer);
			successfulExport = true;
		} else if (exportSettings.format == ExportSettings::FormatId::Png) {
			if (exportSettings.image.IsValid ()) {
				RenderPixels pixels (exportSettings.image.width * exportSettings.image.multisampling, exportSettings.image.height * exportSettings.image.multisampling);
				RenderSettings renderSettings (ViewMode::Polygons, AxisMode::Off, 0.0, 0);
				scene.DrawOffscreen (renderSettings, pixels);

				wxImage image (pixels.GetWidth (), pixels.GetHeight ());
				image.InitAlpha ();
				for (int x = 0; x < image.GetWidth (); x++) {
					for (int y = 0; y < image.GetHeight (); y++) {
						unsigned char r, g, b, a;
						pixels.GetPixel (x, y, r, g, b, a);
						image.SetRGB (x, image.GetHeight () - y - 1, r, g, b);
						image.SetAlpha (x, image.GetHeight () - y - 1, a);
					}
				}
				if (exportSettings.image.multisampling != 1) {
					image.Rescale (exportSettings.image.width, exportSettings.image.height, wxIMAGE_QUALITY_NEAREST);
				}

				wxFileName filePath (exportSettings.folder, exportSettings.name + L".png");
				image.SaveFile (filePath.GetFullPath (), wxBITMAP_TYPE_PNG);
				successfulExport = true;
				image.Destroy ();
			}
		}

		if (successfulExport) {
			EndModal (wxID_OK);
		} else {
			wxMessageDialog messageDialog (this, L"Failed export model.", L"Error!", wxICON_ERROR | wxOK);
			messageDialog.ShowModal ();
		}
	} else if (evt.GetId () == DialogIds::FormatSettingsButtonId) {
		ImageSettingsDialog imageSettingsDialog (this, exportSettings.image);
		if (imageSettingsDialog.ShowModal () == wxID_OK) {
			exportSettings.image = imageSettingsDialog.GetImageSettings ();
			return;
		}
	} else if (evt.GetId () == DialogIds::BrowseFolderButtonId) {
		wxDirDialog dirDialog (this);
		if (dirDialog.ShowModal () == wxID_OK) {
			outputFolderText->SetValue (dirDialog.GetPath ());
		}
	}
}

void ExportDialog::UpdateControls ()
{
	formatSettingsButton->Disable ();
	ExportSettings::FormatId formatId = (ExportSettings::FormatId) formatChoice->GetSelection ();
	if (formatId == ExportSettings::FormatId::Png) {
		formatSettingsButton->Enable ();
	} else {
		formatSettingsButton->Disable ();
	}
}

BEGIN_EVENT_TABLE (ExportDialog, wxDialog)
EVT_CHOICE (wxID_ANY, ExportDialog::OnChoice)
EVT_BUTTON (wxID_ANY, ExportDialog::OnButtonClick)
END_EVENT_TABLE ()
