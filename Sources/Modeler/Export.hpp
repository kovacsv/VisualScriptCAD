#ifndef MODELER_EXPORT_HPP
#define MODELER_EXPORT_HPP

#include "Model.hpp"

#include <string>

namespace Modeler
{

class ModelWriter
{
public:
	virtual void OpenFile (const std::string& fileName) = 0;
	virtual void CloseFile () = 0;
	virtual void WriteLine (const std::string& text) = 0;

	template<typename... Args>
	void WriteLine (const char* format, Args... args)
	{
		char buffer[1024];
		std::snprintf (buffer, 1024, format, args...);
		WriteLine (std::string (buffer));
	}
};

enum class FormatId
{
	Obj,
	Stl,
	Off
};

void ExportModelToObj (const Model& model, const std::string& name, ModelWriter& writer);
void ExportModelToStl (const Model& model, const std::string& name, ModelWriter& writer);
void ExportModelToOff (const Model& model, const std::string& name, ModelWriter& writer);
bool ExportModel (const Model& model, FormatId formatId, const std::string& folderPath, const std::string& modelName);

}

#endif
