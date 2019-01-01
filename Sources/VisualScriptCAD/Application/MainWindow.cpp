#include "MainWindow.hpp"

#include "ModelControl.hpp"
#include "RenderModelConverter.hpp"
#include "ModelEvaluationData.hpp"
#include "InfoDialog.hpp"
#include "ModelExport.hpp"
#include "IconStore.hpp"
#include "Version.hpp"
#include "VersionInfo.hpp"

#include <locale>
#include <codecvt>

class ApplicationHeaderIO : public NUIE::ExternalHeaderIO
{
public:
	ApplicationHeaderIO ()
	{

	}

	virtual bool Read (NE::InputStream& inputStream) const override
	{
		std::wstring appName;
		inputStream.Read (appName);
		if (appName != VSCAD_APP_NAME) {
			return false;
		}
		Version readVersion;
		readVersion.Read (inputStream);
		if (readVersion > AppVersion) {
			return false;
		}
		int readFileVersion = 0;
		inputStream.Read (readFileVersion);
		if (readFileVersion > FileVersion) {
			return false;
		}
		return true;
	}

	virtual void Write (NE::OutputStream& outputStream) const override
	{
		outputStream.Write (std::wstring (VSCAD_APP_NAME));
		AppVersion.Write (outputStream);
		outputStream.Write (FileVersion);
	}
};

static const ApplicationHeaderIO appHeaderIO;

static std::string WideStringToNormalString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes (str);
}

EvaluationData::EvaluationData (Modeler::Model& model) :
	model (model)
{
}

EvaluationData::~EvaluationData ()
{
}

Modeler::Model& EvaluationData::GetModel ()
{
	return model;
}

Modeler::MeshId EvaluationData::AddMesh (const Modeler::Mesh& mesh)
{
	Modeler::MeshId meshId = model.AddMesh (mesh);
	addedMeshes.insert (meshId);
	return meshId;
}

void EvaluationData::RemoveMesh (Modeler::MeshId meshId)
{
	model.RemoveMesh (meshId);
	deletedMeshes.insert (meshId);
}

const Modeler::Model& EvaluationData::GetModel () const
{
	return model;
}

const std::unordered_set<Modeler::MeshId>& EvaluationData::GetAddedMeshes () const
{
	return addedMeshes;
}

const std::unordered_set<Modeler::MeshId>& EvaluationData::GetDeletedMeshes () const
{
	return deletedMeshes;
}

void EvaluationData::Clear ()
{
	addedMeshes.clear ();
	deletedMeshes.clear ();
}

ModelControlSynchronizer::ModelControlSynchronizer (std::shared_ptr<EvaluationData>& evalData, ModelControl* modelControl) :
	evalData (evalData),
	modelControl (modelControl)
{
}

void ModelControlSynchronizer::Synchronize ()
{
	for (Modeler::MeshId meshId : evalData->GetDeletedMeshes ()) {
		modelControl->RemoveMesh (meshId);
	}
	for (Modeler::MeshId meshId : evalData->GetAddedMeshes ()) {
		modelControl->AddMesh (evalData->GetModel (), meshId);
	}
	evalData->Clear ();
}

ApplicationState::ApplicationState () :
	currentFileName ()
{

}

void ApplicationState::ClearCurrentFileName ()
{
	currentFileName.clear ();
}

void ApplicationState::SetCurrentFileName (const std::wstring& newCurrentFileName)
{
	currentFileName = newCurrentFileName;
}

bool ApplicationState::HasCurrentFileName () const
{
	return !currentFileName.empty ();
}

const std::wstring& ApplicationState::GetCurrentFileName () const
{
	return currentFileName;
}

