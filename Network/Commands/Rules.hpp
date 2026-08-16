#pragma once
#include <string>

#include <Network/CommandsHandler.hpp>


void RulesCommand(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	pPlayer->SendVariantPacket({ "OnDialogRequest", GetBaseApp()->GetRules() });
}