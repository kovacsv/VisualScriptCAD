#include "SettingsDialog.hpp"

static const wxSize ChoiceControlSize (250, 25);
static const wxSize SliderControlSize (250, 35);

SettingsDialogPanel::SettingsDialogPanel (const std::wstring& name, wxDialog* parent) :
	name (name),
	panel (new wxPanel (parent, wxID_ANY)),
	loaded (false)
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
		AxisModeChoiceId = 1002,
		AxisSizeSliderId = 1002
	};

	ModelViewPanel (wxDialog* parent) :
		SettingsDialogPanel (L"Model View", parent),
		gridSizer (new wxFlexGridSizer (3, 2, 5, 20)),
		boxSizer (new wxBoxSizer (wxVERTICAL)),
		viewModeChoice (new wxChoice (panel, DialogIds::ViewModeChoiceId, wxDefaultPosition, ChoiceControlSize)),
		axisModeChoice (new wxChoice (panel, DialogIds::AxisModeChoiceId, wxDefaultPosition, ChoiceControlSize)),
		axisSizeSlider (new wxSlider (panel, DialogIds::AxisSizeSliderId, 10, 5, 50, wxDefaultPosition, SliderControlSize, wxSL_LABELS))
	{
		viewModeChoice->Append (L"Lines");
		viewModeChoice->Append (L"Polygons");

		axisModeChoice->Append (L"On");
		axisModeChoice->Append (L"Off");

		gridSizer->Add (new wxStaticText (panel, wxID_ANY, L"View Mode"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (viewModeChoice, 1, wxEXPAND);
		gridSizer->Add (new wxStaticText (panel, wxID_ANY, L"Axis Mode"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (axisModeChoice, 1, wxEXPAND);
		gridSizer->Add (new wxStaticText (panel, wxID_ANY, L"Axis Size"), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
		gridSizer->Add (axisSizeSlider, 1, wxEXPAND);

		boxSizer->Add (gridSizer, 1, wxEXPAND | wxALL, 5);
		panel->SetSizer (boxSizer);
	}

	virtual void LoadFromUserSettings (const UserSettings& userSettings) override
	{
		if (userSettings.renderSettings.viewMode == ViewMode::Lines) {
			viewModeChoice->Select (0);
		} else if (userSettings.renderSettings.viewMode == ViewMode::Polygons) {
			viewModeChoice->Select (1);
		}

		if (userSettings.renderSettings.axisMode == AxisMode::On) {
			axisModeChoice->Select (0);
		} else if (userSettings.renderSettings.axisMode == AxisMode::Off) {
			axisModeChoice->Select (1);
		}

		axisSizeSlider->SetValue (userSettings.renderSettings.axisSize);
	}

	virtual void SaveToUserSettings (UserSettings& userSettings) const override
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

		userSettings.renderSettings.axisSize = axisSizeSlider->GetValue ();
	}

private:
	wxFlexGridSizer*	gridSizer;
	wxBoxSizer*			boxSizer;
	wxChoice*			viewModeChoice;
	wxChoice*			axisModeChoice;
	wxSlider*			axisSizeSlider;
};

wxPanel* SettingsDialogPanel::GetPanel ()
{
	return panel;
}

void SettingsDialogPanel::SetLoaded ()
{
	loaded = true;
}

bool SettingsDialogPanel::IsLoaded () const
{
	return loaded;
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
	settingsList->Select (0);

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
		SaveAllPanels ();
		EndModal (wxID_OK);
	}
}

void SettingsDialog::AddPanels ()
{
	panels.push_back (std::unique_ptr<SettingsDialogPanel> (new ModelViewPanel (this)));

	for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
		settingsList->Append (panel->GetName ());
	}
}

void SettingsDialog::SaveAllPanels ()
{
	for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
		if (panel->IsLoaded ()) {
			panel->SaveToUserSettings (userSettings);
		}
	}
}

void SettingsDialog::ActivatePanel (size_t index)
{
	SaveAllPanels ();

	std::unique_ptr<SettingsDialogPanel>& activePanel = panels[index];
	for (std::unique_ptr<SettingsDialogPanel>& panel : panels) {
		panel->GetPanel ()->Hide ();
	}
	activePanel->GetPanel ()->Show ();
	activePanel->LoadFromUserSettings (userSettings);
	activePanel->SetLoaded ();

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