MenuBar::MenuBar () :
	wxMenuBar ()
{
	wxMenu* fileMenu = new wxMenu ();
	fileMenu->Append (CommandId::File_New, "New");
	fileMenu->Append (CommandId::File_Open, "Open...");
	fileMenu->Append (CommandId::File_Save, "Save...");
	fileMenu->Append (CommandId::File_SaveAs, "Save As...");
	fileMenu->AppendSeparator ();
	fileMenu->Append (CommandId::File_Exit, L"Exit");
	Append (fileMenu, L"&File");

	wxMenu* editMenu = new wxMenu ();
	editMenu->Append (CommandId::Edit_Undo, "Undo");
	editMenu->Append (CommandId::Edit_Redo, "Redo");
	Append (editMenu, L"&Edit");

	wxMenu* viewMenu = new wxMenu ();
	viewMenu->AppendRadioItem (CommandId::View_Editor, "Show Editor");
	viewMenu->AppendRadioItem (CommandId::View_Model, "Show Model");
	viewMenu->AppendRadioItem (CommandId::View_Split, "Split View");
	Append (viewMenu, L"&View");

	wxMenu* editorMenu = new wxMenu ();
	wxMenu* updateModeMenu = new wxMenu ();
	updateModeMenu->AppendRadioItem (CommandId::Mode_Automatic, "Automatic");
	updateModeMenu->AppendRadioItem (CommandId::Mode_Manual, "Manual");
	updateModeMenu->AppendSeparator ();
	updateModeMenu->Append (CommandId::Mode_Update, "Update Now");
	editorMenu->AppendSubMenu (updateModeMenu, L"Update");
	editorMenu->Append (CommandId::Editor_FitToWindow, "Fit to Window");
	Append (editorMenu, L"E&ditor");

	wxMenu* modelMenu = new wxMenu ();
	modelMenu->Append (CommandId::Model_FitToWindow, "Fit to Window");
	wxMenu* viewModeMenu = new wxMenu ();
	viewModeMenu->AppendRadioItem (CommandId::Model_ViewMode_Lines, "Lines");
	viewModeMenu->AppendRadioItem (CommandId::Model_ViewMode_Polygons, "Polygons")->Check ();
	modelMenu->AppendSubMenu (viewModeMenu, L"View Mode");
	wxMenu* axisModeMenu = new wxMenu ();
	axisModeMenu->AppendRadioItem (CommandId::Model_AxisMode_On, "On");
	axisModeMenu->AppendRadioItem (CommandId::Model_AxisMode_Off, "Off")->Check ();
	modelMenu->AppendSubMenu (axisModeMenu, L"Axis Mode");
	modelMenu->Append (CommandId::Model_Info, "Information...");
	modelMenu->Append (CommandId::Model_Export, "Export...");
	Append (modelMenu, L"&Model");

	wxMenu* aboutMenu = new wxMenu ();
	aboutMenu->Append (CommandId::About_GitHub, "Open GitHub");
	Append (aboutMenu, L"&About");
}

void MenuBar::UpdateStatus (ViewMode viewMode, WXAS::NodeEditorControl::UpdateMode updateMode)
{
	if (viewMode == ViewMode::Editor) {
		FindItem (CommandId::View_Editor)->Check (true);
	} else if (viewMode == ViewMode::Model) {
		FindItem (CommandId::View_Model)->Check (true);
	} else if (viewMode == ViewMode::Split) {
		FindItem (CommandId::View_Split)->Check (true);
	} else {
		DBGBREAK ();
	}

	if (updateMode == WXAS::NodeEditorControl::UpdateMode::Automatic) {
		FindItem (CommandId::Mode_Automatic)->Check (true);
	} else if (updateMode == WXAS::NodeEditorControl::UpdateMode::Manual) {
		FindItem (CommandId::Mode_Manual)->Check (true);
	} else {
		DBGBREAK ();
	}
}

ToolBar::ToolBar (wxWindow* parent) :
	wxToolBar (parent, wxID_ANY)
{
	AddIconButton (CommandId::File_New, page, page_size, L"New");
	AddIconButton (CommandId::File_Open, page_go, page_go_size, L"Open");
	AddIconButton (CommandId::File_Save, page_save, page_save_size, L"Save");

	AddSeparator ();

	AddIconButton (CommandId::Edit_Undo, arrow_undo, arrow_undo_size, L"Undo");
	AddIconButton (CommandId::Edit_Redo, arrow_redo, arrow_redo_size, L"Redo");

	AddSeparator ();

	AddRadioButton (CommandId::View_Editor, application_side_expand, application_side_expand_size, L"Show Editor");
	AddRadioButton (CommandId::View_Model, application_side_contract, application_side_contract_size, L"Show Model");
	AddRadioButton (CommandId::View_Split, application_tile_horizontal, application_tile_horizontal_size, L"Split View");

	AddSeparator ();

	AddRadioButton (CommandId::Mode_Automatic, control_end_blue, control_end_blue_size, L"Automatic Update");
	AddRadioButton (CommandId::Mode_Manual, control_pause_blue, control_pause_blue_size, L"Manual Update");
	AddIconButton (CommandId::Mode_Update, control_play_blue, control_play_blue_size, L"Update Now");

	AddSeparator ();

	AddIconButton (CommandId::Model_FitToWindow, arrow_in, arrow_in_size, L"Fit to Window");

	ToggleTool (CommandId::Mode_Automatic, true);
	ToggleTool (CommandId::View_Split, true);

	Realize ();
}

