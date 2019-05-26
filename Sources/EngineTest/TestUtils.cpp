#include "TestUtils.hpp"
#include "Geometry.hpp"

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

bool IsEqualVec (const glm::dvec3& a, const glm::dvec3& b)
{
	return Geometry::IsEqual (a.x, b.x) && Geometry::IsEqual (a.y, b.y) && Geometry::IsEqual (a.z, b.z);
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
