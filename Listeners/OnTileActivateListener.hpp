#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnTileActivateListener(Player* pPlayer, GameUpdatePacket* pPacket)
	{
		g_lastCall = "Listeners::OnTileActivateListener";
		if (!pPlayer || !pPacket || !pPlayer->GetWorld())
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		Tile* pTile = pWorld->GetTileMap()->GetTile((int)pPacket->intX, (int)pPacket->intY);
		if (!pTile)
		{
			return;
		}

		ItemInfo* pItem = pTile->GetItem();
		if (!pItem)
		{
			return;
		}

		switch (pItem->type)
		{
			case TYPE_MAIN_DOOR:
			{
				GetWorldsManager()->ExitWorld(pPlayer, pWorld, true);
			} break;

			case TYPE_CHECKPOINT: 
			{
				CL_Vec2f pos = { (float)(pPacket->intX) * 32.f, (float)(pPacket->intY) * 32.f };
				if (pAvatar->GetRespawnPoint() != pos)
				{
					pAvatar->SetRespawnPoint(pos);
				}

				pPlayer->SendVariantPacket({ "SetRespawnPos", pPacket->intX + (pPacket->intY * pWorld->GetTileMap()->GetWidth()) }, pAvatar->GetNetID());
			} break;

			case TYPE_DOOR: case TYPE_PORTAL: 
			{
				TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				std::string target = pExtra->m_destination;
				std::string id = pExtra->m_id;

				auto SendFail = [&](const std::string& msg = "") 
				{
					pPlayer->SendVariantPacket({ "OnSetFreezeState", 1 }, pAvatar->GetNetID(), 0);
					if (!msg.empty())
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), msg, 0, 1 });
					}

					pPlayer->SendVariantPacket({ "OnSetFreezeState", 0 }, pAvatar->GetNetID(), 0);
				};

				bool bIsLocked = pTile->IsLocked();
				bool bIsOwner = pTile->IsOwner(pPlayer);
				bool bIsAdmin = pTile->IsAdmin(pPlayer);
				if (pExtra->m_flags == 8 && (bIsLocked && !bIsOwner && !bIsAdmin))
				{
					SendFail("The door is locked!");
					return;
				}

				LogMsg("using door");
				if (target.empty()) 
				{
					LogMsg("no target");
					SendFail();
					pAvatar->SetPosition(pWorld->GetTileMap()->GetSpawnPoint());
					pPlayer->SendVariantPacket({ "OnSetPos", pWorld->GetTileMap()->GetSpawnPoint() }, pAvatar->GetNetID());
					return;
				}

				if (target.find(":") != std::string::npos) 
				{
					target = target.substr(0, target.find(':'));
					id = target.substr(target.find(':') + 1);
					LogMsg("target: %s | id: %s", target.c_str(), id.c_str());
				}

				World* pTargetWorld = nullptr;
				if (!target.empty() && target != pWorld->GetName())
				{
					pTargetWorld = GetWorldsManager()->GetWorldByName(target);
					if (!pTargetWorld)
					{
						SendFail("Something went wrong...");
						return;
					}
				}

				bool bFoundItem = false;
				int teleportX = -1;
				int teleportY = -1;

				if (!id.empty())
				{
					if (target == pWorld->GetName())
					{
						for (int i = 0; i < pWorld->GetTileMap()->GetTiles().size(); i++)
						{
							Tile* pTile = &pWorld->GetTileMap()->GetTiles().at(i);
							if (!pTile || !pTile->GetItem() || (pTile->GetForeground() != ITEM_ID_PATH_MARKER && pTile->GetItem()->type != TYPE_DOOR && pTile->GetItem()->type != TYPE_PORTAL))
							{
								continue;
							}

							if (pTile->GetForeground() == ITEM_ID_PATH_MARKER)
							{
								TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(pTile->GetTileExtra());
								if (!pExtra || pExtra->m_id != id)
								{
									continue;
								}

								bFoundItem = true;
								teleportX = pTile->GetIndex() % pWorld->GetTileMap()->GetWidth();
								teleportY = pTile->GetIndex() / pWorld->GetTileMap()->GetWidth();
								break;
							}

							if (pTile->GetItem()->type == TYPE_DOOR || pTile->GetItem()->type == TYPE_PORTAL)
							{
								TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(pTile->GetTileExtra());
								if (!pExtra || pExtra->m_id != id)
								{
									continue;
								}

								bFoundItem = true;
								teleportX = pTile->GetIndex() % pWorld->GetTileMap()->GetWidth();
								teleportY = pTile->GetIndex() / pWorld->GetTileMap()->GetWidth();
								break;
							}
						}
					}
					else
					{
						for (int i = 0; i < pTargetWorld->GetTileMap()->GetTiles().size(); i++)
						{
							Tile* pTile = &pTargetWorld->GetTileMap()->GetTiles().at(i);
							if (!pTile || !pTile->GetItem() || (pTile->GetForeground() != ITEM_ID_PATH_MARKER && pTile->GetItem()->type != TYPE_DOOR && pTile->GetItem()->type != TYPE_PORTAL))
							{
								continue;
							}

							if (pTile->GetForeground() == ITEM_ID_PATH_MARKER)
							{
								TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(pTile->GetTileExtra());
								if (!pExtra || pExtra->m_id != id)
								{
									continue;
								}

								bFoundItem = true;
								teleportX = pTile->GetIndex() % pTargetWorld->GetTileMap()->GetWidth();
								teleportY = pTile->GetIndex() / pTargetWorld->GetTileMap()->GetWidth();
								break;
							}

							if (pTile->GetItem()->type == TYPE_DOOR || pTile->GetItem()->type == TYPE_PORTAL)
							{
								TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(pTile->GetTileExtra());
								if (!pExtra || pExtra->m_id != id)
								{
									continue;
								}

								bFoundItem = true;
								teleportX = pTile->GetIndex() % pTargetWorld->GetTileMap()->GetWidth();
								teleportY = pTile->GetIndex() / pTargetWorld->GetTileMap()->GetWidth();
								break;
							}
						}
					}
				}

				LogMsg("checking");
				if (!bFoundItem)
				{
					if (!pTargetWorld)
					{
						SendFail();
						return;
					}

					CL_Vec2f spawnXY = { pWorld->GetTileMap()->GetSpawnPoint() };
					if (pWorld != pTargetWorld)
					{
						spawnXY = { pTargetWorld->GetTileMap()->GetSpawnPoint() };
						GetWorldsManager()->ExitWorld(pPlayer, pWorld, false);
						GetWorldsManager()->EnterWorld(pPlayer, pTargetWorld, spawnXY);
						return;
					}

					pAvatar->SetPosition(spawnXY);
					pPlayer->SendVariantPacket({ "OnSetPos", spawnXY }, pAvatar->GetNetID());
					return;
				}
				else
				{
					if (!pTargetWorld)
					{
						SendFail();
						return;
					}

					CL_Vec2f spawnXY = { (float)teleportX * 32.f, (float)teleportY * 32.f };
					if (pWorld != pTargetWorld)
					{
						GetWorldsManager()->ExitWorld(pPlayer, pWorld, false);
						GetWorldsManager()->EnterWorld(pPlayer, pTargetWorld, spawnXY);
						return;
					}

					pAvatar->SetPosition(spawnXY);
					pPlayer->SendVariantPacket({ "OnSetPos", spawnXY }, pAvatar->GetNetID());
					return;
				}

			} break;
		}
	}

} // namespace Listeners