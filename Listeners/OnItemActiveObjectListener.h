#pragma once

#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnItemActivateObjectListener(Player* pPlayer, GameUpdatePacket* pPacket)
	{
		if (!pPlayer || !pPacket)
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

		int destX = (int)(pPacket->vecX / 32.f);
		int destY = (int)(pPacket->vecY / 32.f);
		if (destX < 0 || destX >= pWorld->GetTileMap()->GetWidth() || destY < 0 || destY >= pWorld->GetTileMap()->GetHeight())
		{
			return;
		}

		if (std::abs(pAvatar->GetPosition().X - pPacket->vecX) > 128 || std::abs(pAvatar->GetPosition().Y - pPacket->vecY) > 128)
		{
			pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "(Too far away)", 0, 1 });
			return;
		}

		if (!pWorld->CanWalkThatWay(pPlayer, pPacket))
		{
			return;
		}

		pWorld->CollectObject(pAvatar->GetNetID(), pPacket->objectID, CL_Vec2f{ pPacket->vecX, pPacket->vecY });
	}
} // namespace Listeners