#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "SimpleTest.hpp"
#include "Export.hpp"

class ModelWriterForTest : public Modeler::ModelWriter
{
public:
	ModelWriterForTest ();

	virtual void	OpenFile (const std::wstring& fileName) override;
	virtual void	CloseFile () override;
	virtual void	WriteLine (const std::wstring& text) override;

	std::wstring	result;
};

bool CheckString (const std::wstring& expected, const std::wstring& result);

#endif
