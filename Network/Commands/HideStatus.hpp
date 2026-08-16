#pragma once
#include <string>

#include <Network/CommandsHandler.hpp>
#include <Player/NetAvatar.hpp>
#include <World/World.hpp>

void HideStatusCommand(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	if (!pPlayer->HasBit(PLAYERFLAG_SUPER_SUPPORTER))
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	std::string msg = "Super Supporter status hidden!";
	if (pPlayer->GetCharacterState().HasStateFlag(STATEFLAG_SUPER_SUPPORTER))
	{
		pPlayer->GetCharacterState().ToggleStateFlag(STATEFLAG_SUPER_SUPPORTER, false);
	}
	else
	{
		msg = "Super Supporter status visible!";
		pPlayer->GetCharacterState().ToggleStateFlag(STATEFLAG_SUPER_SUPPORTER, true);
	}

	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + msg);
	pAvatar->Update();
}