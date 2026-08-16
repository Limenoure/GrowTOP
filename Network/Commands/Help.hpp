#pragma once
#include <string>

#include <Network/CommandsHandler.hpp>


void HelpCommand(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	if (head.args.size() >= 1)
	{
		std::string command = head.args[0];
		std::transform(command.begin(), command.end(), command.begin(), ::tolower);
		std::string matching = "";

		for (int i = 0; i < GetCommandsHandler()->GetCommands().size(); i++)
		{
			Command* pCommand = GetCommandsHandler()->GetCommands().at(i);
			if (!pCommand)
			{
				continue;
			}

			std::string name = pCommand->GetName();
			std::vector<std::string> aliases = pCommand->GetAliases();
			if (name != command && std::find(aliases.begin(), aliases.end(), command) == aliases.end())
			{

				if (command.starts_with(name))
				{
					matching = name;
				}

				continue;
			}

			if (pPlayer->GetRole() < pCommand->GetPermission())
			{
				pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You can't check what this command does, because you don't have it unlocked!");
				return;
			}

			if (pCommand->GetName() == "hidestatus" && !pPlayer->HasBit(PLAYERFLAG_SUPER_SUPPORTER))
			{
				pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You can't check what this command does, because you don't have it unlocked!");
				return;
			}

			pPlayer->SendVariantPacket({ "OnConsoleMessage", pCommand->GetDescription() });
			return;
		}

		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|Did you mean to type /help " + matching + "?");
		return;
	}

	std::string reply = ">> Commands: ";
	for (int i = 0; i < GetCommandsHandler()->GetCommands().size(); i++)
	{
		Command* pCommand = GetCommandsHandler()->GetCommands().at(i);
		if (!pCommand)
		{
			continue;
		}

		if (pCommand->GetName() == "hidestatus" && !pPlayer->HasBit(PLAYERFLAG_SUPER_SUPPORTER))
		{
			continue;
		}

		if (pPlayer->GetRole() >= pCommand->GetPermission())
		{
			reply.append(" /" + pCommand->GetName());
		}
	}

	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + reply);
}