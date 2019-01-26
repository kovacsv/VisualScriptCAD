#include "UserSettings.hpp"
#include "tinyxml2.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

#include <locale>
#include <codecvt>

static const size_t MaxRecentFileNumber = 10;

static std::wstring NormalStringToWideString (const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes (str);
}

static std::string WideStringToNormalString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes (str);
}

static bool ReadStringNode (const tinyxml2::XMLNode* parent, const char* nodeName, std::wstring& text)
{
	const tinyxml2::XMLElement* node = parent->FirstChildElement (nodeName);
	while (node == nullptr) {
		return false;
	}
	text = NormalStringToWideString (node->GetText ());
	return true;
}

static void WriteStringNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, const std::wstring& text)
{
	tinyxml2::XMLElement* node = doc.NewElement (nodeName);
	node->SetText (WideStringToNormalString (text).c_str ());
	parent->InsertEndChild (node);
}

static bool ReadBooleanNode (const tinyxml2::XMLNode* parent, const char* nodeName, bool& value)
{
	std::wstring nodeValue;
	if (!ReadStringNode (parent, nodeName, nodeValue)) {
		return false;
	}
	value = (nodeValue == L"true" ? true : false);
	return true;
}

static void WriteBooleanNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, bool value)
{
	WriteStringNode (doc, parent, nodeName, value ? L"true" : L"false");
}

template <typename EnumType>
class XmlEnum
{
public:
	XmlEnum (const std::string& tagName, const std::vector<std::pair<EnumType, std::string>> enumStrings) :
		tagName (tagName),
		enumStrings (enumStrings)
	{
	}

	void Read (const tinyxml2::XMLNode* parent, EnumType* result) const
	{
		const tinyxml2::XMLElement* element = parent->FirstChildElement (tagName.c_str ());
		if (element == nullptr) {
			return;
		}
		std::string enumString = element->GetText ();
		for (const auto& it : enumStrings) {
			if (it.second == enumString) {
				*result = it.first;
				break;
			}
		}
	}

	void Write (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, EnumType enumValue) const
	{
		std::string enumString;
		for (const auto& it : enumStrings) {
			if (it.first == enumValue) {
				enumString = it.second;
				break;
			}
		}
		if (enumString.empty ()) {
			return;
		}
		tinyxml2::XMLElement* elem = doc.NewElement (tagName.c_str ());
		elem->SetText (enumString.c_str ());
		parent->InsertEndChild (elem);
	}

private:
	std::string										tagName;
	std::vector<std::pair<EnumType, std::string>>	enumStrings;
};

static const std::string UserSettingsFileName ("VisualScriptCADSettings.xml");
static const char SettingsNodeName[] = "VisualScriptCADSettings";
static const char RenderSettingsNodeName[] = "RenderSettings";
static const char ExportSettingsNodeName[] = "ExportSettings";
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

static const XmlEnum<Modeler::FormatId> ExportFormatEnum ("ModelFormat", {
	{ Modeler::FormatId::Obj, "Obj" },
	{ Modeler::FormatId::Stl, "Stl" },
	{ Modeler::FormatId::Off, "Off" }
});

RenderSettings::RenderSettings (ViewMode viewMode, AxisMode axisMode) :
	viewMode (viewMode),
	axisMode (axisMode)
{

}

ExportSettings::ExportSettings (Modeler::FormatId format, const std::wstring& folder, const std::wstring& name) :
	format (format),
	folder (folder),
	name (name)
{
}

UserSettings::UserSettings () :
	renderSettings (ViewMode::Polygons, AxisMode::Off),
	exportSettings (Modeler::FormatId::Obj, wxStandardPaths::Get ().GetUserDir (wxStandardPathsBase::Dir_Desktop).ToStdWstring (), L"model"),
	isMaximized (true)
{
	wxFileName xmlFileName (wxStandardPaths::Get ().GetUserDataDir (), UserSettingsFileName);
	wxString xmlFileDir = xmlFileName.GetPath ();
	if (!wxDir::Exists (xmlFileDir)) {
		wxDir::Make (xmlFileDir);
	}
	xmlFilePath = xmlFileName.GetFullPath ();
}

void UserSettings::Load ()
{
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
		ViewModeEnum.Read (renderSettingsNode, &renderSettings.viewMode);
		AxisModeEnum.Read (renderSettingsNode, &renderSettings.axisMode);
	}

	const tinyxml2::XMLNode* exportSettingsNode = settingsNode->FirstChildElement (ExportSettingsNodeName);
	if (exportSettingsNode != nullptr) {
		ExportFormatEnum.Read (exportSettingsNode, &exportSettings.format);
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
	settingsNode->InsertEndChild (renderSettingsNode);

	tinyxml2::XMLNode* exportSettingsNode = doc.NewElement (ExportSettingsNodeName);
	ExportFormatEnum.Write (doc, exportSettingsNode, exportSettings.format);
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

	doc.SaveFile (xmlFilePath.c_str ());
}

void UserSettings::AddRecentFile (const std::wstring& filePath)
{
	auto found = std::find (recentFiles.begin (), recentFiles.end (), filePath);
	if (found != recentFiles.end ()) {
		return;
	}

	recentFiles.push_back (filePath);
	if (recentFiles.size () > MaxRecentFileNumber) {
		recentFiles.erase (recentFiles.begin ());
	}
}
