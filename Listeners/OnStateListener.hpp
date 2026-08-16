#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnStateListener(Player* pPlayer, GameUpdatePacket* pPacket)
	{
		g_lastCall = "Listeners::OnStateListener";
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

		pPlayer->ToggleTempBit(PLAYERTEMPFLAG_FACING_LEFT, pPacket->flags & NET_GAME_PACKET_FLAG_FACING_LEFT);

		int destX = (int)(pPacket->vecX / 32.f);
		int destY = (int)(pPacket->vecY / 32.f);
		if (destX < 0 || destX >= pWorld->GetTileMap()->GetWidth() || destY < 0 || destY >= pWorld->GetTileMap()->GetHeight())
		{
			return;
		}

		if (pPacket->speedOut < -800.f || pPacket->speedOut > 800.f || pPacket->speedIn < -850.f || pPacket->speedIn > 850.f) 
		{
			pPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetPosition() }, pAvatar->GetNetID());
			return;
		}

		if (!pWorld->CanWalkThatWay(pPlayer, pPacket, true))
		{
			return;
		}

		if (pPacket->flags & NET_GAME_PACKET_FLAG_ON_FIRE_DAMAGE || pPacket->flags & NET_GAME_PACKET_FLAG_ON_DAMAGE || pPacket->flags & NET_GAME_PACKET_FLAG_ON_ACID_DAMAGE)
		{
			if (std::chrono::steady_clock::now() - pPlayer->GetRegenTick() >= std::chrono::seconds(3)) 
			{
				pPlayer->SetHealth(200);
				pPlayer->OnRegenTick();
			}
			else 
			{
				pPlayer->OnRegenTick();
				pPlayer->SetHealth(pPlayer->GetHealth() - 40);
				if (pPlayer->GetHealth() < 1)
				{
					pPlayer->SetHealth(1);
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
			}
		}

		CL_Vec2i startPos = { static_cast<int>((pAvatar->GetPosition().X + 10) / 32), static_cast<int>((pAvatar->GetPosition().Y + 15) / 32) };
		CL_Vec2i goalPos = { static_cast<int>((pPacket->vecX + 10) / 32), static_cast<int>((pPacket->vecY + 15) / 32) };

		if (std::abs(goalPos.X - startPos.X) >= 8 || std::abs(startPos.Y - goalPos.Y) >= 8 || std::abs(goalPos.X - startPos.X) <= -8 || std::abs(startPos.Y - goalPos.Y) <= -8) 
		{
			pPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetPosition() }, pAvatar->GetNetID());
			return;
		}

		pPacket->netID = pAvatar->GetNetID();
		pAvatar->SetPosition({ pPacket->vecX, pPacket->vecY });
		
		pWorld->Broadcast([&](int netID, Player* pTargetPlayer) 
		{
			pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
		});
	}
} // namespace Listeners