#pragma once
#include <string>

#include <BaseApp.hpp>
#include <Network/CommandsHandler.hpp>
#include <Player/NetAvatar.hpp>
#include <World/World.hpp>

void MakeMeASupporter101Command(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}
	
	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (!GetBaseApp()->GetConfig().bBeta)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
		return;
	}

	pPlayer->ToggleBit(PLAYERFLAG_SUPPORTER, true);
	pPlayer->SetSkinColor(MAKE_RGBA(255, 255, 255, 255));
	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You've been upped.");
	pPlayer->OnSetBux();
	pAvatar->Update();
}

void MakeMeASuperSupporter101Command(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (!GetBaseApp()->GetConfig().bBeta)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
		return;
	}

	pPlayer->ToggleBit(PLAYERFLAG_SUPER_SUPPORTER, true);
	pPlayer->GetCharacterState().ToggleStateFlag(STATEFLAG_SUPER_SUPPORTER, true);
	pPlayer->SetSkinColor(MAKE_RGBA(251, 0, 0, 130));
	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You've been upped.");
	pPlayer->OnSetBux();
	pAvatar->Update();
}

void MakeMeAsMod101(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (!GetBaseApp()->GetConfig().bBeta)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
		return;
	}

	pPlayer->ToggleBit(PLAYERFLAG_SUPER_SUPPORTER, true);
	pPlayer->GetCharacterState().ToggleStateFlag(STATEFLAG_SUPER_SUPPORTER, true);
	pPlayer->SendVariantPacket({ "OnNameChanged", "`6@" + pPlayer->GetDisplayName(pWorld) + "``", pPlayer->GetTitleIcon() }, pAvatar->GetNetID());
	pPlayer->SetSkinColor(MAKE_RGBA(251, 0, 0, 130));
	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You've been upped.");
	pPlayer->OnSetBux();
	pAvatar->Update();
}