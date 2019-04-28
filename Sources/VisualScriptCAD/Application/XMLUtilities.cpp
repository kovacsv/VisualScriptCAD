#include "XMLUtilities.hpp"

#include <locale>
#include <codecvt>

std::wstring NormalStringToWideString (const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes (str);
}

std::string WideStringToNormalString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes (str);
}

bool ReadStringNode (const tinyxml2::XMLNode* parent, const char* nodeName, std::string& text)
{
	const tinyxml2::XMLElement* node = parent->FirstChildElement (nodeName);
	while (node == nullptr) {
		return false;
	}
	text = node->GetText ();
	return true;
}

void WriteStringNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, const std::string& text)
{
	tinyxml2::XMLElement* node = doc.NewElement (nodeName);
	node->SetText (text.c_str ());
	parent->InsertEndChild (node);
}

bool ReadStringNode (const tinyxml2::XMLNode* parent, const char* nodeName, std::wstring& text)
{
	const tinyxml2::XMLElement* node = parent->FirstChildElement (nodeName);
	while (node == nullptr) {
		return false;
	}
	text = NormalStringToWideString (node->GetText ());
	return true;
}

void WriteStringNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, const std::wstring& text)
{
	tinyxml2::XMLElement* node = doc.NewElement (nodeName);
	node->SetText (WideStringToNormalString (text).c_str ());
	parent->InsertEndChild (node);
}

bool ReadIntegerNode (const tinyxml2::XMLNode* parent, const char* nodeName, int& value)
{
	std::wstring nodeValue;
	if (!ReadStringNode (parent, nodeName, nodeValue)) {
		return false;
	}
	value = std::stoi (nodeValue);
	return true;
}

void WriteIntegerNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, int value)
{
	WriteStringNode (doc, parent, nodeName, std::to_wstring (value));
}

bool ReadDoubleNode (const tinyxml2::XMLNode* parent, const char* nodeName, double& value)
{
	std::wstring nodeValue;
	if (!ReadStringNode (parent, nodeName, nodeValue)) {
		return false;
	}
	value = std::stod (nodeValue);
	return true;
}

void WriteDoubleNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, double value)
{
	WriteStringNode (doc, parent, nodeName, std::to_wstring (value));
}

bool ReadBooleanNode (const tinyxml2::XMLNode* parent, const char* nodeName, bool& value)
{
	std::wstring nodeValue;
	if (!ReadStringNode (parent, nodeName, nodeValue)) {
		return false;
	}
	value = (nodeValue == L"true" ? true : false);
	return true;
}

void WriteBooleanNode (tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* parent, const char* nodeName, bool value)
{
	WriteStringNode (doc, parent, nodeName, value ? L"true" : L"false");
}
