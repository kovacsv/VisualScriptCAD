#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "CLICommand.hpp"

void PrintUsage ()
{
	std::wcout << "Usage: VisualScriptCADCLI <command> [parameters]" << std::endl;
}

class OpenSaveCommand : public Command
{
public:
	OpenSaveCommand () :
		Command (L"open_save_obj", 1)
	{
	}

	virtual bool Do () const override
	{
		return true;
	}
};

int wmain (int argc, wchar_t* argv[])
{
	_setmode (_fileno (stdout), _O_WTEXT);
	if (argc < 2) {
		PrintUsage ();
		return 1;
	}

	CommandHandler commandHandler;
	commandHandler.RegisterCommand (CommandPtr (new OpenSaveCommand ()));

	std::wstring commandName = argv[1];
	CommandPtr command = commandHandler.GetCommand (commandName);
	if (command == nullptr) {
		PrintUsage ();
		return 1;
	}
	if (command->GetParameterCount () != argc - 2) {
		PrintUsage ();
		return 1;
	}

	if (!command->Do ()) {
		return 1;
	}
	return 0;
}
