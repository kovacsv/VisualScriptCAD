#include "OpenExportCommand.hpp"

#include "NUIE_NodeEditor.hpp"
#include "Export.hpp"
#include "ModelEvaluationData.hpp"
#include "ApplicationHeaderIO.hpp"

#include "CLIEnvironment.hpp"

#include <fstream>

class WindowsFileIO : public NUIE::ExternalFileIO
{
public:
	virtual bool ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const override
	{
		std::ifstream file;
		file.open (fileName, std::ios::binary);
		if (DBGERROR (!file.is_open ())) {
			return false;
		}

		buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
		file.close ();

		return true;
	}

	virtual bool WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const override
	{
		std::ofstream file;
		file.open (fileName, std::ios::binary);
		if (DBGERROR (!file.is_open ())) {
			return false;
		}

		file.write (buffer.data (), buffer.size ());
		file.close ();

		return true;
	}
};

OpenExportCommand::OpenExportCommand () :
	CLI::Command (L"open_export_obj", 3)
{
}

bool OpenExportCommand::Do (const std::vector<std::wstring>& parameters) const
{
	std::wstring vscFileName = parameters[0];
	std::wstring objFileFolder = parameters[1];
	std::wstring objFileName = parameters[2];

	std::shared_ptr<ModelEvaluationData> evalData (new ModelEvaluationData ());
	CLI::NodeUIEnvironment env (evalData);
	NUIE::NodeEditor nodeEditor (env);

	WindowsFileIO fileIO;
	ApplicationHeaderIO headerIO;

	if (!nodeEditor.Open (vscFileName, &fileIO, &headerIO)) {
		return false;
	}
	
	//Modeler::ExportModel (evalData->GetModel (), Modeler::FormatId::Obj, objFileName);

	return true;
}
