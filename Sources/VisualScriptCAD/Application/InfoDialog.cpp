#include "InfoDialog.hpp"

InfoDialog::InfoDialog (wxWindow *parent, const std::wstring& title, const std::wstring& text) :
	wxDialog (parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	infoText (new wxTextCtrl (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY))
{
	infoText->SetValue (text);
}
