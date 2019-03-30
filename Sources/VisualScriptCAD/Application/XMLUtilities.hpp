#ifndef XMLUTILITIES_HPP
#define XMLUTILITIES_HPP

#include "NE_Stream.hpp"
#include "tinyxml2.h"

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

#endif
