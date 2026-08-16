#ifndef NETWORK__COMMANDSHANDLER__HPP
#define NETWORK__COMMANDSHANDLER__HPP

#include <string>
#include <vector>

#include <Network/Command.hpp>
#include <Network/CommandHeader.hpp>

class CommandsHandler
{
public:
	CommandsHandler() = default;
	~CommandsHandler();

	std::vector<Command*>& GetCommands() { return m_commands; }

	void Init();
	bool ProccessSlashCommand(CommandHeader& head);

private:
	std::vector<Command*> m_commands;

};

CommandsHandler* GetCommandsHandler();


//commands
void HelpCommand(const CommandHeader& head);
void CheatCommand(const CommandHeader& head);
void HideStatusCommand(const CommandHeader& head);
void MakeMeASupporter101Command(const CommandHeader& head);
void MakeMeASuperSupporter101Command(const CommandHeader& head);
void RulesCommand(const CommandHeader& head);
void MakeMeAsMod101(const CommandHeader& head);
#endif // NETWORK__COMMANDSHANDLER__HPP