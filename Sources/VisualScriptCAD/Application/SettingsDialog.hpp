#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include "UserSettings.hpp"

#include <wx/wx.h>
#include <wx/listbox.h>

#include <memory>

class SettingsDialogPanel
{
public:
	SettingsDialogPanel (const std::wstring& name, wxDialog* parent);
	virtual ~SettingsDialogPanel ();

	const std::wstring&		GetName () const;
	wxPanel*				GetPanel ();

	virtual void			LoadFromUserSettings (const UserSettings& userSettings) = 0;
	virtual void			SaveToUserSettings (UserSettings& userSettings) const = 0;

protected:
	std::wstring	name;
	wxPanel*		panel;
	bool			loaded;
};

class SettingsDialog : public wxDialog
{
public:
	enum DialogIds
	{
		SettingsListId = 1001,
		SaveButtonId = 1100
	};

	SettingsDialog (wxWindow *parent, const UserSettings& userSettings);

	const UserSettings&		GetUserSettings () const;

private:
	void					OnListItemSelected (wxCommandEvent& evt);
	void					OnButtonClick (wxCommandEvent& evt);

	void					AddPanels ();
	void					SaveAllPanels ();
	void					ActivatePanel (size_t index);

	wxBoxSizer*											horizontalSizer;
	wxBoxSizer*											verticalSizer;
	wxListBox*											settingsList;
	wxButton*											saveButton;
	std::vector<std::unique_ptr<SettingsDialogPanel>>	panels;
	
	UserSettings										userSettings;

	DECLARE_EVENT_TABLE ();
};

#endif
