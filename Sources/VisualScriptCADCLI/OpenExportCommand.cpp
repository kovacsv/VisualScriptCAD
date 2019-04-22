#include "OpenExportCommand.hpp"

#include "NUIE_NodeEditor.hpp"
#include "ModelEvaluationData.hpp"
#include "ApplicationHeaderIO.hpp"


#include "CLIEnvironment.hpp"

OpenExportCommand::OpenExportCommand () :
	CLI::Command (L"open_export_obj", 1)
{
}

bool OpenExportCommand::Do () const
{
	std::shared_ptr<ModelEvaluationData> evalData (new ModelEvaluationData ());
	CLI::NodeUIEnvironment env (evalData);
	NUIE::NodeEditor nodeEditor (env);

	// TODO: location and FileIO
	ApplicationHeaderIO headerIO;
	if (!nodeEditor.Open (L"", nullptr, &headerIO)) {
		return false;
	}

	
	return true;
}
