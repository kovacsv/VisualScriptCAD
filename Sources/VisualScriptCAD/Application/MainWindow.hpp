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
	View_Editor_AlignToWindow	= 7,
	View_Editor_FitToWindow		= 8,
	View_Model_FitToWindow		= 9,
	View_Model_ResetView		= 10,
	Tools_Options				= 11,
	Model_Info					= 12,
	Model_Export				= 13,
	About_GitHub				= 14,
	Tool_Undo					= 15,
	Tool_Redo					= 16,
	Tool_View_Editor			= 17,
	Tool_View_Model				= 18,
	Tool_View_Split				= 19,
	Tool_Mode_Automatic			= 20,
	Tool_Mode_Manual			= 21,
	Tool_Mode_Update			= 22,
	File_OpenExample_First		= 100,
	File_OpenRecent_First		= 200
};

enum class SplitViewMode
{
	Editor,
	Model,
	Split
};

class EditorModelBridge :	public SelectionUpdater,
							public ModelUpdater
							
{
public:
	EditorModelBridge ();

	void			Init (const std::shared_ptr<ModelEvaluationData>& newEvalData, NodeEditorControl* newNodeEditorControl, ModelControl* newModelControl);

	virtual void	UpdateSelection (const NE::NodeCollection& selectedNodes) override;
	virtual void	UpdateModel () override;

private:
	std::shared_ptr<ModelEvaluationData>	evalData;
	NodeEditorControl*						nodeEditorControl;
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
	EditorModelBridge						editorModelBridge;
	ModelControl*							modelControl;
	NodeEditorControl*						nodeEditorControl;

	ApplicationState						applicationState;
	SplitViewMode							splitViewMode;
	UserSettings							userSettings;
	int										sashPosition;

	DECLARE_EVENT_TABLE ()
};

#endif
