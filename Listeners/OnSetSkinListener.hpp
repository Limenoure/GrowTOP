#pragma once

#include <string>
#include <iostream>

namespace Listeners 
{
	void OnSetSkinListener(Player* pPlayer, const std::string& textData)
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

		unsigned int color = std::stoul(t.GetParmString("color", 1).c_str());

		pPlayer->SetSkinColor(color);
		pWorld->Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendVariantPacket({ "OnChangeSkin", pPlayer->GetSkinColor() }, pAvatar->GetNetID()); });
	}
} // namespace Listeners