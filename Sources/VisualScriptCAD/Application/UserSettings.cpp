#include "UserSettings.hpp"
#include "tinyxml2.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

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
}

void UserSettings::Save ()
{
	tinyxml2::XMLDocument doc;
	doc.InsertEndChild (doc.NewDeclaration ());

	tinyxml2::XMLNode* settingsNode = doc.NewElement (SettingsNodeName);
	doc.InsertEndChild (settingsNode);

	ViewModeEnum.Write (doc, settingsNode, renderSettings.viewMode);
	AxisModeEnum.Write (doc, settingsNode, renderSettings.axisMode);

	doc.SaveFile (xmlFilePath.c_str ());
	
}
