#include "CLICommand.hpp"

namespace CLI
{

Command::Command (const std::wstring& commandName, int parameterCount) :
	commandName (commandName),
	parameterCount (parameterCount)
{

}

Command::~Command ()
{
}

const std::wstring& Command::GetName () const
{
	return commandName;
}

int Command::GetParameterCount () const
{
	return parameterCount;
}

CommandHandler::CommandHandler ()
{
}

bool CommandHandler::RegisterCommand (const CommandPtr& command)
{
	if (GetCommand (command->GetName ()) != nullptr) {
		return false;
	}
	commands.push_back (command);
	return true;
}

CommandPtr CommandHandler::GetCommand (const std::wstring& name) const
{
	for (const CommandPtr& command : commands) {
		if (command->GetName () == name) {
			return command;
		}
	}
	return nullptr;
}

}
