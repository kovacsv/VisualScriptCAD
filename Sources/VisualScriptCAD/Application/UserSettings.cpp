#include "UserSettings.hpp"
#include "tinyxml2.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#ifdef _WIN32
	#include <windows.h>
#endif

static const size_t MaxRecentFileNumber = 10;

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
static const char RecentFilesNodeName[] = "RecentFiles";
static const char RecentFileNodeName[] = "RecentFile";

static const XmlEnum<ViewMode> ViewModeEnum ("ViewMode", {
	{ ViewMode::Lines, "Lines" },
	{ ViewMode::Polygons, "Polygons" }
});

static const XmlEnum<AxisMode> AxisModeEnum ("AxisMode", {
	{ AxisMode::On, "On" },
	{ AxisMode::Off, "Off" }
});

RenderSettings::RenderSettings (ViewMode viewMode, AxisMode axisMode) :
	viewMode (viewMode),
	axisMode (axisMode)
{

}

UserSettings::UserSettings () :
	renderSettings (ViewMode::Polygons, AxisMode::Off)
{
	wxFileName xmlFileName (wxStandardPaths::Get ().GetTempDir (), UserSettingsFileName);
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

	ViewModeEnum.Read (settingsNode, &renderSettings.viewMode);
	AxisModeEnum.Read (settingsNode, &renderSettings.axisMode);

#ifdef _WIN32
	const tinyxml2::XMLNode* recentFilesNode = settingsNode->FirstChildElement (RecentFilesNodeName);
	if (recentFilesNode != nullptr) {
		const tinyxml2::XMLElement* recentFileNode = recentFilesNode->FirstChildElement (RecentFileNodeName);
		while (recentFileNode != nullptr) {
			std::string recentFile = recentFileNode->GetText ();
			wchar_t wideCharString[2048];
			int size = MultiByteToWideChar (CP_UTF8, 0, recentFile.c_str (), recentFile.length (), wideCharString, 2048);
			wideCharString[size] = 0;
			recentFiles.push_back (wideCharString);
			recentFileNode = recentFileNode->NextSiblingElement (RecentFileNodeName);
		}
	}
#endif
}

void UserSettings::Save ()
{
	tinyxml2::XMLDocument doc;
	doc.InsertEndChild (doc.NewDeclaration ());

	tinyxml2::XMLNode* settingsNode = doc.NewElement (SettingsNodeName);
	doc.InsertEndChild (settingsNode);

	ViewModeEnum.Write (doc, settingsNode, renderSettings.viewMode);
	AxisModeEnum.Write (doc, settingsNode, renderSettings.axisMode);

#ifdef _WIN32
	tinyxml2::XMLElement* recentFilesNode = doc.NewElement (RecentFilesNodeName);
	for (const std::wstring& recentFile : recentFiles) {
		tinyxml2::XMLElement* recentFileNode = doc.NewElement (RecentFileNodeName);
		char multiByteString[2048];
		int size = WideCharToMultiByte (CP_UTF8, 0, recentFile.c_str (), recentFile.length (), multiByteString, 2048, NULL, NULL);
		multiByteString[size] = 0;
		recentFileNode->SetText (multiByteString);
		recentFilesNode->InsertEndChild (recentFileNode);
	}
	settingsNode->InsertEndChild (recentFilesNode);
#endif

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
