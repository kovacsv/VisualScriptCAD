#ifndef XMLUTILITIES_HPP
#define XMLUTILITIES_HPP

#include "NE_Stream.hpp"
#include "tinyxml2.h"

#include <string>
#include <vector>

std::wstring	NormalStringToWideString (const std::string& str);
std::string		WideStringToNormalString (const std::wstring& str);

bool			ReadStringNode (const tinyxml2::XMLNode* parent, const char* nodeName, std::string& text);
void			WriteStringNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, const std::string& text);

bool			ReadStringNode (const tinyxml2::XMLNode* parent, const char* nodeName, std::wstring& text);
void			WriteStringNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, const std::wstring& text);

bool			ReadIntegerNode (const tinyxml2::XMLNode* parent, const char* nodeName, int& value);
void			WriteIntegerNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, int value);

bool			ReadBooleanNode (const tinyxml2::XMLNode* parent, const char* nodeName, bool& value);
void			WriteBooleanNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, bool value);

template <typename EnumType>
class XmlEnum
{
public:
	XmlEnum (const std::string& tagName, const std::vector<std::pair<EnumType, std::string>> enumStrings) :
		tagName (tagName),
		enumStrings (enumStrings)
	{
	}

	void Read (const tinyxml2::XMLNode* parent, EnumType& result) const
	{
		const tinyxml2::XMLElement* element = parent->FirstChildElement (tagName.c_str ());
		if (element == nullptr) {
			return;
		}
		std::string enumString = element->GetText ();
		for (const auto& it : enumStrings) {
			if (it.second == enumString) {
				result = it.first;
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

#endif
