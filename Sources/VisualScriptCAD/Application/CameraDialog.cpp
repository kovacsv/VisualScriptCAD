#include "CameraDialog.hpp"
#include "Geometry.hpp"

static const wxSize nameMinSize (100, -1);
static const wxSize smallControlMinSize (100, -1);
static const wxSize controlMinSize (300, -1);
static const double MinCameraValue = -10000.0;
static const double MaxCameraValue = 10000.0;

CameraDialog::CameraDialog (wxWindow* parent, const Modeler::Camera& camera) :
	wxDialog (parent, wxID_ANY, L"Camera Settings", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	origCamera (camera),
	saveButton (new wxButton (this, DialogIds::SaveButtonId, L"Save")),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	eyeXSpin (new wxSpinCtrlDouble (this, DialogIds::EyeXSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	eyeYSpin (new wxSpinCtrlDouble (this, DialogIds::EyeYSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	eyeZSpin (new wxSpinCtrlDouble (this, DialogIds::EyeZSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	centerXSpin (new wxSpinCtrlDouble (this, DialogIds::CenterXSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	centerYSpin (new wxSpinCtrlDouble (this, DialogIds::CenterYSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	centerZSpin (new wxSpinCtrlDouble (this, DialogIds::CenterZSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	upXSpin (new wxSpinCtrlDouble (this, DialogIds::UpXSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	upYSpin (new wxSpinCtrlDouble (this, DialogIds::UpYSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1)),
	upZSpin (new wxSpinCtrlDouble (this, DialogIds::UpZSpinId, L"", wxDefaultPosition, smallControlMinSize, wxSP_ARROW_KEYS, MinCameraValue, MaxCameraValue, 0.0, 0.1))
{
	{
		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Eye Position", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		wxBoxSizer* vectorHorizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		vectorHorizontalSizer->Add (eyeXSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (eyeYSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (eyeZSpin, 1, wxEXPAND | wxALL, 5);
		horizontalSizer->Add (vectorHorizontalSizer, 1, wxEXPAND | wxALL, 0);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	{
		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Center Position", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		wxBoxSizer* vectorHorizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		vectorHorizontalSizer->Add (centerXSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (centerYSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (centerZSpin, 1, wxEXPAND | wxALL, 5);
		horizontalSizer->Add (vectorHorizontalSizer, 1, wxEXPAND | wxALL, 0);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	{
		wxBoxSizer* horizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		horizontalSizer->Add (new wxStaticText (this, wxID_ANY, L"Up Vector", wxDefaultPosition, nameMinSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		wxBoxSizer* vectorHorizontalSizer = new wxBoxSizer (wxHORIZONTAL);
		vectorHorizontalSizer->Add (upXSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (upYSpin, 1, wxEXPAND | wxALL, 5);
		vectorHorizontalSizer->Add (upZSpin, 1, wxEXPAND | wxALL, 5);
		horizontalSizer->Add (vectorHorizontalSizer, 1, wxEXPAND | wxALL, 0);
		boxSizer->Add (horizontalSizer, 0, wxEXPAND);
	}

	eyeXSpin->SetValue (camera.GetEye ().x);
	eyeYSpin->SetValue (camera.GetEye ().y);
	eyeZSpin->SetValue (camera.GetEye ().z);

	centerXSpin->SetValue (camera.GetCenter ().x);
	centerYSpin->SetValue (camera.GetCenter ().y);
	centerZSpin->SetValue (camera.GetCenter ().z);

	upXSpin->SetValue (camera.GetUp ().x);
	upYSpin->SetValue (camera.GetUp ().y);
	upZSpin->SetValue (camera.GetUp ().z);

	boxSizer->Add (saveButton, 0, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, 5);
	SetSizerAndFit (boxSizer);
	SetEscapeId (wxID_CANCEL);
}

Modeler::Camera CameraDialog::GetCamera () const
{
	glm::dvec3 eye (eyeXSpin->GetValue (), eyeYSpin->GetValue (), eyeZSpin->GetValue ());
	glm::dvec3 center (centerXSpin->GetValue (), centerYSpin->GetValue (), centerZSpin->GetValue ());
	glm::dvec3 up (upXSpin->GetValue (), upYSpin->GetValue (), upZSpin->GetValue ());
	Modeler::Camera camera (eye, center, glm::normalize (up), origCamera.GetFieldOfViewY (), origCamera.GetNearPlane (), origCamera.GetFarPlane ());
	return camera;
}

void CameraDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == DialogIds::SaveButtonId) {
		glm::dvec3 eye (eyeXSpin->GetValue (), eyeYSpin->GetValue (), eyeZSpin->GetValue ());
		glm::dvec3 center (centerXSpin->GetValue (), centerYSpin->GetValue (), centerZSpin->GetValue ());
		glm::dvec3 up (upXSpin->GetValue (), upYSpin->GetValue (), upZSpin->GetValue ());
		if (Modeler::IsValidCamera (eye, center, up, origCamera.GetFieldOfViewY (), origCamera.GetNearPlane (), origCamera.GetFarPlane ())) {
			EndModal (wxID_OK);
		}
	}
}

BEGIN_EVENT_TABLE (CameraDialog, wxDialog)
EVT_BUTTON (wxID_ANY, CameraDialog::OnButtonClick)
END_EVENT_TABLE ()
