#ifndef CAMERADIALOG_HPP
#define CAMERADIALOG_HPP

#include "Camera.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

class CameraDialog : public wxDialog
{
public:
	enum DialogIds
	{
		EyeXSpinId = 1001,
		EyeYSpinId = 1002,
		EyeZSpinId = 1003,
		CenterXSpinId = 1004,
		CenterYSpinId = 1005,
		CenterZSpinId = 1006,
		UpXSpinId = 1007,
		UpYSpinId = 1008,
		UpZSpinId = 1009,
		SaveButtonId = 1100
	};

	CameraDialog (wxWindow* parent, const Modeler::Camera& camera);

	Modeler::Camera		GetCamera () const;

private:
	void				OnButtonClick (wxCommandEvent& evt);

	Modeler::Camera		origCamera;

	wxButton*			saveButton;
	wxBoxSizer*			boxSizer;
	wxSpinCtrlDouble*	eyeXSpin;
	wxSpinCtrlDouble*	eyeYSpin;
	wxSpinCtrlDouble*	eyeZSpin;
	wxSpinCtrlDouble*	centerXSpin;
	wxSpinCtrlDouble*	centerYSpin;
	wxSpinCtrlDouble*	centerZSpin;
	wxSpinCtrlDouble*	upXSpin;
	wxSpinCtrlDouble*	upYSpin;
	wxSpinCtrlDouble*	upZSpin;

	DECLARE_EVENT_TABLE ();
};

#endif
