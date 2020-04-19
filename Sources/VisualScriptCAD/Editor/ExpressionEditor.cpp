#include "ExpressionEditor.hpp"
#include "ExpressionCalculator.hpp"

ExpressionEditorDialog::ExpressionEditorDialog (wxWindow* parent, const std::wstring& expression) :
	wxDialog (parent, wxID_ANY, L"Edit Expression", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	expressionControl (new wxTextCtrl (this, wxID_ANY, expression, wxDefaultPosition, wxSize (400, 30))),
	okButton (new wxButton (this, wxID_OK, L"OK")),
	expression (expression)
{
	boxSizer->Add (expressionControl, 1, wxEXPAND | wxALL, 0);
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 0);
	SetSizerAndFit (boxSizer);

	SetEscapeId (wxID_CANCEL);
}

void ExpressionEditorDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == wxID_OK) {
		EndModal (wxID_OK);
	}
}

void ExpressionEditorDialog::OnText (wxCommandEvent&)
{
	expression = expressionControl->GetValue ();
	okButton->Enable (BoostOperations::ParseExpression (expression));
}

const std::wstring& ExpressionEditorDialog::GetExpression () const
{
	return expression;
}

BEGIN_EVENT_TABLE (ExpressionEditorDialog, wxDialog)
EVT_BUTTON (wxID_ANY, ExpressionEditorDialog::OnButtonClick)
EVT_TEXT (wxID_ANY, ExpressionEditorDialog::OnText)
END_EVENT_TABLE ()