void ToolBar::UpdateStatus (ViewMode viewMode, WXAS::NodeEditorControl::UpdateMode updateMode)
{
	if (viewMode == ViewMode::Editor) {
		ToggleTool (CommandId::View_Editor, true);
	} else if (viewMode == ViewMode::Model) {
		ToggleTool (CommandId::View_Model, true);
	} else if (viewMode == ViewMode::Split) {
		ToggleTool (CommandId::View_Split, true);
	} else {
		DBGBREAK ();
	}

	if (updateMode == WXAS::NodeEditorControl::UpdateMode::Automatic) {
		ToggleTool (CommandId::Mode_Automatic, true);
	} else if (updateMode == WXAS::NodeEditorControl::UpdateMode::Manual) {
		ToggleTool (CommandId::Mode_Manual, true);
	} else {
		DBGBREAK ();
	}
}

wxToolBarToolBase* ToolBar::AddIconButton (int toolId, const void* imageData, size_t imageSize, std::wstring helpText)
{
	return AddTool (toolId, wxEmptyString, wxBitmap::NewFromPNGData (imageData, imageSize), wxNullBitmap, wxITEM_NORMAL, helpText.c_str ());
}

wxToolBarToolBase* ToolBar::AddRadioButton (int toolId, const void* imageData, size_t imageSize, std::wstring helpText)
{
	return AddRadioTool (toolId, wxEmptyString, wxBitmap::NewFromPNGData (imageData, imageSize), wxNullBitmap, helpText.c_str ());
}

