#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnRespawnListener(Player* pPlayer)
	{
		if (!pPlayer)
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		if (!pWorld)
		{
			return;
		}

		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		pPlayer->SetHealth(200);
		pAvatar->SetPosition(pAvatar->GetRespawnPoint());

		pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
		{
			pTargetPlayer->SendVariantPacket({ "OnSetFreezeState", 2 }, pAvatar->GetNetID(), 0);
			pTargetPlayer->SendVariantPacket({ "OnKilled" }, pAvatar->GetNetID(), 0);
			pTargetPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetRespawnPoint() }, pAvatar->GetNetID(), 2000);
			pTargetPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/teleport.wav\ndelayMS|0");
			pTargetPlayer->SendVariantPacket({ "OnSetFreezeState", 0 }, pAvatar->GetNetID(), 2000);
		});
	}

	void OnRespawnSpikeListener(Player* pPlayer, const std::string& textData)
	{
		if (!pPlayer)
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		if (!pWorld)
		{
			return;
		}

		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textData.c_str());

		int tileX = t.GetParmInt("tilex", 1);
		int tileY = t.GetParmInt("tiley", 1);

		pPlayer->SetHealth(200);
		pAvatar->SetPosition(pAvatar->GetRespawnPoint());

		pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
		{
			pTargetPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetRespawnPoint() }, pAvatar->GetNetID(), 2000);
			pTargetPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/teleport.wav\ndelayMS|0");
			pTargetPlayer->SendVariantPacket({ "OnSetFreezeState", 0 }, pAvatar->GetNetID(), 2000);
		});
	}
}