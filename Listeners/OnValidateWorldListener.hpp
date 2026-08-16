#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnValidateWorldListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::ValidateWorldListener";
		if (!pPlayer || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME) || pPlayer->HasTempBit(PLAYERTEMPFLAG_INWORLD))
			return;

		TextScanner t;
		t.SetupFromMemoryAddress(textData.c_str());

		std::string worldName = t.GetParmString("name", 1);
		GetWorldsManager()->ValidateWorld(pPlayer, worldName);
	}
	
} // namespace Listeners