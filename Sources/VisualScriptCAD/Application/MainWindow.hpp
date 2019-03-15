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
	Edit_Undo					= 7,
	Edit_Redo					= 8,
	Mode_Automatic				= 9,
	Mode_Manual					= 10,
	Mode_Update					= 11,
	View_Editor					= 12,
	View_Model					= 13,
	View_Split					= 14,
	Editor_FitToWindow			= 15,
	Model_FitToWindow			= 16,
	Model_ViewMode_Lines		= 17,
	Model_ViewMode_Polygons		= 18,
	Model_AxisMode_On			= 19,
	Model_AxisMode_Off			= 20,
	Model_Info					= 21,
	Model_Export				= 22,
	About_GitHub				= 23,
	File_OpenRecent_First		= 100
};

enum class SplitViewMode
{
	Editor,
	Model,
	Split
};

class EvaluationData : public ModelEvaluationData
{
public:
	EvaluationData (Modeler::Model& model);
	virtual ~EvaluationData ();

	virtual Modeler::Model&						GetModel () override;
	virtual Modeler::MeshId						AddMesh (const Modeler::Mesh& mesh) override;
	virtual void								RemoveMesh (Modeler::MeshId meshId) override;

	const Modeler::Model&						GetModel () const;

	const std::unordered_set<Modeler::MeshId>&	GetAddedMeshes () const;
	const std::unordered_set<Modeler::MeshId>&	GetDeletedMeshes () const;
	void										Clear ();

private:
	Modeler::Model&							model;
	std::unordered_set<Modeler::MeshId>		addedMeshes;
	std::unordered_set<Modeler::MeshId>		deletedMeshes;
};

class ModelControlSynchronizer : public ModelSynchronizer
{
public:
	ModelControlSynchronizer (std::shared_ptr<EvaluationData>& evalData, ModelControl* modelControl);

	virtual void Synchronize () override;

private:
	std::shared_ptr<EvaluationData>& evalData;
	ModelControl* modelControl;
};

class MenuBar : public wxMenuBar
{
public:
	MenuBar ();

	void		UpdateStatus (SplitViewMode viewMode, WXAS::NodeEditorControl::UpdateMode updateMode, const UserSettings& userSettings);

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

	Modeler::Model						model;
	std::shared_ptr<EvaluationData>		evaluationData;
	MenuBar*							menuBar;
	ToolBar*							toolBar;
	wxSplitterWindow*					editorAndModelSplitter;
	ModelControl*						modelControl;
	ModelControlSynchronizer			modelControlSynchronizer;
	NodeEditorControl*					nodeEditorControl;

	ApplicationState					applicationState;
	SplitViewMode						splitViewMode;
	UserSettings						userSettings;
	int									sashPosition;

	DECLARE_EVENT_TABLE ()
};

#endif
