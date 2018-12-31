#ifndef INFODIALOG_HPP
#define INFODIALOG_HPP

#include <wx/wx.h>

class InfoDialog : public wxDialog
{
public:
	InfoDialog (wxWindow *parent, const std::wstring& title, const std::wstring& text);

private:
	wxTextCtrl*		infoText;
};

#endif