MainWindow::MainWindow (const std::wstring& defaultFileName) :
	wxFrame (NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (1000, 600)),
	model (),
	evaluationData (new EvaluationData (model)),
	menuBar (new MenuBar ()),
	toolBar (new ToolBar (this)),
	editorAndModelSplitter (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
	modelControl (new ModelControl (editorAndModelSplitter)),
	modelControlSynchronizer (evaluationData, modelControl),
	nodeEditorControl (new NodeEditorControl (editorAndModelSplitter, evaluationData, modelControlSynchronizer)),
	viewMode (ViewMode::Split),
	sashPosition (700)
{
	wxIcon icon;
	icon.CopyFromBitmap (wxBitmap::NewFromPNGData (appicon32, appicon32_size));
	SetIcon (icon);

	std::wstring title = std::wstring (L"Visual Script CAD ") + std::wstring (VSCAD_VERSION_STRING);
	SetTitle (title);

	SetMinSize (wxSize (640, 480));
	SetMenuBar (menuBar);
	SetToolBar (toolBar);
	CreateStatusBar ();
	UpdateStatusBar ();

	editorAndModelSplitter->SetSashGravity (0.5);
	editorAndModelSplitter->SetMinimumPaneSize (100);
	editorAndModelSplitter->SplitVertically (nodeEditorControl, modelControl, sashPosition);

	if (!defaultFileName.empty ()) {
		OpenFile (defaultFileName);
	}
}

MainWindow::~MainWindow ()
{
	
}

void MainWindow::OnCommand (wxCommandEvent& evt)
{
	CommandId commandId = (CommandId) evt.GetId ();
	ProcessCommand (commandId);
}

void MainWindow::OnClose (wxCloseEvent&)
{
	if (ConfirmLosingUnsavedChanges ()) {
		Destroy ();
	}
}

void MainWindow::ProcessCommand (CommandId commandId)
{
	WXAS::NodeEditorControl* editor = nodeEditorControl->GetEditor ();
	switch (commandId) {
		case File_New:
			{
				if (!ConfirmLosingUnsavedChanges ()) {
					break;
				}
				NewFile ();
			}
			break;
		case File_Open:
			{
				if (!ConfirmLosingUnsavedChanges ()) {
					break;
				}
				wxFileDialog fileDialog (this, L"Open", L"", L"", L"VisualScriptCAD files (*.vsc)|*.vsc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
				if (fileDialog.ShowModal () == wxID_OK) {
					std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
					OpenFile (fileName);
				}
			}
			break;
		case File_Save:
			{
				wxFileDialog fileDialog (this, L"Save", L"", L"", L"VisualScriptCAD files (*.vsc)|*.vsc", wxFD_SAVE);
				if (applicationState.HasCurrentFileName ()) {
					editor->Save (WideStringToNormalString (applicationState.GetCurrentFileName ()), &appHeaderIO);
				} else if (fileDialog.ShowModal () == wxID_OK) {
					std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
					editor->Save (WideStringToNormalString (fileName), &appHeaderIO);
					applicationState.SetCurrentFileName (fileName);
				}
			}
			break;
		case File_SaveAs:
			{
				wxFileDialog fileDialog (this, L"Save As", L"", L"", L"VisualScriptCAD files (*.vsc)|*.vsc", wxFD_SAVE);
				if (fileDialog.ShowModal () == wxID_OK) {
					std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
					editor->Save (WideStringToNormalString (fileName), &appHeaderIO);
					applicationState.SetCurrentFileName (fileName);
				}
			}
			break;
		case File_Exit:
			{
				Close (true);
			}
			break;
		case Edit_Undo:
			{
				editor->Undo ();
			}
			break;
		case Edit_Redo:
			{
				editor->Redo ();
			}
			break;
		case Mode_Automatic:
			{
				editor->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Automatic);
			}
			break;
		case Mode_Manual:
			{
				editor->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Manual);
			}
			break;
		case Mode_Update:
			{
				editor->ManualUpdate ();
			}
			break;
		case View_Editor:
			{
				editorAndModelSplitter->SplitVertically (nodeEditorControl, modelControl, sashPosition);
				sashPosition = editorAndModelSplitter->GetSashPosition ();
				editorAndModelSplitter->Unsplit (modelControl);
				viewMode = ViewMode::Editor;
			}
			break;
		case View_Model:
			{
				editorAndModelSplitter->SplitVertically (nodeEditorControl, modelControl, sashPosition);
				sashPosition = editorAndModelSplitter->GetSashPosition ();
				editorAndModelSplitter->Unsplit (nodeEditorControl);
				viewMode = ViewMode::Model;
			}
			break;
		case View_Split:
			{
				editorAndModelSplitter->SplitVertically (nodeEditorControl, modelControl, sashPosition);
				viewMode = ViewMode::Split;
			}
			break;
		case Editor_FitToWindow:
			{
				editor->FitToWindow ();
			}
			break;
		case Model_FitToWindow:
			{
				modelControl->FitToWindow ();
			}
			break;
		case Model_ViewMode_Lines:
			{
				RenderScene::Settings settings = modelControl->GetRenderSettings ();
				settings.viewMode = RenderScene::ViewMode::Lines;
				modelControl->SetRenderSettings (settings);
			}
			break;
		case Model_ViewMode_Polygons:
			{
				RenderScene::Settings settings = modelControl->GetRenderSettings ();
				settings.viewMode = RenderScene::ViewMode::Polygons;
				modelControl->SetRenderSettings (settings);
			}
			break;
		case Model_AxisMode_On:
			{
				RenderScene::Settings settings = modelControl->GetRenderSettings ();
				settings.axisMode = RenderScene::AxisMode::On;
				modelControl->SetRenderSettings (settings);
			}
			break;
		case Model_AxisMode_Off:
			{
				RenderScene::Settings settings = modelControl->GetRenderSettings ();
				settings.axisMode = RenderScene::AxisMode::Off;
				modelControl->SetRenderSettings (settings);
			}
			break;
		case Model_Info:
			{
				Modeler::ModelInfo modelInfo = model.GetModelInfo ();
				std::wstring modelInfoText = L"";
				modelInfoText += L"Mesh geometry count: " + std::to_wstring (modelInfo.meshGeometryCount) + L"\n";
				modelInfoText += L"Mesh count: " + std::to_wstring (modelInfo.meshCount) + L"\n";
				modelInfoText += L"Vertex count: " + std::to_wstring (modelInfo.vertexCount) + L"\n";
				modelInfoText += L"Triangle count: " + std::to_wstring (modelInfo.triangleCount) + L"\n";
				InfoDialog modelInfoDialog (this, L"Model Information", modelInfoText);
				modelInfoDialog.ShowModal ();
			}
			break;
		case Model_Export:
			{
				ExportDialog modelExportDialog (this, model);
				modelExportDialog.ShowModal ();
			}
			break;
		case About_GitHub:
			{
				wxLaunchDefaultBrowser ("https://github.com/kovacsv/VisualScriptCAD");
			}
			break;
	}
	UpdateMenuBar ();
	UpdateStatusBar ();
}

