#include "SettingsDialog.hpp"

SettingsDialogPanel::SettingsDialogPanel (const std::wstring& name, wxDialog* parent) :
	name (name),
	panel (new wxPanel (parent, wxID_ANY))
{

}

SettingsDialogPanel::~SettingsDialogPanel ()
{

}

const std::wstring& SettingsDialogPanel::GetName () const
{
	return name;
}

class ModelViewPanel : public SettingsDialogPanel
{
public:
	enum DialogIds
	{
		ViewModeChoiceId = 1001,
		AxisModeChoiceId = 1002
	};

	ModelViewPanel (wxDialog* parent, const UserSettings& userSettings) :
		SettingsDialogPanel (L"Model View", parent),
		gridSizer (new wxGridSizer (2, 2, 5, 5)),
		boxSizer (new wxBoxSizer (wxVERTICAL)),
		viewModeChoice (new wxChoice (panel, DialogIds::ViewModeChoiceId)),
		axisModeChoice (new wxChoice (panel, DialogIds::AxisModeChoiceId))
	{
		viewModeChoice->Append (L"Lines");
		viewModeChoice->Append (L"Polygons");
		if (userSettings.renderSettings.viewMode == ViewMode::Lines) {
			viewModeChoice->Select (0);
		} else if (userSettings.renderSettings.viewMode == ViewMode::Polygons) {
			viewModeChoice->Select (1);
		}

		axisModeChoice->Append (L"On");
		axisModeChoice->Append (L"Off");
		if (userSettings.renderSettings.axisMode == AxisMode::On) {
			axisModeChoice->Select (0);
		} else if (userSettings.renderSettings.axisMode == AxisMode::Off) {
			axisModeChoice->Select (1);
		}

		gridSizer->Add (new wxStaticText (panel, wxID_ANY, L"View Mode"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (viewModeChoice, 1, wxEXPAND);
		gridSizer->Add (new wxStaticText (panel, wxID_ANY, L"Axis Mode"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (axisModeChoice, 1, wxEXPAND);

		boxSizer->Add (gridSizer, 1, wxEXPAND | wxALL, 5);
		panel->SetSizer (boxSizer);
	}

	virtual void SaveUserSettings (UserSettings& userSettings) override
	{
		if (viewModeChoice->GetSelection () == 0) {
			userSettings.renderSettings.viewMode = ViewMode::Lines;
		} else if (viewModeChoice->GetSelection () == 1) {
			userSettings.renderSettings.viewMode = ViewMode::Polygons;
		}

		if (axisModeChoice->GetSelection () == 0) {
			userSettings.renderSettings.axisMode = AxisMode::On;
		} else if (axisModeChoice->GetSelection () == 1) {
			userSettings.renderSettings.axisMode = AxisMode::Off;
		}
	}

private:
	wxGridSizer*	gridSizer;
	wxBoxSizer*		boxSizer;
	wxChoice*		viewModeChoice;
	wxChoice*		axisModeChoice;
};

wxPanel* SettingsDialogPanel::GetPanel ()
{
	return panel;
}

SettingsDialog::SettingsDialog (wxWindow *parent, const UserSettings& userSettings) :
	wxDialog (parent, wxID_ANY, L"Options", wxDefaultPosition, wxSize (600, 400), wxDEFAULT_DIALOG_STYLE),
	horizontalSizer (new wxBoxSizer (wxHORIZONTAL)),
	verticalSizer (new wxBoxSizer (wxVERTICAL)),
	settingsList (new wxListBox (this, SettingsListId, wxDefaultPosition, wxSize (200, 0))),
	saveButton (new wxButton (this, wxID_OK, L"Save")),
	panels (),
	userSettings (userSettings)
{
	horizontalSizer->Add (settingsList, 0, wxEXPAND, 5);
	verticalSizer->Add (horizontalSizer, 1, wxEXPAND | wxALL, 5);
	verticalSizer->Add (saveButton, 0, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);

	AddPanels ();
	ActivatePanel (0);
	SetSizer (verticalSizer);

	SetEscapeId (wxID_CANCEL);
}

const UserSettings& SettingsDialog::GetUserSettings () const
{
	return userSettings;
}

void SettingsDialog::OnListItemSelected (wxCommandEvent& evt)
{
	if (evt.GetId () == SettingsListId) {
		ActivatePanel (settingsList->GetSelection ());
	}
}

void SettingsDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == wxID_OK) {
		for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
			panel->SaveUserSettings (userSettings);
		}
		EndModal (wxID_OK);
	}
}

void SettingsDialog::AddPanels ()
{
	panels.push_back (std::unique_ptr<SettingsDialogPanel> (new ModelViewPanel (this, userSettings)));

	for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
		settingsList->Append (panel->GetName ());
	}
}

void SettingsDialog::ActivatePanel (size_t index)
{
	settingsList->Select (index);

	std::unique_ptr<SettingsDialogPanel>& activePanel = panels[index];
	for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
		panel->GetPanel ()->Hide ();
	}
	activePanel->GetPanel ()->Show ();

	if (horizontalSizer->GetItemCount () > 1) {
		horizontalSizer->Remove (1);
	}
	horizontalSizer->Add (activePanel->GetPanel ());
	horizontalSizer->RecalcSizes ();
	verticalSizer->RecalcSizes ();
}

BEGIN_EVENT_TABLE (SettingsDialog, wxDialog)
EVT_LISTBOX (wxID_ANY, SettingsDialog::OnListItemSelected)
EVT_BUTTON (wxID_ANY, SettingsDialog::OnButtonClick)
END_EVENT_TABLE ()
