#ifndef CLICOMMAND_HPP
#define CLICOMMAND_HPP

#include <memory>
#include <vector>

namespace CLI
{

class Command
{
public:
	Command (const std::wstring& commandName, int parameterCount);
	virtual ~Command ();

	const std::wstring&		GetName () const;
	int						GetParameterCount () const;
	virtual bool			Do (const std::vector<std::wstring>& parameters) const = 0;

private:
	std::wstring	commandName;
	int				parameterCount;

};

typedef std::shared_ptr<Command> CommandPtr;

class CommandHandler
{
public:
	CommandHandler ();

	bool		RegisterCommand (const CommandPtr& command);
	CommandPtr	GetCommand (const std::wstring& name) const;

private:
	std::vector<CommandPtr> commands;
};

}

#endif
