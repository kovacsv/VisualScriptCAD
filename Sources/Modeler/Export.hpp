#ifndef MODELER_EXPORT_HPP
#define MODELER_EXPORT_HPP

#include "Model.hpp"

#include <string>

namespace Modeler
{

class ModelWriter
{
public:
	virtual void OpenFile (const std::wstring& fileName) = 0;
	virtual void CloseFile () = 0;
	virtual void WriteLine (const std::wstring& text) = 0;

	template<typename... Args>
	void WriteLine (const wchar_t* format, Args... args)
	{
		wchar_t buffer[1024];
		std::swprintf (buffer, 1024, format, args...);
		WriteLine (std::wstring (buffer));
	}
};

enum class FormatId
{
	Obj = 0,
	Stl = 1,
	Off = 2
};

void ExportModelToObj (const Model& model, const std::wstring& name, ModelWriter& writer);
void ExportModelToStl (const Model& model, const std::wstring& name, ModelWriter& writer);
void ExportModelToOff (const Model& model, const std::wstring& name, ModelWriter& writer);
bool ExportModel (const Model& model, FormatId formatId, const std::wstring& name, ModelWriter& writer);
bool ExportMesh (const Mesh& mesh, FormatId formatId, const std::wstring& name, ModelWriter& writer);

}

#endif
