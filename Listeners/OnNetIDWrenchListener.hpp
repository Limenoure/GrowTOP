#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnNetIDWrenchListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnNetIDWrenchListener";
		if (!pPlayer || !pPlayer->GetWorld())
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		int netID = std::atoi(textData.substr(textData.find("|netid|") + 7).c_str());
		Player* pTargetPlayer = nullptr;
		for (int i = 0; i < pWorld->GetNetObjectManager()->GetObjects().size(); i++)
		{
			NetAvatar* pTargetAvatar = pWorld->GetNetObjectManager()->GetObjects().at(i);
			if (!pTargetAvatar || pTargetAvatar->GetNetID() != netID)
			{
				continue;
			}

			pTargetPlayer = pTargetAvatar->GetPlayer();
		}

		pPlayer->SendWrench(netID, pTargetPlayer);
	}
	
} // namespace Listeners