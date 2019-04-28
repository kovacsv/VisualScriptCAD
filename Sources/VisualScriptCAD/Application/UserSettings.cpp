#include "UserSettings.hpp"
#include "XMLUtilities.hpp"
#include "tinyxml2.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

static const size_t MaxRecentFileNumber = 10;

static std::string GetXmlFilePath ()
{
	static const std::string UserSettingsFileName ("VisualScriptCADSettings.xml");
	wxFileName xmlFileName (wxStandardPaths::Get ().GetUserDataDir (), UserSettingsFileName);
	wxString xmlFileDir = xmlFileName.GetPath ();
	if (!wxDir::Exists (xmlFileDir)) {
		wxDir::Make (xmlFileDir);
	}
	return xmlFileName.GetFullPath ().ToStdString ();
}

static const char SettingsNodeName[] = "VisualScriptCADSettings";
static const char RenderSettingsNodeName[] = "RenderSettings";
static const char RenderGridSizeNodeName[] = "GridSize";
static const char RenderGridCountNodeName[] = "GridCount";
static const char ExportSettingsNodeName[] = "ExportSettings";
static const char ImageSettingsNodeName[] = "ImageSettings";
static const char ImageWidthNodeName[] = "ImageWidth";
static const char ImageHeightNodeName[] = "ImageHeight";
static const char MultisamplingNodeName[] = "ImageMultisampling";
static const char ExportFolderNodeName[] = "ModelFolder";
static const char ExportModelNodeName[] = "ModelName";
static const char RecentFilesNodeName[] = "RecentFiles";
static const char RecentFileNodeName[] = "RecentFile";
static const char IsMaximizedNodeName[] = "IsMaximized";

static const XmlEnum<ViewMode> ViewModeEnum ("ViewMode", {
	{ ViewMode::Lines, "Lines" },
	{ ViewMode::Polygons, "Polygons" }
});

static const XmlEnum<AxisMode> AxisModeEnum ("AxisMode", {
	{ AxisMode::On, "On" },
	{ AxisMode::Off, "Off" }
});

static const XmlEnum<ExportSettings::FormatId> ExportFormatEnum ("ExportFormat", {
	{ ExportSettings::FormatId::Obj, "Obj" },
	{ ExportSettings::FormatId::Stl, "Stl" },
	{ ExportSettings::FormatId::Off, "Off" },
	{ ExportSettings::FormatId::Png, "Png" },
});

RenderSettings::RenderSettings () :
	RenderSettings (ViewMode::Polygons, AxisMode::On, 1.0, 20)
{

}

RenderSettings::RenderSettings (ViewMode viewMode, AxisMode axisMode, double gridSize, int gridCount) :
	viewMode (viewMode),
	axisMode (axisMode),
	gridSize (gridSize),
	gridCount (gridCount)
{

}

ImageSettings::ImageSettings () :
	ImageSettings (800, 600, 4)
{

}

ImageSettings::ImageSettings (int width, int height, int multisampling) :
	width (width),
	height (height),
	multisampling (multisampling)
{
}

bool ImageSettings::IsValid () const
{
	// TODO: strange memory errors in case of smaller images
	return width >= 50 && height >= 50 && multisampling > 0;
}

ExportSettings::ExportSettings () :
	ExportSettings (ExportSettings::FormatId::Obj, wxStandardPaths::Get ().GetUserDir (wxStandardPathsBase::Dir_Desktop).ToStdWstring (), L"model")
{
}

ExportSettings::ExportSettings (FormatId format, const std::wstring& folder, const std::wstring& name) :
	format (format),
	image (),
	folder (folder),
	name (name)
{
}

UserSettings::UserSettings () :
	renderSettings (),
	exportSettings (),
	isMaximized (true)
{

}

