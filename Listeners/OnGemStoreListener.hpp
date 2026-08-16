#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnGemStoreListener(Player* pPlayer, const std::string& textData)
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

		GetStoreManager()->ShowStore(pPlayer, STORETAB_GLOBAL_OFFERS);
	}
}