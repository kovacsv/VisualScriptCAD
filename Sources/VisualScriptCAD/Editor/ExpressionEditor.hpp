#ifndef EXPRESSIONEDITOR_HPP
#define EXPRESSIONEDITOR_HPP

#include <wx/wx.h>

class ExpressionEditorDialog : public wxDialog
{
public:
	ExpressionEditorDialog (wxWindow *parent, const std::wstring& expression);

	void					OnButtonClick (wxCommandEvent& evt);
	void					OnText (wxCommandEvent& evt);
	
private:
	wxBoxSizer*				boxSizer;
	wxTextCtrl*				expressionControl;
	wxButton*				okButton;

	std::wstring			expression;

	DECLARE_EVENT_TABLE ();
};

#endif