void UserSettings::Load ()
{
	std::string xmlFilePath = GetXmlFilePath ();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile (xmlFilePath.c_str ());
	if (error != tinyxml2::XMLError::XML_SUCCESS) {
		return;
	}

	const tinyxml2::XMLNode* settingsNode = doc.FirstChildElement (SettingsNodeName);
	if (settingsNode == nullptr) {
		return;
	}

	const tinyxml2::XMLNode* renderSettingsNode = settingsNode->FirstChildElement (RenderSettingsNodeName);
	if (renderSettingsNode != nullptr) {
		ViewModeEnum.Read (renderSettingsNode, renderSettings.viewMode);
		AxisModeEnum.Read (renderSettingsNode, renderSettings.axisMode);
		ReadDoubleNode (renderSettingsNode, RenderGridSizeNodeName, renderSettings.gridSize);
		ReadIntegerNode (renderSettingsNode, RenderGridCountNodeName, renderSettings.gridCount);
	}

	const tinyxml2::XMLNode* exportSettingsNode = settingsNode->FirstChildElement (ExportSettingsNodeName);
	if (exportSettingsNode != nullptr) {
		ExportFormatEnum.Read (exportSettingsNode, exportSettings.format);
		const tinyxml2::XMLNode* imageSettingsNode = exportSettingsNode->FirstChildElement (ImageSettingsNodeName);
		if (imageSettingsNode != nullptr) {
			ReadIntegerNode (imageSettingsNode, ImageWidthNodeName, exportSettings.image.width);
			ReadIntegerNode (imageSettingsNode, ImageHeightNodeName, exportSettings.image.height);
			ReadIntegerNode (imageSettingsNode, MultisamplingNodeName, exportSettings.image.multisampling);
		}
		ReadStringNode (exportSettingsNode, ExportFolderNodeName, exportSettings.folder);
		ReadStringNode (exportSettingsNode, ExportModelNodeName, exportSettings.name);
	}

	const tinyxml2::XMLNode* recentFilesNode = settingsNode->FirstChildElement (RecentFilesNodeName);
	if (recentFilesNode != nullptr) {
		const tinyxml2::XMLElement* recentFileNode = recentFilesNode->FirstChildElement (RecentFileNodeName);
		while (recentFileNode != nullptr) {
			std::string recentFile = recentFileNode->GetText ();
			recentFiles.push_back (NormalStringToWideString (recentFile));
			recentFileNode = recentFileNode->NextSiblingElement (RecentFileNodeName);
		}
	}

	ReadBooleanNode (settingsNode, IsMaximizedNodeName, isMaximized);
}

void UserSettings::Save ()
{
	tinyxml2::XMLDocument doc;
	doc.InsertEndChild (doc.NewDeclaration ());

	tinyxml2::XMLNode* settingsNode = doc.NewElement (SettingsNodeName);
	doc.InsertEndChild (settingsNode);

	tinyxml2::XMLNode* renderSettingsNode = doc.NewElement (RenderSettingsNodeName);
	ViewModeEnum.Write (doc, renderSettingsNode, renderSettings.viewMode);
	AxisModeEnum.Write (doc, renderSettingsNode, renderSettings.axisMode);
	WriteDoubleNode (doc, renderSettingsNode, RenderGridSizeNodeName, renderSettings.gridSize);
	WriteIntegerNode (doc, renderSettingsNode, RenderGridCountNodeName, renderSettings.gridCount);
	settingsNode->InsertEndChild (renderSettingsNode);

	tinyxml2::XMLNode* exportSettingsNode = doc.NewElement (ExportSettingsNodeName);
	ExportFormatEnum.Write (doc, exportSettingsNode, exportSettings.format);
	tinyxml2::XMLNode* imageSettingsNode = doc.NewElement (ImageSettingsNodeName);
	WriteIntegerNode (doc, imageSettingsNode, ImageWidthNodeName, exportSettings.image.width);
	WriteIntegerNode (doc, imageSettingsNode, ImageHeightNodeName, exportSettings.image.height);
	WriteIntegerNode (doc, imageSettingsNode, MultisamplingNodeName, exportSettings.image.multisampling);
	exportSettingsNode->InsertEndChild (imageSettingsNode);
	WriteStringNode (doc, exportSettingsNode, ExportFolderNodeName, exportSettings.folder);
	WriteStringNode (doc, exportSettingsNode, ExportModelNodeName, exportSettings.name);
	settingsNode->InsertEndChild (exportSettingsNode);

	tinyxml2::XMLElement* recentFilesNode = doc.NewElement (RecentFilesNodeName);
	for (const std::wstring& recentFile : recentFiles) {
		tinyxml2::XMLElement* recentFileNode = doc.NewElement (RecentFileNodeName);
		recentFileNode->SetText (WideStringToNormalString (recentFile).c_str ());
		recentFilesNode->InsertEndChild (recentFileNode);
	}
	settingsNode->InsertEndChild (recentFilesNode);

	WriteBooleanNode (doc, settingsNode, IsMaximizedNodeName, isMaximized);

	std::string xmlFilePath = GetXmlFilePath ();
	doc.SaveFile (xmlFilePath.c_str ());
}

void UserSettings::AddRecentFile (const std::wstring& filePath)
{
	auto found = std::find (recentFiles.begin (), recentFiles.end (), filePath);
	if (found != recentFiles.end ()) {
		recentFiles.erase (found);
	}

	recentFiles.push_back (filePath);
	if (recentFiles.size () > MaxRecentFileNumber) {
		recentFiles.erase (recentFiles.begin ());
	}
}

void UserSettings::RemoveRecentFile (const std::wstring& filePath)
{
	auto found = std::find (recentFiles.begin (), recentFiles.end (), filePath);
	if (found != recentFiles.end ()) {
		recentFiles.erase (found);
	}
}
