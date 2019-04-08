#include "TestUtils.hpp"

ModelWriterForTest::ModelWriterForTest () :
	result ()
{
}

void ModelWriterForTest::OpenFile (const std::wstring& fileName)
{
	result += L"# open file " + fileName + L"\n";
}

void ModelWriterForTest::CloseFile ()
{
	result += L"# close file\n";
}

void ModelWriterForTest::WriteLine (const std::wstring& text)
{
	result += text + L"\n";
}

bool CheckString (const std::wstring& expected, const std::wstring& result)
{
	if (result == expected) {
		return true;
	} else {
		std::wcout << std::endl << expected;
		std::wcout << std::endl << result << std::endl;
		return false;
	}
}
