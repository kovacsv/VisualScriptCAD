#include "OpenExportCommand.hpp"

#include "NUIE_NodeEditor.hpp"
#include "Export.hpp"
#include "ModelEvaluationData.hpp"
#include "ApplicationHeaderIO.hpp"

#include "CLIEnvironment.hpp"

#include <fstream>

class FileIO : public NUIE::ExternalFileIO
{
public:
	virtual bool ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const override
	{
		std::ifstream file;
		file.open (fileName, std::ios::binary);
		if (!file.is_open ()) {
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
		if (!file.is_open ()) {
			return false;
		}

		file.write (buffer.data (), buffer.size ());
		file.close ();

		return true;
	}
};

class ModelWriter : public Modeler::ModelWriter
{
public:
	ModelWriter (const std::wstring& folder) :
		folder (folder)
	{
		
	}

	virtual void OpenFile (const std::wstring& fileName) override
	{
		if (file.is_open ()) {
			return;
		}
		file.open (folder + L"\\" + fileName);
	}

	virtual void CloseFile () override
	{
		file.close ();
	}

	virtual void WriteLine (const std::wstring& text) override
	{
		file << text << std::endl;
	}

private:
	std::wstring	folder;
	std::wofstream	file;
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

	FileIO fileIO;
	ApplicationHeaderIO headerIO;

	if (!nodeEditor.Open (vscFileName, &fileIO, &headerIO)) {
		return false;
	}
	
	ModelWriter writer (objFileFolder);
	if (!Modeler::ExportModel (evalData->GetModel (), Modeler::FormatId::Obj, objFileName, writer)) {
		return false;
	}

	return true;
}