void MainWindow::OnKeyDown (wxKeyEvent& evt)
{
	wxChar unicodeKey = evt.GetUnicodeKey ();
	NUIE::ModifierKeys modifierKeys = WXAS::GetModiferKeysFromEvent (evt);
	switch (unicodeKey) {
		case L'N':
			{
				if (modifierKeys.Contains (NUIE::ModifierKeyCode::Control)) {
					ProcessCommand (CommandId::File_New);
				}
			}
			break;
		case L'O':
			{
				if (modifierKeys.Contains (NUIE::ModifierKeyCode::Control)) {
					ProcessCommand (CommandId::File_Open);
				}
			}
			break;
		case L'S':
			{
				if (modifierKeys.Contains (NUIE::ModifierKeyCode::Control) && modifierKeys.Contains (NUIE::ModifierKeyCode::Shift)) {
					ProcessCommand (CommandId::File_SaveAs);
				} else if (modifierKeys.Contains (NUIE::ModifierKeyCode::Control)) {
					ProcessCommand (CommandId::File_Save);
				}
			}
			break;
	}
	nodeEditorControl->OnKeyDown (evt);
}

bool MainWindow::ConfirmLosingUnsavedChanges ()
{
	WXAS::NodeEditorControl* editor = nodeEditorControl->GetEditor ();
	if (editor->NeedToSave ()) {
		wxMessageDialog confirmationDialog (this, L"You have made some unsaved changes.\nAre you sure you want to continue?", L"Unsaved changes", wxYES_NO);
		int result = confirmationDialog.ShowModal ();
		if (result != wxID_YES) {
			return false;
		}
	}
	return true;
}

void MainWindow::UpdateMenuBar ()
{
	WXAS::NodeEditorControl* editor = nodeEditorControl->GetEditor ();
 	menuBar->UpdateStatus (viewMode, editor->GetUpdateMode ());
	toolBar->UpdateStatus (viewMode, editor->GetUpdateMode ());
}

void MainWindow::UpdateStatusBar ()
{
	std::wstring currentFileText = L"No File";
	if (applicationState.HasCurrentFileName ()) {
		currentFileText = applicationState.GetCurrentFileName ();
	}
	SetStatusText (currentFileText);
}

void MainWindow::NewFile ()
{
	nodeEditorControl->Clear ();
	modelControl->Clear ();
	model.Clear ();
	applicationState.ClearCurrentFileName ();
}

void MainWindow::OpenFile (const std::wstring& fileName)
{
	WXAS::NodeEditorControl* editor = nodeEditorControl->GetEditor ();
	NewFile ();
	bool success = false;
	try {
		success = editor->Open (WideStringToNormalString (fileName), &appHeaderIO);
	} catch (...) {
	}
	if (success) {
		applicationState.SetCurrentFileName (fileName);
		editor->FitToWindow ();
		modelControl->FitToWindow ();
	} else {
		wxMessageDialog messageDialog (this, L"Failed to open file.", L"Error!", wxICON_ERROR | wxOK);
		messageDialog.ShowModal ();
		NewFile ();
	}
}

BEGIN_EVENT_TABLE (MainWindow, wxFrame)
EVT_MENU (wxID_ANY, MainWindow::OnCommand)
EVT_CLOSE (MainWindow::OnClose)
EVT_CHAR_HOOK (MainWindow::OnKeyDown)
END_EVENT_TABLE ()
