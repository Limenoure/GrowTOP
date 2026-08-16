#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnQuitToExitListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnQuitToExitListener";
		if (!pPlayer || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME) || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INWORLD))
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		GetWorldsManager()->ExitWorld(pPlayer, pWorld);
	}
	
} // namespace Listeners