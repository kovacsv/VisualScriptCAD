#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ModelEvaluationData.hpp"
#include "NodeEditorControl.hpp"
#include "ModelControl.hpp"

#include <wx/wx.h>
#include <wx/splitter.h>

enum CommandId
{
	File_New					= 1,
	File_Open					= 2,
	File_Save					= 3,
	File_SaveAs					= 4,
	File_Exit					= 5,
	File_ClearRecent			= 6,
	View_Editor_FitToWindow		= 7,
	View_Model_FitToWindow		= 8,
	Tools_Options				= 9,
	Model_Info					= 10,
	Model_Export				= 11,
	About_GitHub				= 12,
	Tool_Undo					= 13,
	Tool_Redo					= 14,
	Tool_View_Editor			= 15,
	Tool_View_Model				= 16,
	Tool_View_Split				= 17,
	Tool_Mode_Automatic			= 18,
	Tool_Mode_Manual			= 19,
	Tool_Mode_Update			= 20,
	File_OpenExample_First		= 100,
	File_OpenRecent_First		= 200
};

enum class SplitViewMode
{
	Editor,
	Model,
	Split
};

class ModelControlSynchronizer : public ModelSynchronizer
{
public:
	ModelControlSynchronizer (std::shared_ptr<ModelEvaluationData>& evalData, ModelControl* modelControl);

	virtual void Synchronize () override;

private:
	std::shared_ptr<ModelEvaluationData>&	evalData;
	ModelControl*							modelControl;
};

class MenuBar : public wxMenuBar
{
public:
	MenuBar ();

	void		UpdateStatus (const UserSettings& userSettings);

private:
	wxMenu*		openRecentMenu;
};

class ToolBar : public wxToolBar
{
public:
	ToolBar (wxWindow* parent);

	void	UpdateStatus (SplitViewMode viewMode, WXAS::NodeEditorControl::UpdateMode updateMode);

private:
	wxToolBarToolBase*	AddIconButton (int toolId, const void* imageData, size_t imageSize, std::wstring helpText);
	wxToolBarToolBase*	AddRadioButton (int toolId, const void* imageData, size_t imageSize, std::wstring helpText);
};

class ApplicationState
{
public:
	ApplicationState ();

	void					ClearCurrentFileName ();
	void					SetCurrentFileName (const std::wstring& newCurrentFileName);
	bool					HasCurrentFileName () const;
	const std::wstring&		GetCurrentFileName () const;

private:
	std::wstring currentFileName;
};

class MainWindow : public wxFrame
{
public:
	MainWindow (const std::wstring& fileName);
	~MainWindow ();

	void	OnCommand (wxCommandEvent& evt);
	void	OnClose (wxCloseEvent& event);
	void	ProcessCommand (CommandId commandId);
	void	OnKeyDown (wxKeyEvent& evt);

private:
	bool	ConfirmLosingUnsavedChanges ();
	void	SyncUserSettings ();
	void	UpdateMenuBar ();
	void	UpdateStatusBar ();

	void	NewFile ();
	void	OpenFile (const std::wstring& fileName);

	std::shared_ptr<ModelEvaluationData>	evaluationData;
	MenuBar*								menuBar;
	ToolBar*								toolBar;
	wxSplitterWindow*						editorAndModelSplitter;
	ModelControl*							modelControl;
	ModelControlSynchronizer				modelControlSynchronizer;
	NodeEditorControl*						nodeEditorControl;

	ApplicationState						applicationState;
	SplitViewMode							splitViewMode;
	UserSettings							userSettings;
	int										sashPosition;

	DECLARE_EVENT_TABLE ()
};

#endif
