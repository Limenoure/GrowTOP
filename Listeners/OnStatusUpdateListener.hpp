#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnStatusUpdateListener(Player* pPlayer, GameUpdatePacket* pPacket)
	{
		g_lastCall = "Listeners::OnStatusUpdateListener";
		if (!pPlayer || !pPacket || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INWORLD))
		{
			return;
		}

		World* pWorld = GetWorldsManager()->GetWorldByName(pPlayer->GetWorld()->GetName());
		if (!pWorld)
		{
			return;
		}

		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		pPacket->netID = pAvatar->GetNetID();
		pWorld->Broadcast([&](int netID, Player* pTargetPlayer) 
		{
				pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
		});
	}
	
} // namespace Listeners