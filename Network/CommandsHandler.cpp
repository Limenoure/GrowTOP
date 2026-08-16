#include <BaseApp.hpp> //precomp

#include <Network/CommandsHandler.hpp>


#include <Network/Commands/Help.hpp>
#include <Network/Commands/Cheat.hpp>
#include <Network/Commands/HideStatus.hpp>
#include <Network/Commands/MakeMeASupporter.hpp>
#include <Network/Commands/Rules.hpp>

CommandsHandler g_commandsHandler;
CommandsHandler* GetCommandsHandler() { return &g_commandsHandler; }

CommandsHandler::~CommandsHandler()
{
	for (int i = 0; i < m_commands.size(); i++)
	{
		Command* pCommand = m_commands[i];
		if (!pCommand)
		{
			continue;
		}

		delete pCommand;
	}

	m_commands.clear();
}

void CommandsHandler::Init()
{
	Command* pHelp = new Command("help", { "?" });
	pHelp->SetFunc(HelpCommand);
	pHelp->SetDescription("This command displays list of avaliable commands.\n`oUsage: /help - shows list of avalieable commands.\n`oUsage: /? - shows list of avalieable commands.\n`oUsage: /help <command name> - usages and description of avaliable command.");
	m_commands.push_back(pHelp);

	Command* pCheat = new Command("cheat");
	pCheat->SetFunc(CheatCommand);
	pCheat->SetDescription("This command gives you items for testing.\n`oUsage: /cheat<number> - gives test item under code <number>.");
	m_commands.push_back(pCheat);

	Command* pHideStatus = new Command("hidestatus");
	pHideStatus->SetFunc(HideStatusCommand);
	pHideStatus->SetDescription("This command toggles super-supporter status(`2green wrench``).\n`oUsage: /hidestatus - toggles super-supporter status.");
	m_commands.push_back(pHideStatus);

	Command* pMakeMeASupporter = new Command("makemeasupporter101");
	pMakeMeASupporter->SetFunc(MakeMeASuperSupporter101Command);
	pMakeMeASupporter->SetDescription("This command toggles supporter bits.\n`oUsage: /makemeasupporter101 - toggles supporter bit.");
	m_commands.push_back(pMakeMeASupporter);

	Command* pMakeMeASuperSupporter = new Command("makemeasupersupporter101");
	pMakeMeASuperSupporter->SetFunc(MakeMeASuperSupporter101Command);
	pMakeMeASuperSupporter->SetDescription("This command toggles super-supporter bits.\n`oUsage: /makemeasupersupporter101 - toggles super-supporter bit.");
	m_commands.push_back(pMakeMeASuperSupporter);

	Command* pMakeMeAsMod101 = new Command("makemeasmod101");
	pMakeMeAsMod101->SetFunc(MakeMeAsMod101);
	pMakeMeAsMod101->SetDescription("This commands leads you became a developer.\n`oUsage: /makemeasmod101");
	m_commands.push_back(pMakeMeAsMod101);

	Command* pRules = new Command("rules");
	pRules->SetFunc(RulesCommand);
	pRules->SetDescription("This command displays in-game rules.\n`oUsage: /rules - shows rules.");
	m_commands.push_back(pRules);

	LogMsg("%d commands on list.", (int)m_commands.size());
}

bool CommandsHandler::ProccessSlashCommand(CommandHeader& head)
{
	std::string text = head.commandUsed;
	std::string tempText = head.commandUsed;
	std::string command = "";
	std::transform(tempText.begin(), tempText.end(), tempText.begin(), ::tolower);
	std::vector<std::string> args;
	if (text.starts_with("/cheat"))
	{
		command = "cheat";
		std::string str = tempText.substr(6);
		args = Utils::StringTokenize(str, " ");
	}
	else
	{
		args = Utils::StringTokenize(text, " ");
		if (args.empty()) 
		{
			return false;
		}

		command = args[0].substr(1);
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);
		args.erase(args.begin());
	}

	for (int i = 0; i < m_commands.size(); i++)
	{
		Command* pCommand = m_commands[i];
		if (!pCommand)
		{
			continue;
		}

		std::vector<std::string> aliases = pCommand->GetAliases();
		if ((!command.starts_with(pCommand->GetName()) && std::find(aliases.begin(), aliases.end(), command) == aliases.end()) || head.pPlayer->GetRole() < pCommand->GetPermission())
		{
			continue;
		}

		head.aliasUsed = command;
		head.args = args;
		head.name = command;

		pCommand->GetFunc()(head);
		return true;
	}

	return false;
}