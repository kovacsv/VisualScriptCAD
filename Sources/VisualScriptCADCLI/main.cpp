#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "NodeRegistry.hpp"
#include "CLICommand.hpp"
#include "OpenExportCommand.hpp"

#ifdef DEBUG
#pragma comment(lib, "NodeEngineDebug.lib")
#pragma comment(lib, "NodeUIEngineDebug.lib")
#pragma comment(lib, "BuiltInNodesDebug.lib")
#else
#pragma comment(lib, "NodeEngine.lib")
#pragma comment(lib, "NodeUIEngine.lib")
#pragma comment(lib, "BuiltInNodes.lib")
#endif

void PrintUsage ()
{
	std::wcout << "Usage: VisualScriptCADCLI <command> [parameters]" << std::endl;
}

int wmain (int argc, wchar_t* argv[])
{
	// make sure every node is statically initialized
	GetNodeRegistry ();

	_setmode (_fileno (stdout), _O_WTEXT);
	if (argc < 2) {
		PrintUsage ();
		return 1;
	}

	CLI::CommandHandler commandHandler;
	commandHandler.RegisterCommand (CLI::CommandPtr (new OpenExportCommand ()));

	std::wstring commandName = argv[1];
	CLI::CommandPtr command = commandHandler.GetCommand (commandName);
	if (command == nullptr) {
		PrintUsage ();
		return 1;
	}

	std::vector<std::wstring> parameters;
	for (int i = 2; i < argc; i++) {
		parameters.push_back (argv[i]);
	}

	if (command->GetParameterCount () != parameters.size ()) {
		PrintUsage ();
		return 1;
	}

	if (!command->Do (parameters)) {
		return 1;
	}

	return 0;
}
