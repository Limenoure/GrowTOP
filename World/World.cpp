#include <BaseApp.hpp> //precomp

#include <World/World.hpp>
#include <Player/Player.hpp>

#include <ProtonSDK/MiscUtils.h>

World::World(const std::string& name, const int& width /* = 100 */, const int& height /* = 60 */) : m_name(name)
{
	//deleting for safety
	if (m_pTilemap)
	{
		delete m_pTilemap;
	}
	if (m_pObjectmap)
	{
		delete m_pObjectmap;
	}
	if (m_pNetObjectManager)
	{
		delete m_pNetObjectManager;
	}

	m_pTilemap = new WorldTileMap(width, height);
	m_pObjectmap = new WorldObjectMap();
	m_pNetObjectManager = new NetObjectManager(this);
}

World::~World()
{
	if (m_pTilemap)
	{
		delete m_pTilemap;
	}
	if (m_pObjectmap)
	{
		delete m_pObjectmap;
	}
	if (m_pNetObjectManager)
	{
		delete m_pNetObjectManager;
	}
}


void World::Broadcast(std::function<void(int, Player*)> pair)
{
	if (!m_pNetObjectManager)
	{
		return;
	}

	for (int i = 0; i < m_pNetObjectManager->GetObjects().size(); i++)
	{
		NetAvatar* pAvatar = m_pNetObjectManager->GetObjects().at(i);
		if (!pAvatar || !pAvatar->GetPlayer())
		{
			continue;
		}

		pair(pAvatar->GetNetID(), pAvatar->GetPlayer());
	}
}

Tile* World::GetTile(const uint16_t& index)
{
	return m_pTilemap->GetTile(index);
}
Tile* World::GetTile(const uint16_t& x, const uint16_t& y)
{
	return m_pTilemap->GetTile(x, y);
}

Tile* World::GetTile(const float& fX, const float& fY)
{
	return m_pTilemap->GetTile(fX, fY);
}

Tile* World::GetTile(const CL_Vec2f& vec)
{
	return m_pTilemap->GetTile(vec);
}

Tile* World::GetTile(const CL_Vec2i& vec)
{
	return m_pTilemap->GetTile(vec);
}

std::string World::GetCategoryName()
{
	switch (m_category)
	{
	     case WORLDCATEGORY_ADVENTURE: { return "Adventure"; }
		 case WORLDCATEGORY_ART: { return "Art"; }
		 case WORLDCATEGORY_FARM: { return "Farm"; }
		 case WORLDCATEGORY_GAME: { return "Game"; }
		 case WORLDCATEGORY_GUILD: { return "Guild"; }
		 case WORLDCATEGORY_INFORMATION: { return "Information"; }
		 case WORLDCATEGORY_MUSIC: { return "Music"; }
		 case WORLDCATEGORY_PARKOUR: { return "Parkour"; }
		 case WORLDCATEGORY_PUZZLE: { return "Puzzle"; }
		 case WORLDCATEGORY_ROLEPLAY: { return "Roleplay"; }
		 case WORLDCATEGORY_SHOP: { return "Shop"; }
		 case WORLDCATEGORY_SOCIAL: { return "Social"; }
		 case WORLDCATEGORY_STORAGE: { return "Storage"; }
		 case WORLDCATEGORY_STORY: { return "Story"; }
		 case WORLDCATEGORY_TRADE: { return "Trade"; }
	}

	return "None";
}
size_t World::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t); //world/map version
	estimated += sizeof(int); //world bits/flags
	estimated += sizeof(uint16_t) + m_name.length();

	estimated += m_pTilemap->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
	if (bPacket && fGameVersion >= 4.31f)
	{
		//unknown values
		estimated += sizeof(int);
		estimated += sizeof(int);
		estimated += sizeof(int);
	}

	estimated += m_pObjectmap->GetMemoryEstimated();
	estimated += sizeof(int) * 2; //weathers

	return estimated;
}

void World::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_version, pMem, offset, true);
	MemorySerialize(&m_bits, pMem, offset, true);
	MemorySerialize(m_name, pMem, offset, true);

	m_pTilemap->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
	if (bPacket && fGameVersion >= 4.31f) 
	{
		int zero = 0;
		MemorySerialize(&zero, pMem, offset, true);
		MemorySerialize(&zero, pMem, offset, true);
		MemorySerialize(&zero, pMem, offset, true);
	}

	m_pObjectmap->SerializeToMem(pMem, offset, bPacket);

	MemorySerialize(&m_weather, pMem, offset, true);
	MemorySerialize(&m_baseWeather, pMem, offset, true);
}

std::string World::GetSize(const size_t& size)
{
	const double KB = 1024.0;
	const double MB = KB * 1024.0;
	const double GB = MB * 1024.0;
	const double TB = GB * 1024.0;

	size_t actualSize = size;
	std::ostringstream oss;

	std::fixed(oss);
	oss << std::setprecision(2);

	if (actualSize >= TB)
	{
		oss << actualSize / TB << "tb, ";
		actualSize %= static_cast<size_t>(TB);
	}

	if (actualSize >= GB) 
	{
		oss << actualSize / GB << "gb, ";
		actualSize %= static_cast<size_t>(GB);
	}

	if (actualSize >= MB) 
	{
		oss << actualSize / MB << "mb, ";
		actualSize %= static_cast<size_t>(MB);
	}

	if (actualSize >= KB) 
	{
		oss << actualSize / KB << "kb";
		actualSize %= static_cast<size_t>(KB);
	}

	return oss.str();
}

int World::GetPlayersInWorld()
{
	if (!m_pNetObjectManager)
	{
		return 0;
	}

	return m_pNetObjectManager->GetCount();
}

void World::HandlePacketTileChangeRequest(Player* pPlayer, GameUpdatePacket* pPacket)
{
	if (!pPlayer || !pPacket)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pPacket->intData);
	if (!pItem)
	{
		return;
	}

	switch (pItem->type) 
	{
	    case TYPE_FIST: 
		     HandlePacketTileChangePunch(pPlayer, pPacket);
	         break;

	    case TYPE_WRENCH:
		    HandlePacketTileChangeWrench(pPlayer, pPacket);
			break;

	    default: 
		    HandlePacketTileChangePlace(pPlayer, pPacket);
	        break;
	}
}

void World::HandlePacketTileChangePunch(Player* pPlayer, GameUpdatePacket* pPacket)
{
	if (!pPlayer || !pPacket)
	{
		return;
	}

	CL_Vec2f interactedAt = CL_Vec2f{ (float)pPacket->intX, (float)pPacket->intY };
	Tile* pTile = m_pTilemap->GetTile(interactedAt);
	if (!pTile)
	{
		return;
	}

	ItemInfo* pItem = pTile->GetItem();
	if (!pItem)
	{
		return;
	}

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (pItem->ID == 0 || pPlayer->GetHitPower() == 0)
	{
		return;
	}

	if (std::chrono::steady_clock::now() - pTile->GetTick() >= std::chrono::seconds(pItem->regenTime)) 
	{
		pTile->SetDamage(0);
		pTile->SetTick(std::chrono::steady_clock::now());
	}

	pTile->SetTick(std::chrono::steady_clock::now());

	switch (pItem->type)
	{
		case TYPE_SWITCHEROO: case TYPE_SWITCHEROO2: case TYPE_BOOMBOX: case TYPE_BACK_BOOMBOX: case TYPE_CHEST: case TYPE_DEADLY_IF_ON: 
		{
			if (pItem->ID == ITEM_ID_ANGRY_ADVENTURE_GORILLA)
			{
				break;
			}

			pTile->ToggleFlag(TILEFLAG_ENABLED, !pTile->HasFlag(TILEFLAG_ENABLED));
	    } break;

		case TYPE_DICE:
		{
			TileExtraRoll* pExtra = dynamic_cast<TileExtraRoll*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				break;
			}

			uint8_t roll = (uint8_t)Randomizer::Get(0, 6);
			pPacket->roll = roll;
			pExtra->m_roll = roll;
		} break;

	    case TYPE_SEED:
		{
			TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				break;
			}

			if (std::chrono::steady_clock::now() - pExtra->m_growingSince < std::chrono::seconds(pItem->bloomTime))
			{
				break;
			}

			OnTreeSmashSpawnThings(pTile, pTile->GetItem(), false, pAvatar->GetNetID());
		} break;

		case TYPE_ITEM_SUCKER: 
		{
			TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				break;
			}
			
			if (pTile->GetForeground() == ITEM_ID_MAGPLANT_5000 && pTile->GetDamage() == 0) 
			{
				if (pExtra->m_itemID == ITEM_ID_BLANK) 
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "Cannot activate the MAGPLANT 5000, you need to set the item first." });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Cannot activate the MAGPLANT 5000, you need to set the item first.", 0, 1 });
					break;
				}

				ItemInfo* pDisplayItem = GetItemInfoManager()->GetItem(pExtra->m_itemID);
				if (!pDisplayItem)
				{
					break;
				}

				if (pDisplayItem->type == TYPE_CONSUMABLE || pDisplayItem->type == TYPE_LOCK || pDisplayItem->type == TYPE_CLOTHES) 
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "Cannot activate the MAGPLANT 5000, that item can not be placed!" });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Cannot activate the MAGPLANT 5000, that item can not be placed!", 0, 1 });
					break;
				}

				pExtra->m_bBuildingMode = !pExtra->m_bBuildingMode;
				SendTileUpdate(pTile);
			}
		} break;
	}

	pPacket->type = NET_GAME_PACKET_TILE_APPLY_DAMAGE;
	pPacket->netID = pAvatar->GetNetID();
	pPacket->tileDamage = pPlayer->GetHitPower();
	pTile->SetDamage(pTile->GetDamage() + pPlayer->GetHitPower());

	if (pTile->GetDamage() >= pItem->hardness) 
	{
		pPacket->type = NET_GAME_PACKET_TILE_CHANGE_REQUEST;
		pPacket->intData = ITEM_ID_FIST;

		if (pItem->editableTypes & AUTOPICKUP && pPlayer->GetInventoryItemCount(pItem->ID) + 1 > pItem->maxCount) 
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
			pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I better not break that, I have no room to pick it up!", 0, 1 });
			return;
		}

		switch (pItem->type) 
		{
		    case TYPE_CHECKPOINT:
		    {
			    for (int i = 0; i < m_pNetObjectManager->GetObjects().size(); i++)
				{
					NetAvatar* pTargetAvatar = m_pNetObjectManager->GetObjects().at(i);
					if (!pTargetAvatar)
					{
						continue;
					}

				    if (pTargetAvatar->GetRespawnPoint().X / 32.f == pPacket->intX && pTargetAvatar->GetRespawnPoint().Y / 32.f == pPacket->intY) 
					{
						pTargetAvatar->SetRespawnPoint(m_pTilemap->GetSpawnPoint());
						pTargetAvatar->GetPlayer()->SendVariantPacket({ "SetRespawnPos", m_pTilemap->GetSpawnPoint().X / 32.f + (m_pTilemap->GetSpawnPoint().Y / 32.f * m_pTilemap->GetWidth()) }, pTargetAvatar->GetNetID());
				    }
			    }
		    } break;

			case TYPE_LOCK:
			{
				TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					break;
				}

				m_pTilemap->RemoveAllTilesFromThisLock(pTile);
				if (pItem->lockPower == 0)
				{
					m_ownerID = -1;
					m_lockIndex = 0;
					Broadcast([&](int netID, Player* pTargetPlayer) 
	                { 
						pTargetPlayer->SendVariantPacket({ "OnNameChanged", pPlayer->GetDisplayName(this), pPlayer->GetTitleIcon() }, pAvatar->GetNetID());
						pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5[```w" + m_name + "`` has had its `$World Lock`` removed!`5]``"});
					});
				}
			} break;

			case TYPE_DISPLAY_BLOCK:
			{
				TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					break;
				}

				if (pExtra->m_itemID == ITEM_ID_BLANK)
				{
					break;
				}

				if (!pPlayer->AddInventoryItem(pExtra->m_itemID, 1, true))
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I better not break that, I have no room to pick it up!", 0, 1 });
					pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
					pTile->SetDamage(0);
					return;
				}
				
				GameUpdatePacket getItem;
				getItem.type = NET_GAME_PACKET_ITEM_EFFECT;
				getItem.animationType = 5;
				getItem.vecX = (float)(pPacket->intX * 32.f) + 16.f;
				getItem.vecY = (float)(pPacket->intY * 32.f) + 16.f;
				getItem.itemAltID = pExtra->m_itemID;
				getItem.targetNetID = pAvatar->GetNetID();
				getItem.itemCount = 1;

				Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &getItem, sizeof(GameUpdatePacket)); });
				pExtra->m_itemID = 0;
			} break;

			case TYPE_DONATION_BOX:
			{
				TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					break;
				}

				if (pExtra->m_donations.size() > 1)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Empty the box before breaking!", 0, 1 });
					pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
					pTile->SetDamage(0);
					return;
				}
			} break;

			case TYPE_ITEM_SUCKER: 
			{
				TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					break;
				}

				if (pExtra->m_itemID != ITEM_ID_BLANK && pExtra->m_count > 1) 
				{
					pTile->SetDamage(0);
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "Empty the machine before smashing!", });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Empty the machine before smashing!", 0, 1 });
					pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
					return;
				}

			} break;

		}

		if (pItem->editableTypes & AUTOPICKUP) 
		{
			pPlayer->AddInventoryItem(pTile->GetForeground(), 1, true);
			pPlayer->SendVariantPacket({ "OnConsoleMessage", std::format("Collected `w1 {}``.", pItem->name, pItem->rarity != 999 ? " Rarity: `w" + std::to_string(pItem->rarity) + "``." : "") });
		}

		OnSmashGoodie(pPlayer, pTile);

		if (pItem->rarity != 999) 
		{
			pAvatar->SendExperience(1 + (pItem->rarity / 5));
		}
		 
		if (!(pItem->editableTypes & AUTOPICKUP) && pItem->rarity != 999) 
		{
			bool bLucky = false;
			bool bBlock = false;
			bool bSeed = false;
			int gems = 0;

			OnBlockSmashSpawnThings(pItem, &bLucky, &bBlock, &bSeed,  &gems);
			float spawnX = pPacket->intX * 32.f;
			float spawnY = pPacket->intY * 32.f;

			if (bBlock && !GetBaseApp()->GetConfig().bOldFarmables)
			{
				WorldObject obj;
				obj.ID = pItem->ID;
				obj.x = spawnX;
				obj.y = spawnY;
				obj.count = 1;
				//TODO: other buffs

				AddObject(obj, true);
			}

			if (bSeed && !GetBaseApp()->GetConfig().bOldFarmables)
			{
				WorldObject obj;
				obj.ID = pItem->ID + 1;
				obj.x = spawnX;
				obj.y = spawnY;
				obj.count = 1;
				//TODO: other buffs

				AddObject(obj, true);
			}

			if (gems > 0) 
			{
				if (bLucky) 
				{
					if (Randomizer::Get(0, 100) < 98)
					{
						gems = gems / 3;
					}
					else
					{
						Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendVariantPacket({ "OnParticleEffect", 125, CL_Vec2f{ spawnX, spawnY }, 0.0f, 0.0f }); });
					}
				}

				if (GetBaseApp()->GetConfig().bOldFarmables)
				{
					pPlayer->GetItems()->SetGems(pPlayer->GetItems()->GetGems() + gems);
					pPlayer->OnSetBux();
				}
				else
				{
					SpawnGemsOnATile(gems, spawnX, spawnY);
				}
			}
		}

		pTile->SetDamage(0);
		pTile->ResetExtra();
		if (pItem->type == TYPE_BACKGROUND)
		{
			pTile->SetBackground(0);
		}
		else
		{
			pTile->SetForeground(0);
		}
	}

	Broadcast([&](int netID, Player* pTargetPlayer)
	{
		pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});
}

void World::HandlePacketTileChangePlace(Player* pPlayer, GameUpdatePacket* pPacket)
{
	if (!pPlayer || !pPacket)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pPacket->intData);
	if (!pItem)
	{
		return;
	}

	CL_Vec2f interactedAt = CL_Vec2f{ (float)pPacket->intX, (float)pPacket->intY };
	Tile* pTile = GetTile(interactedAt);
	if (!pTile)
	{
		return;
	}

	ItemInfo* pTileItem = GetItemInfoManager()->GetItem(pTile->GetForeground() == 0 ? pTile->GetBackground() : pTile->GetForeground());
	if (!pTileItem)
	{
		return;
	}

	if (pItem->type == TYPE_CONSUMABLE && pTileItem->type != TYPE_DISPLAY_BLOCK)
	{
		if (HasBit(WORLDBIT_NOCONSUME))
		{
			pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "(Not allowed to use consumables here!)", 0, 1 });
			return;
		}

		HandlePacketTileChangeConsume(pPlayer, pPacket);
		return;
	}

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
	{
		return;
	}

	if (pItem->flags & ONLY_FOR_WORLD_OWNER)
	{
		if (m_lockIndex == 0 || m_ownerID != pPlayer->GetUserID())
		{
			pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "This item can only be used in World-Locked.", 0, 1 });
			return;
		}
	}

	pPacket->netID = pAvatar->GetNetID();

	if (pTile->GetForeground() != 0) 
	{
		switch (pTileItem->type)
		{
		    case TYPE_DISPLAY_BLOCK:
			{
				TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					break;
				}

				if (std::chrono::steady_clock::now() - pExtra->m_placedAt < std::chrono::seconds(5)) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`7[`4The Display Block needs to warm up for 5 seconds before use!``]``", 0, 1 });
					return;
				} 

				if (pExtra->m_itemID != 0) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Remove what's in there first.", 0, 1 });
					return;
				} 
				
				if (pItem->type == TYPE_LOCK || pItem->type == TYPE_DISPLAY_BLOCK || pItem->ID == ITEM_ID_DISPLAY_BOX)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Sorry, no displaying Display Blocks or Locks.", 0, 1 });
					return;
				} 
				
				if (pItem->ID == ITEM_ID_WORLD_KEY || pItem->ID == ITEM_ID_GUILD_KEY) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "No no no.", 0, 1 });
					return;
				} 
				
				if (pItem->ID == ITEM_ID_SCREEN_DOOR) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Don't be a scammer.", 0, 1 });
					return;
				} 
				
				if (pItem->editableTypes & UNTRADEABLE) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't display untradeable items.", 0, 1 });
					return;
				}

				if (pPlayer->RemoveInventoryItem(pItem->ID, 1, true))
				{
					GameUpdatePacket throwItem;
					throwItem.type = NET_GAME_PACKET_ITEM_EFFECT;
					throwItem.vecX = (float)(pPacket->intX * 32.f) + 16.f;
					throwItem.vecY = (float)(pPacket->intY * 32.f) + 16.f;
					throwItem.particleID = 4;
					throwItem.itemAltID = pItem->ID;
					throwItem.targetNetID = pAvatar->GetNetID();
					throwItem.particleSizeAlt = 1;
					Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &throwItem, sizeof(GameUpdatePacket)); });

					pExtra->m_itemID = pItem->ID;
					SendTileUpdate(pTile);
				}

			} break;
			case TYPE_SEED: 
			{
				if (pTileItem->type != TYPE_SEED && pTileItem->ID != ITEM_ID_DISPLAY_BLOCK) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can only use seeds on blank tiles or existing trees.", 0, 1 });
					pPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/cant_place_tile.wav" });
					return;
				}
				
				if (pTileItem->type == TYPE_SEED) 
				{
					TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(pTile->GetTileExtra());
					if (!pExtra)
					{
						return;
					}

					if (std::chrono::steady_clock::now() - pExtra->m_growingSince >= std::chrono::seconds(pTileItem->bloomTime)) 
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "This tree is already too big to splice another seed with it.", 0, 1 });
						pPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/cant_place_tile.wav" });
						return;
					}

					if (pTile->HasFlag(TILEFLAG_SPLICED_TREE)) 
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "It would be too dangerous to try to mix three seeds.", 0, 1 });
						pPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/cant_place_tile.wav" });
						return;
					}

					uint16_t result = GetItemInfoManager()->GetSplicingInfo(pTileItem->ID, pItem->ID);
					ItemInfo* pResultItem = GetItemInfoManager()->GetItem(result - 1);
					if (result == -1 || !pResultItem) 
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), std::format("Hmm, it looks like `${}`` and `${}`` can't be spliced.", pItem->name, pTileItem->name), 0, 1 });
						pPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/cant_place_tile.wav" });
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true))
					{
						return;
					}

					pTile->ResetExtra();
					pTile->SetForeground(result);
					pTile->ToggleFlag(TILEFLAG_SPLICED_TREE, true);
					pExtra = dynamic_cast<TileExtraSeed*>(pTile->GetTileExtra());
					if (!pExtra)
					{
						break;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), std::format("{} and {} have been spliced to make a `${} Tree``!", pItem->name, pTileItem->name, pResultItem->name), 0, 1 });
					pPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/splash.wav" });
					SendTileUpdate(pTile);

					pPacket->fruits = pExtra->m_fruits;
					Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength); });
				}
			} return;
		    default:
			{
				break;
			}
		}

		if (pItem->type != TYPE_BACKGROUND)
		{
			return;
		}
	}

	if (pItem->editableTypes & FLIPPED && pPlayer->HasTempBit(PLAYERTEMPFLAG_FACING_LEFT))
	{
		//multifacing
		pTile->ToggleFlag(TILEFLAG_FLIPPED, true);
	}

	switch (pItem->type)
	{
	    case TYPE_CLOTHES: case TYPE_ANCES:
	    {
			int bodyPart = pItem->bodyPart;
			if (pItem->type == TYPE_ANCES)
			{
				bodyPart = 9;
			}

			if (pItem->ID != ITEM_ID_EXQUISITE_GAUNTLET_OF_ELEMENTS && pPlayer->GetItems()->GetCloth(HAND) == ITEM_ID_EXQUISITE_GAUNTLET_OF_ELEMENTS)
			{
				pPlayer->SendTileSelect();
			}

			pPlayer->GetItems()->SetCloth((eClothingType)bodyPart, pPlayer->GetItems()->GetCloth((eClothingType)bodyPart) == pItem->ID ? 0 : pItem->ID, true);
			pAvatar->Update();

	    } return;
		case TYPE_LOCK:
		{
			if (interactedAt.X == 0 && interactedAt.Y == 0 || interactedAt.X == m_pTilemap->GetWidth() && interactedAt.Y == m_pTilemap->GetHeight())
			{
				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Use a lock on a blank tile next to the things you want to lock.", 0, 1 });
				return;
			}

			if (pItem->lockPower == 0 && m_lockIndex != 0)
			{
				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Only one `$World Lock`` can be placed in a world, you'd have to remove the other one first.", 0, 1 });
				return;
			}

			pPlayer->RemoveInventoryItem(pItem->ID, 1, true);
			pTile->SetForeground(pItem->ID);

			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				break;
			}

			pExtra->m_ownerID = pPlayer->GetUserID();

			if (pItem->lockPower != 0)
			{
				m_pTilemap->AddTilesThisWouldLock(pTile, pItem->lockPower, false);
				SendLock(pPlayer, pTile);

				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Area locked.", 0, 1 });
				Broadcast([&](int netID, Player* pTargetPlayer) 
				{
					pTargetPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/use_lock.wav" });
				});

				return;
			}

			m_ownerID = pPlayer->GetUserID();
			m_lockIndex = pTile->GetIndex();

			Broadcast([&](int netID, Player* pTargetPlayer)
			{
				pTargetPlayer->SendVariantPacket({ "OnNameChanged", pPlayer->GetDisplayName(this), pPlayer->GetTitleIcon() }, pAvatar->GetNetID());
				pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5[```w" + m_name + "`` has been `$World Locked`` by `w" + pPlayer->GetName() + "```5]``" });
				pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`5[```w" + m_name + "`` has been `$World Locked`` by `w" + pPlayer->GetName() + "```5]``", 0, 1 });
				pTargetPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/use_lock.wav" });
			});

			for (int i = 0; i < m_pTilemap->GetTiles().size(); i++)
			{
				Tile* pTile = &m_pTilemap->GetTiles().at(i);
				if (!pTile)
				{
					continue;
				}

				pTile->SetLockIndex(pTile->GetIndex());
			}

			SendLock(pPlayer, pTile);
		} return;
		case TYPE_SEED:
		{
			pTile->SetForeground(pItem->ID);
			pPlayer->RemoveInventoryItem(pItem->ID, 1, false);

			if (pItem->ID == ITEM_ID_MAGIC_EGG)
			{
				//pTile->SetForeground(pItem->ID - 1);
				GameUpdatePacket particle;
				particle.type = NET_GAME_PACKET_SEND_PARTICLE_EFFECT;
				particle.netID = -1;
				particle.particleRotation = 10;
				particle.particleAltID = 66;
				particle.vecX = (float)(interactedAt.X * 32.f) + 16.f;
				particle.vecY = (float)(interactedAt.Y * 32.f) + 16.f;
				
				Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &particle, sizeof(GameUpdatePacket)); });
				SendTileUpdate(pTile);
				return;
			}

			TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				break;
			}

			pPacket->fruits = pExtra->m_fruits;
			Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength); });
		} return;
	}

	pPlayer->RemoveInventoryItem(pItem->ID, 1, false);

	if (pItem->type == TYPE_BACKGROUND)
	{
		pTile->SetBackground(pItem->ID);
	}
	else
	{
		pTile->SetForeground(pItem->ID);
	}

	Broadcast([&](int netID, Player* pTargetPlayer)
	{
		pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});
}

void World::HandlePacketTileChangeConsume(Player* pPlayer, GameUpdatePacket* pPacket)
{
	if (!pPlayer || !pPacket)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pPacket->intData);
	if (!pItem)
	{
		return;
	}

	CL_Vec2f interactedAt = CL_Vec2f{ (float)pPacket->intX, (float)pPacket->intY };
	Tile* pTile = GetTile(interactedAt);
	if (!pTile)
	{
		return;
	}

	ItemInfo* pTileItem = GetItemInfoManager()->GetItem(pTile->GetForeground() == 0 ? pTile->GetBackground() : pTile->GetForeground());
	if (!pTileItem)
	{
		return;
	}

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	NetAvatar* pTargetAvatar = nullptr;
	for (int i = 0; i < m_pNetObjectManager->GetObjects().size(); i++) 
	{
		NetAvatar* pTarget = m_pNetObjectManager->GetObjects().at(i);
		if (!pTarget || !(std::abs(pTarget->GetPosition().X - (interactedAt.X * 32)) <= 16 && std::abs(pTarget->GetPosition().Y - (interactedAt.Y * 32)) <= 16))
		{
			continue;
		}

		pTargetAvatar = pTarget;
	}

	auto SendMustBeUsedOnPerson = [&pAvatar, pPlayer]() { pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Must be used on a person.", 0, 1 }); };
	auto SendMustBeOnOtherPerson = [&pAvatar, pPlayer]() { pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Use that on somebody else!", 0, 1 }); };
	auto SendMustBeUsedOnYourself = [&pAvatar, pPlayer]() { pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can only use that on yourself.", 0, 1 }); };

	switch (pItem->ID) 
	{
	    case ITEM_ID_POPCORN: 
		{
		    if (!pTargetAvatar) 
			{
			     SendMustBeUsedOnPerson();
			     return;
		    }
		    if (pTargetAvatar != pAvatar) 
			{
		        pAvatar->OnTalkBubble(pAvatar->GetNetID(), "Too tasty to share.", pAvatar->GetNetID());
			    return;
		    }

		    if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true))
		    {
			    return;
		    }

		    GameUpdatePacket packet;
		    packet.type = NET_GAME_PACKET_NPC;
		    packet.npcType = 2;
		    packet.npcEvent = 5/*NPC_EVENT_BURP*/;
		    packet.intX = pAvatar->GetNetID();

		    Broadcast([&](int netID, Player* pTargetPlayer) 
			{
			    pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &packet, sizeof(GameUpdatePacket));
		    });

	    } return;
		case ITEM_ID_FREEZE_WAND:
		{
			if (!pTargetAvatar) {
				SendMustBeUsedOnPerson();
				return;
			}
			if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true)) {
				return;
			}
			GameUpdatePacket packet;
			packet.type = NET_GAME_PACKET_NPC;
			packet.npcType = 3; //not used
			packet.npcEvent = 3; //not used
			packet.intX = pAvatar->GetNetID();
			
			pAvatar->OnConsoleMessage("Freeze Wand used.");
			pAvatar->OnTalkBubble(pAvatar->GetNetID(), "Freeze Wand used.");
			Broadcast([&](int netID, Player* pTargetPlayer) {
				pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &packet, sizeof(GameUpdatePacket));
				});

		} return;
		case ITEM_ID_CAVE_BLAST: 
		case ITEM_ID_BEACH_BLAST: 
		case ITEM_ID_MONOCHROME_BLAST: 
		case ITEM_ID_MARS_BLAST: 
		case ITEM_ID_TREASURE_BLAST:
		case ITEM_ID_UNDERSEA_BLAST: 
		case ITEM_ID_BOUNTIFUL_BLAST: 
		case ITEM_ID_DESERT_BLAST: 
		{
			if (pItem->ID == ITEM_ID_BEACH_BLAST && pPlayer->GetInventoryItemCount(ITEM_ID_FIREWORKS) < 100) {
				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You need 100 Fireworks to launch!", 0, 1 });
				return;
			}

			std::string dialog = "set_default_color|`o\n";
			dialog += "add_label_with_icon|big|`w" + pItem->name + "``|left|" + std::to_string(pItem->ID) + "\n";
			dialog += "embed_data|itemID|" + std::to_string(pItem->ID) + "\n";
			dialog += "add_textbox|This item creates a new world! Enter a unique name for it.|\n";
			dialog += "add_text_input|world_name|New World Name||24|\n";
			dialog += "end_dialog|terraformer_reply|Cancel|`5Create!``|";

			pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });

		} return;

	    default:
	    {
			pAvatar->OnTalkBubble(pAvatar->GetNetID(), "This consumable isn't implemented, message developers to suggest it.", pAvatar->GetNetID());
			pAvatar->OnConsoleMessage("This consumable isn't implemented, message developers to suggest it.");
			return;
	    }
	}

	return;
}

void World::HandlePacketTileChangeWrench(Player* pPlayer, GameUpdatePacket* pPacket)
{
	if (!pPlayer || !pPacket)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pPacket->intData);
	if (!pItem)
	{
		return;
	}

	CL_Vec2f interactedAt = CL_Vec2f{ (float)pPacket->intX, (float)pPacket->intY };
	Tile* pTile = GetTile(interactedAt);
	if (!pTile)
	{
		return;
	}

	ItemInfo* pTileItem = GetItemInfoManager()->GetItem(pTile->GetForeground() == 0 ? pTile->GetBackground() : pTile->GetForeground());
	if (!pTileItem)
	{
		return;
	}

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	if (!(pTileItem->editableTypes & EDITABLE))
	{
		return;
	}

	if (!CanWrench(pTile, pPlayer))
	{
		pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I don't have access to this area.", 0, 1 });
		return;
	}

	switch (pTileItem->type)
	{
	    case TYPE_DOOR:
		{
			TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				return;
			}

			if (pItem->ID == ITEM_ID_PASSWORD_DOOR)
			{
				std::string dialog = "set_default_color|`o\n";
				dialog += "add_label_with_icon|big|`wEdit " + pItem->name + "``|left|" + std::to_string(pTileItem->ID) + "|\n";
				dialog += "add_text_input|door_name|Label|" + pExtra->m_label + "|100|\n";
				dialog += "add_text_input|door_target|Destination|" + pExtra->m_destination + "|24|\n";
				dialog += "add_smalltext|Enter a Destination in this format: `2WORLDNAME:ID``|left|\n";
				dialog += "add_smalltext|Leave `2WORLDNAME`` blank (:ID) to go to the door with `2ID`` in the `2Current World``.|left|\n";
				dialog += "add_text_input|door_id|Password|" + pExtra->m_password + "|23|\n";
				if (pTile->IsLocked())
				{
					dialog += std::format("add_checkbox|checkbox_locked|Is open to public|{}\n", (pExtra->m_flags == 0 ? "1" : "0"));
				}

				dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
				dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
				dialog += "end_dialog|door_edit|Cancel|OK|\n";

				pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
				return;
			}

			std::string dialog = "set_default_color|`o\n";
			dialog += "add_label_with_icon|big|`wEdit " + pItem->name + "``|left|" + std::to_string(pTileItem->ID) + "|\n";
			dialog += "add_text_input|door_name|Label|" + pExtra->m_label + "|100|\n";
			dialog += "add_text_input|door_target|Destination|" + pExtra->m_destination + "|24|\n";
			dialog += "add_smalltext|Enter a Destination in this format: `2WORLDNAME:ID``|left|\n";
			dialog += "add_smalltext|Leave `2WORLDNAME`` blank (:ID) to go to the door with `2ID`` in the `2Current World``.|left|\n";
			dialog += "add_text_input|door_id|ID|" + pExtra->m_id + "|23|\n";
			dialog += "add_smalltext|Set a unique `2ID`` to target this door as a Destination from another!|left|\n";
			if (pTile->IsLocked())
			{
				dialog += std::format("add_checkbox|checkbox_locked|Is open to public|{}\n", (pExtra->m_flags == 0 ? "1" : "0"));
			}

			dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
			dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
			dialog += "end_dialog|door_edit|Cancel|OK|\n";

			pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
		} break;

		case TYPE_SIGN:
		{
			TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				return;
			}

			if (pItem->ID == ITEM_ID_PATH_MARKER)
			{
				std::string dialog = "set_default_color|`o\n";
				dialog += std::format("add_label_with_icon|big|`wEdit {}``|left|{}|\n", pItem->name, pTileItem->ID);
				dialog += "add_textbox|Enter an ID. You can use this as a destination for Doors.``|left|\n";
				dialog += std::format("add_text_input|sign_text||{}|128|\n", pExtra->m_id);
				dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
				dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
				dialog += "end_dialog|sign_edit|Cancel|OK|\n";

				pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
				return;
			}

			std::string dialog = "set_default_color|`o\n";
			dialog += std::format("add_label_with_icon|big|`wEdit {}``|left|{}|\n", pItem->name, pTileItem->ID);
			dialog += "add_textbox|What would you like to write on this sign?``|left|\n";
			dialog += std::format("add_text_input|sign_text||{}|128|\n", pExtra->m_label);
			dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
			dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
			dialog += "end_dialog|sign_edit|Cancel|OK|\n";

			pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
		} break;

		case TYPE_DONATION_BOX:
		{
			TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				return;
			}

			int gifts = pExtra->GetGifts(pPlayer->GetUserID());
			int size = (int)pExtra->m_donations.size();
			std::string msg = "The box is currently empty.";
			if (size > 0)
			{
				msg = "The box contains `$" + std::to_string(size) + "`` gifts.";
			}

			if (!pTile->IsOwner(pPlayer) && pTile->IsLocked())
			{
				std::string dialog = "set_default_color|`o\n";
				dialog += "add_label_with_icon|big|`wEdit " + pTileItem->name + "``|left|" + std::to_string(pTileItem->ID) + "|\n";
				dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
				dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
				if (size < 20)
				{
					dialog += "add_textbox|" + msg + "|left|\n";
					if (gifts < 3)
					{
						dialog += "add_spacer|small|\n";
						dialog += "add_textbox|Want to leave a gift for the owner?|left|\n";
						dialog += "add_item_picker|itemid|`wGive Gift`` (Min rarity: `52``)|Choose an item to give|\n";
					}
					else
					{
						dialog += "add_textbox|You've already crammed `w3 ``of your gifts into the box, better wait.|left|\n";
					}
				}
				else
				{
					dialog += "add_textbox|This box already has `w20`` gifts in it.  Try again later.|left|\n";
				}

				dialog += "end_dialog|donation_box_edit|Cancel||\n";

				pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
				return;
			}

			std::string dialog = "set_default_color|`o\n";
			dialog += "add_label_with_icon|big|`wEdit " + pTileItem->name + "``|left|" + std::to_string(pTileItem->ID) + "|\n";
			dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
			dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
			if (!pExtra->m_donations.empty())
			{
				dialog += "add_textbox|You have `w" + std::to_string(size) + "`` gifts waiting:|left|\n";
				for (const auto& [ID, info] : pExtra->m_donations)
				{
					ItemInfo* pItem = GetItemInfoManager()->GetItem(info.itemID);
					if (!pItem)
					{
						continue;
					}

					std::string msg = "" + pItem->name + " (`w" + std::to_string(info.count) + "``) from `w" + info.lastName + "``";
					if (!info.message.empty())
					{
						msg.append("`#- \"" + info.message + "\"");
					}

					dialog += "add_checkbox|" + std::to_string(ID) + "|" + msg + "|0\n";
				}

				dialog += "add_spacer|small|\n";
				dialog += "add_button|clear|`4Retrieve All Gifts``|noflags|0|0|\n";
				
				if (size >= 20)
				{
					dialog += "add_textbox|This box already has `w20`` gifts in it.  Try again later.|left|\n";
				}
				else
				{
					dialog += "add_spacer|small|\n";
					dialog += "add_item_picker|itemid|`wGive Item`` (Min rarity: `52``)|Choose an item to give|\n";
					dialog += "add_item_picker|itemid|`wGive Gems`` (Min gems: `550``)|Choose an item to give|\n";
				}
			}
			else
			{
				dialog += "add_textbox|The box is currently empty.|left\n";
				dialog += "add_item_picker|itemid|`wGive Gift`` (Min rarity: `52``)|Choose an item to give|\n";
			}

			dialog += "end_dialog|donation_box_edit|Cancel||\n";
			pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
		} break;

		case TYPE_LOCK:
		{
			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				return;
			}

			bool bIsOwner = pExtra->m_ownerID == pPlayer->GetUserID();
			bool bIsAdmin = std::find(pExtra->m_admins.begin(), pExtra->m_admins.end(), pPlayer->GetUserID()) != pExtra->m_admins.end();
			if (bIsOwner)
			{
				std::string dialog = "set_default_color|`o\n";
				dialog += "add_label_with_icon|big|`wEdit " + pTileItem->name + "``|left|" + std::to_string(pTileItem->ID) + "\n";
				dialog += "add_label|small|`wAccess list:``|left\n";
				dialog += "add_spacer|small|\n";
				if (pExtra->m_admins.empty())
				{
					dialog += "add_label|small|Currently, you're the only one with access.|left|\n";
				}
				else
				{
					for (int i = 0; i < pExtra->m_admins.size(); i++)
			
					{
						int admin = pExtra->m_admins[i];
						bool bAdmin = std::find(pExtra->m_admins.begin(), pExtra->m_admins.end(), admin) != pExtra->m_admins.end();
						dialog += "add_checkbox|user_" + std::to_string(admin) + "|`wDeletedUser``|" + std::to_string((int)bAdmin) + "\n";
					}
				}
				if (pExtra->m_admins.size() < 25)
				{
					dialog += "add_player_picker|playerNetID|`wAdd``|\n";
					dialog += "add_spacer|small|\n";
				}

				dialog += std::format("add_checkbox|checkbox_public|Allow anyone to Build or Break|{}\n", pTile->HasFlag(TILEFLAG_PUBLIC) ? "1" : "0");
				if (pTileItem->lockPower == 0)
				{
					dialog += std::format("add_checkbox|checkbox_disable_music|Disable Custom Music Blocks|{}\n", pExtra->HasFlag(LOCKFLAG_DISABLE_MUSIC_NOTE) ? "1" : "0");
				    if (pExtra->m_tempo >= 20 && pExtra->m_tempo <= 200)
					{
						dialog += "add_text_input|tempo|Music BPM|" + std::to_string(pExtra->m_tempo) + "|3|\n";
					}

					dialog += "add_spacer|small|\n";
					dialog += std::format("add_checkbox|checkbox_disable_music_render|Make Custom Music Blocks invisible|{}\n", pExtra->HasFlag(LOCKFLAG_INVISIBLE_MUSIC_NOTE) ? "1" : "0");

					if (pPlayer->GetHome() != "EXIT")
					{
						dialog += "add_textbox|Your current home world is: " + pPlayer->GetHome() + "|left|\n";
					}
					dialog += std::format("add_checkbox|checkbox_home_world|Set as Home World|{}\n", pPlayer->GetHome() == m_name ? "1" : "0");

					dialog += "add_text_input|entry_level|World Level: |" + std::to_string(pExtra->m_entryLevel) + "|3|\n";
					dialog += "add_smalltext|Set minimum world entry level.|\n";

					if (pTileItem->ID == ITEM_ID_ROYAL_LOCK)
					{
						dialog += "add_spacer|small|\n";
						dialog += "add_textbox|Ye Royal Options|left|\n";
						dialog += std::format("add_checkbox|checkbox_silence|Silenced, Peasants!|{}\n", pExtra->HasFlag(LOCKFLAG_SILENCED_PLEASANTS) ? "1" : "0");
						dialog += std::format("add_checkbox|checkbox_rainbow|Rainbows For The King!|{}\n", pExtra->HasFlag(LOCKFLAG_ROYAL_RAINBOW) ? "1" : "0");
					}

					if (m_timer != 0)
					{
						dialog += "add_smalltext|Current World Timer set to: `2" + std::to_string(m_timer) + " minutes``.|\n";
					}

					dialog += "add_button|settimer|`wSet World Timer``|noflags|0|0|\n";
					dialog += "add_button|catchange|`wCategory: " + GetCategoryName() + "``|noflags|0|0|\n";
					if (pPlayer->GetInventoryItemCount(ITEM_ID_WORLD_KEY) < 1)
					{
						dialog += "add_button|getkey|`$Get World Key``|noflags|0|0|\n";
					}
				}
				else
				{
					dialog += std::format("add_checkbox|checkbox_ignore|Ignore empty air|{}\n", pExtra->HasFlag(LOCKFLAG_IGNORE_EMPTY_AIR) ? "1" : "0");
					dialog += "add_button|recalcLock|`wRe-apply lock``|noflags|0|0|\n";

					if (pTileItem->ID == ITEM_ID_BUILDER_S_LOCK)
					{
						dialog += "add_smalltext|This lock allows building or breaking.|left|\n";
						dialog += "add_smalltext|(ONLY if \"Allow anyone to build or break\" is checked above!)|left|\n";
						dialog += "add_spacer|small|\n";
						dialog += "add_smalltext|Leaving this box unchecked only allows breaking.|left|\n";
						dialog += std::format("add_checkbox|checkbox_buildonly|Only Allow Building!|{}\n", pExtra->HasFlag(LOCKFLAG_ONLY_BUILDING) ? "1" : "0");
						dialog += "add_spacer|small|\n";
						dialog += "add_smalltext|People with lock access can both build and break unless you check below. The lock owner can always build and break.|left|\n";
						dialog += std::format("add_checkbox|checkbox_admins|Admins are limited|{}\n", pExtra->HasFlag(LOCKFLAG_RESTRICT_ADMIN) ? "1" : "0");
					}
				}

				dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
				dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
				dialog += "end_dialog|lock_edit|Cancel|OK|\n";

				pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
				return;
			}
			
			if (!bIsOwner && !bIsAdmin)
			{
				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I'm `4unable`` to pick the lock.", 0, 1 });
				return;
			}

			if (bIsAdmin && !bIsOwner)
			{
				std::string dialog = "set_default_color|`o\n";
				dialog += "add_label_with_icon|big|`wEdit " + pTileItem->name + "``|left|" + std::to_string(pTileItem->ID) + "\n";
				dialog += "add_textbox|This is `wDeletedUser``'s lock but i still have access to it.|left|\n";
				dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
				dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);
				dialog += "end_dialog|remove_access|Cancel|Remove My Access!|\n";
				
				pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
				return;
			}
		} break;

		case TYPE_ITEM_SUCKER: 
		{
			TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(pTile->GetTileExtra());
			if (!pExtra)
			{
				return;
			}

			ItemInfo* pDisplayItem = GetItemInfoManager()->GetItem(pExtra->m_itemID);
			if (!pDisplayItem)
			{
				return;
			}

			std::string dialog = "set_default_color|`o\n";
			dialog += std::format("add_label_with_icon|big|`w{}``|left|{}|\n", pTileItem->name, pTileItem->ID);
			dialog += std::format("embed_data|tilex|{}\n", interactedAt.X);
			dialog += std::format("embed_data|tiley|{}\n", interactedAt.Y);

			if (pDisplayItem->ID == ITEM_ID_BLANK) 
			{
				dialog += "add_textbox|`6The machine is empty.``|\n";
				dialog += std::format("add_item_picker|selectitem|`wChoose Item``|Choose an item to put on the {}|\n", pTileItem->name);
			}
			else 
			{
				std::string stock = (pExtra->m_count < 1 ? "`6The machine is currently empty!``" : std::format("The machine contains {} `2{}``", pExtra->m_count, pDisplayItem->name));
				dialog += std::format("add_label_with_icon|small|`2{}``|left|{}\n", pDisplayItem->name, pDisplayItem->ID);
				dialog += "add_textbox|" + stock + "|\n";

				if (pPlayer->GetInventoryItemCount(pDisplayItem->ID) > 0 && pExtra->m_count < pExtra->m_capacity)
				{
					dialog += "add_button|additem|`wAdd items to the machine``|noflags|0|0|\n";
				}

				if (pPlayer->GetInventoryItemCount(pDisplayItem->ID) + pExtra->m_count <= pDisplayItem->maxCount && pExtra->m_count > 0)
				{
					dialog += "add_button|retrieveitem|`wRetrieve Items``|noflags|0|0|\n";
				}

				if (pExtra->m_count < 1)
				{
					dialog += "add_button|clearitem|`wChange Item``|noflags|0|0|\n";
				}

				if (pTileItem->ID == ITEM_ID_MAGPLANT_5000) 
				{
					if (pExtra->m_bBuildingMode)
					{
						dialog += std::format("add_textbox|Building mode: `5ACTIVE``<CR>Use the MAGPLANT 5000 Remote to build `2{}`` directly from the {}'s storage.|\n", pDisplayItem->name, pTileItem->name);
					}
					else
					{
						dialog += "add_textbox|Building mode: `6DISABLED``<CR>Punch to activate building mode.|\n";
					}

					if (pPlayer->GetInventoryItemCount(ITEM_ID_MAGPLANT_5000_REMOTE) < 1)
					{
						dialog += "add_button|getplantationdevice|`wGet Remote``|noflags|0|0|\n";
					}
				}

				dialog += std::format("add_checkbox|enablesucking|Enable Collection.|{}\n", (int)pExtra->m_bMagnet);
			}

			dialog += "end_dialog|itemsucker_block|Cancel|Update|\n";
			pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });

		} break;
	}
}

void World::OnBlockSmashSpawnThings(ItemInfo* pItem, bool* bLucky, bool* bBlock, bool* bSeed, int* gems)
{
	if (!pItem)
	{
		return;
	}

	int Roll1;
	int GemsToGive = 0;
	uint32_t Rarity;
	uint8_t bonusChance = 10;

	*bSeed = false;
	*bBlock = false;
	*gems = 0;

	Roll1 = Randomizer::Get(0, 3);
	if (Roll1 == 0) 
	{
		if (pItem->type != TYPE_SEED && !(pItem->editableTypes & DROPLESS))
		{
			*bBlock = true;
		}

		Roll1 = Randomizer::Get(0, 4);
		if (!*bLucky && Roll1 == 0)
		{
			return;
		}

		if (pItem->type != TYPE_SEED && !(pItem->editableTypes & SEEDLESS)) 
		{
			*bSeed = true;
			*bBlock = false;
			return;
		}

		if (pItem->type != TYPE_SEED && Roll1 == 0 && !(pItem->editableTypes & SEEDLESS)) 
		{
			*bSeed = true;
			return;
		}
	}

	if (pItem->rarity != 999 && !(pItem->flags & GEMLESS) && !(pItem->editableTypes & SEEDLESS)) 
	{
		Roll1 = Randomizer::Get(0, 20);
		if (Roll1 == 0)
		{
			*gems = 1 * GetBaseApp()->GetConfig().gemsMultiplier;
		}

		Rarity = pItem->rarity;
		if (7 < Rarity) 
		{
			GemsToGive = (uint32_t)(Rarity >> 2) * (3 >> 2) * GetBaseApp()->GetConfig().gemsMultiplier;
			if (30 < Rarity)
			{
				GemsToGive = (uint32_t)(Rarity >> 2) * GetBaseApp()->GetConfig().gemsMultiplier;
			}

			if (Randomizer::Get(0, 100) < bonusChance) //Spectral Gems
			{
				GemsToGive += std::rand() % 25;
			}

			Roll1 = Randomizer::Get(0, GemsToGive);
			*gems = *gems + Roll1;
		}

		if (*bLucky)
		{
			*gems = *gems * 3;
		}
	}

	return;
}

void World::OnTreeSmashSpawnThings(Tile* pTile, ItemInfo* pItem, bool bLucky, const int& netID)
{
	if (!pTile || !pItem || pItem->type != TYPE_SEED)
	{
		return;
	}

	auto Reset = [&](Tile* pTile)
	{
		if (!pTile)
		{
			return;
		}
		pTile->ToggleFlag(TILEFLAG_TREE_WILL_DROP_SEED, false);
		pTile->ToggleFlag(TILEFLAG_SPLICED_TREE, false);
		pTile->ToggleFlag(TILEFLAG_TREE, false);
		pTile->ResetExtra();
	};

	TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(pTile->GetTileExtra());
	if (!pExtra)
	{
		return;
	}

	CL_Vec2f interactedAt = { pTile->GetIndex() % m_pTilemap->GetWidth() * 32.f, pTile->GetIndex() / m_pTilemap->GetWidth() * 32.f };
	ItemInfo* pFruitItem = GetItemInfoManager()->GetItem(pTile->GetForeground() - 1);
	if (!pFruitItem)
	{
		return;
	}

	uint8_t base = pExtra->m_fruits + 1;
	float chance = static_cast<float>((std::pow(base, 2) + base) / 2);
	uint8_t fruits = static_cast<uint8_t>(chance / pExtra->m_fruits * (pFruitItem->rarity < 10 ? 3.1f : 2.6f));

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(netID);
	if (!pAvatar)
	{
		return;
	}

	if (pFruitItem->ID == ITEM_ID_LEGENDARY_WIZARD) 
	{
		Reset(pTile);
		pTile->SetForeground(ITEM_ID_LEGENDARY_WIZARD);
		if (pAvatar->GetPosition().X < interactedAt.X)
		{
			pTile->ToggleFlag(TILEFLAG_FLIPPED, true);
		}

		SendTileUpdate(pTile);
		return;
	}

	if (base > 0) 
	{
		if (pFruitItem->rarity == 999 || pFruitItem->editableTypes & SEEDLESS || pFruitItem->editableTypes & DROPLESS)
		{
			fruits = 1;
		}

		fruits = (uint8_t)Randomizer::Get<int>(pExtra->m_fruits, fruits);
		if (fruits > 15)
		{
			fruits = 15;
		}

		WorldObject fruit;
		fruit.ID = pFruitItem->ID;
		fruit.x = interactedAt.X;
		fruit.y = interactedAt.Y;
		fruit.count = fruits;

		AddObject(fruit, true);
	}

	if (pTile->HasFlag(TILEFLAG_TREE_WILL_DROP_SEED)) 
	{
		WorldObject seed;
		seed.ID = pTile->GetForeground();
		seed.x = interactedAt.X;
		seed.y = interactedAt.Y;
		seed.count = 1;

		AddObject(seed, true);
		pAvatar->GetPlayer()->SendVariantPacket({ "OnTalkBubble", netID, std::format("A {} falls out!", pItem->name), 0, 1 });
	}

	bool bBlock = false;
	bool bSeed = false;
	int gems = 0;
	OnBlockSmashSpawnThings(pItem, &bLucky, &bBlock, &bSeed, &gems);

	if (gems > 0) 
	{
		if (bLucky) 
		{
			if (Randomizer::Get(0, 100) < 98)
			{
				gems = gems / 3;
			}
			else
			{
				Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendVariantPacket({ "OnParticleEffect", 125, interactedAt, 0.0f, 0.0f }); });
			}
		}

		SpawnGemsOnATile(gems, interactedAt.X, interactedAt.Y);
	}

	Reset(pTile);
	pTile->SetForeground(0);
	SendTileUpdate(pTile);
}

void World::OnSmashGoodie(Player* pPlayer, Tile* pTile)
{
	if (!pPlayer || !pTile)
	{
		return;
	}

	ItemInfo* pItem = pTile->GetItem();
	if (!pItem)
	{
		return;
	}

	float spawnX = (float)(pTile->GetIndex() % m_pTilemap->GetWidth() * 32.f);
	float spawnY = (float)(pTile->GetIndex() / m_pTilemap->GetWidth() * 32.f);
	switch (pItem->ID)
	{
	    case ITEM_ID_GRASS: case ITEM_ID_SUGAR_CANE:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			int hit = Randomizer::Get(1, 10000);
			if (hit < 500 || hit > 1000)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_BUTTERFLY_LEASH;
			item.count = 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;

		case ITEM_ID_MAGIC_BACON_WALLPAPER:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			int hit = Randomizer::Get(1, 10000);
			if (hit < 500 || hit > 1000)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_PIGLET_LEASH;
			item.count = 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;

		case ITEM_ID_COW:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			int hit = Randomizer::Get(1, 10000);
			if (hit < 500 || hit > 1000)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_CALF_LEASH;
			item.count = 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;

		case ITEM_ID_CHICKEN:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			int hit = Randomizer::Get(1, 10000);
			if (hit < 500 || hit > 1000)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_CHICK_LEASH;
			item.count = 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;

		case ITEM_ID_WOOD_BLOCK:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			int roll = Randomizer::Get(0, 250);
			if (roll < 50 || roll > 80)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_MISSION_BLOCK;
			item.count = 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;

		case ITEM_ID_DEEP_IRON:
		{
			if (!GetBaseApp()->GetConfig().bExtraDrops)
			{
				break;
			}

			WorldObject item;
			item.ID = ITEM_ID_DEEP_IRON_ORE;
			item.count = Randomizer::Get(1, 3) + std::rand() % 1 + 1;
			item.x = spawnX;
			item.y = spawnY;

			AddObject(item, true);
		} break;
	}
}

void World::SendLock(Player* pPlayer, Tile* pLock)
{
	if (!pPlayer || !pLock)
	{
		return;
	}

	std::vector<uint16_t> lockedTiles;

	uint16_t lockIndex = pLock->GetIndex();
	for (int i = 0; i < m_pTilemap->GetTiles().size(); i++)
	{
		Tile* pTile = &m_pTilemap->GetTiles().at(i);
		if (!pTile || pTile->GetParent() != lockIndex)
		{
			continue;
		}

		lockedTiles.emplace_back(pTile->GetIndex());
	}

	uint32_t dataLength = (uint32_t)lockedTiles.size() * 2;
	uint32_t tilesLength = (uint32_t)lockedTiles.size();

	GameUpdatePacket* pPacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SEND_LOCK;
	pPacket->intData = pLock->GetForeground();
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->intX = pLock->GetIndex() % m_pTilemap->GetWidth();
	pPacket->intY = pLock->GetIndex() / m_pTilemap->GetWidth();
	pPacket->ownerID = pPlayer->GetUserID();
	pPacket->tilesLength = tilesLength;
	pPacket->dataLength = dataLength;

	int offset = 0;
	if (!lockedTiles.empty())
	{
		for (int i = 0; i < lockedTiles.size(); i++)
		{
			uint16_t index = lockedTiles[i];
			MemorySerialize(&index, pPacket->data, offset, true);
		}
	}

	Broadcast([&](int netID, Player* pTargetPlayer) 
	{ 
		pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});

	free(pPacket);
}

void World::SendTileUpdate(Tile* pTile, const int& delay /* = 0 */)
{
	if (!pTile)
	{
		return;
	}

	uint32_t dataLength = (uint32_t)pTile->GetMemoryEstimated(true, 4.62f, m_version);
	GameUpdatePacket* pPacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SEND_TILE_UPDATE_DATA;
	pPacket->netID = -1;
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->intX = pTile->GetIndex() % m_pTilemap->GetWidth();
	pPacket->intY = pTile->GetIndex() / m_pTilemap->GetWidth();
	pPacket->delay = delay;
	pPacket->dataLength = dataLength;

	int offset = 0;
	pTile->SerializeToMem(pPacket->data, offset, true, 4.62f, m_version);

	Broadcast([&](int netID, Player* pTargetPlayer)
	{
		pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});
	free(pPacket);
}

void World::SendTilesUpdate(std::vector<Tile> tiles, const int& delay /* = 0 */)
{
	if (tiles.empty())
	{
		return;
	}

	uint32_t dataLength = 4;
	for (int i = 0; i < tiles.size(); i++)
	{
		Tile tile = tiles[i];
		dataLength += 8/*tileX, tileY*/ + (uint32_t)tile.GetMemoryEstimated(true, 4.62f, m_version);
	}

	GameUpdatePacket* pPacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE;
	pPacket->netID = -1;
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->intX = -1;
	pPacket->intY = -1;
	pPacket->dataLength = dataLength;
	pPacket->delay = delay;

	int offset = 0;
	for (int i = 0; i < tiles.size(); i++)
	{
		Tile tile = tiles[i];
		int tileX = tile.GetIndex() % m_pTilemap->GetWidth();
		int tileY = tile.GetIndex() / m_pTilemap->GetWidth();

		MemorySerialize(&tileX, pPacket->data, offset, true);
		MemorySerialize(&tileY, pPacket->data, offset, true);
		tile.SerializeToMem(pPacket->data, offset, true, 4.62f, m_version);
	}

	int negative = -1;
	MemorySerialize(&negative, pPacket->data, offset, true);

	Broadcast([&](int netID, Player* pTargetPlayer)
	{
		pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});
	free(pPacket);
}

bool World::CanWrench(Tile* pTile, Player* pPlayer)
{
	if (!pTile || !pPlayer)
	{
		return false;
	}

	if (!pTile->IsLocked())
	{
		return true;
	}

	int parent = pTile->GetParent() == 0 ? pTile->GetLockIndex() : pTile->GetParent();
	if (parent == 0)
	{
		return true;
	}

	Tile* pParent = m_pTilemap->GetTile(parent);
	if (!pParent)
	{
		return false;
	}

	TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pParent->GetTileExtra());
	if (!pExtra)
	{
		return false;
	}

	bool bAccessible = false;
	std::vector<int> access = pExtra->m_admins;
	if (pParent->HasFlag(TILEFLAG_PUBLIC) || pExtra->m_ownerID == pPlayer->GetUserID() || std::find(access.begin(), access.end(), pPlayer->GetUserID()) != access.end())
	{
		bAccessible = true;
	}

	ItemInfo* pItem = pTile->GetItem();
	if (!pItem)
	{
		return false;
	}

	switch (pItem->type)
	{
	    case TYPE_STATS_BLOCK: { return true; } break;
		case TYPE_LOCK: { return pExtra->m_ownerID == pPlayer->GetUserID() || std::find(access.begin(), access.end(), pPlayer->GetUserID()) != access.end(); } break;
		case TYPE_ACHIEVEMENT: { return pExtra->m_ownerID == pPlayer->GetUserID(); } break;
	}

	return bAccessible;
}

void World::SpawnGemsOnATile(int gems, const float& x, const float& y)
{
	if (gems < 1)
	{
		return;
	}

	int blueGems = 0, redGems = 0, greenGems = 0, purpleGems = 0;
	while (gems >= 5)
	{
		blueGems += 1;
		gems -= 5;

		if (blueGems >= 2)
		{
			redGems += 1;
			blueGems -= 2;

			if (redGems >= 5)
			{
				greenGems += 1;
				redGems -= 5;

				if (greenGems >= 2)
				{
					purpleGems += 1;
					greenGems -= 2;
				}
			}
		}
	}

	WorldObject obj;
	obj.ID = ITEM_ID_GEMS;
	for (int i = 0; i < gems; i++)
	{
		obj.count = 1;
		obj.x = x;
		obj.y = y;
		AddObject(obj, true);
	}

	for (int i = 0; i < blueGems; i++)
	{
		obj.count = 5;
		obj.x = x;
		obj.y = y;
		AddObject(obj, true);
	}

	for (int i = 0; i < redGems; i++)
	{
		obj.count = 10;
		obj.x = x;
		obj.y = y;
		AddObject(obj, true);
	}

	for (int i = 0; i < greenGems; i++)
	{
		obj.count = 50;
		obj.x = x;
		obj.y = y;
		AddObject(obj, true);
	}

	for (int i = 0; i < purpleGems; i++)
	{
		obj.count = 100;
		obj.x = x;
		obj.y = y;
		AddObject(obj, true);
	}
}

void World::AddObject(WorldObject& object, const bool& bMagnet /* = false */, const bool& bCenterize /* = false */)
{
	if (!m_pObjectmap || object.x / 32.f < 0 || object.y / 32.f < 0 || object.x / 32.f >= m_pTilemap->GetWidth() || object.y / 32.f >= m_pTilemap->GetHeight())
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(object.ID);
	if (!pItem || object.ID < ITEM_ID_DIRT || object.count < 1 || object.count > pItem->maxCount)
	{
		return;
	}

	auto ItemChange = [&](const short& ID, const int& objectID, const float& vecX, const float& vecY, const uint8_t& count)
	{
		GameUpdatePacket modItem;
		modItem.type = NET_GAME_PACKET_ITEM_CHANGE_OBJECT;
		modItem.objectChangeType = CHANGETYPE_EDIT;
		modItem.intData = ID;
		modItem.itemNetID = objectID;
		modItem.vecX = vecX;
		modItem.vecY = vecY;
		modItem.objectAltCount = count;

		Broadcast([&](int netID, Player* pTargetPlayer)
		{
			pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &modItem, sizeof(GameUpdatePacket));
		});
	};

	if (object.x < 8) 
	{
		object.x = 8.f;
	}

	if (object.y < 8) 
	{
		object.y = 8.f;
	}

	if (object.x >= m_pTilemap->GetWidth() * 32)
	{
		object.x = (m_pTilemap->GetWidth() * 32.f) - 8.f;
	}

	if (object.y >= m_pTilemap->GetHeight() * 32)
	{
		object.y = (m_pTilemap->GetHeight() * 32.f) - 8.f;
	}

	if (!bCenterize) 
	{
		object.x += static_cast<float>(std::rand() % 16);
		object.y += static_cast<float>(std::rand() % 16);
	}

	if (bMagnet) 
	{
		for (int i = 0; i < m_pTilemap->GetTiles().size(); i++) 
		{
			Tile* pTile = &m_pTilemap->GetTiles().at(i);
			if (!pTile || !pTile->GetItem() || (pTile->GetItem()->type != TYPE_ITEM_SUCKER && pTile->GetItem()->type != TYPE_AUTO_ACTION_HARVEST_SUCK))
			{
				continue;
			}

			if (pTile->GetItem()->type == TYPE_ITEM_SUCKER)
			{
				TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(pTile->GetTileExtra());
				if (!pExtra || !pExtra->m_bMagnet || pExtra->m_count >= pExtra->m_capacity || pExtra->m_itemID != object.ID)
				{
					continue;
				}

				int overfillAmount = (pExtra->m_count + object.count > pExtra->m_capacity ? pExtra->m_capacity - pExtra->m_count + object.count : 0);
				pExtra->m_count += object.count - overfillAmount;

				GameUpdatePacket packet;
				packet.type = NET_GAME_PACKET_ITEM_EFFECT;
				packet.intX = object.ID;
				packet.intY = object.ID;
				packet.netID = 1;
				packet.targetNetID = -1;
				packet.vecX = (pTile->GetIndex() % m_pTilemap->GetWidth() * 32.f) + 8.f;
				packet.vecY = (pTile->GetIndex() / m_pTilemap->GetWidth() * 32.f) + 8.f;
				packet.destX = object.x + 12.f;
				packet.destY = object.y + 12.f;
				packet.delay = 300;

				Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &packet, sizeof(GameUpdatePacket)); });

				if (overfillAmount > 0) 
				{
					object.count = overfillAmount;
					AddObject(object, bMagnet, bCenterize);
				}

				SendTileUpdate(pTile);
				return;
			}

			if (pTile->GetItem()->type == TYPE_AUTO_ACTION_HARVEST_SUCK)
			{
				//TileExtraAutoAction2* pExtra = dynamic_cast<TileExtraAutoAction2*>(pTile->GetTileExtra());
				//if (!pExtra || !pExtra->m_magnet || pExtra->m_count >= pExtra->m_capacity || pExtra->m_itemID != object.ID)
				//{
					//continue;
				//}

				//int overfillAmount = (pExtra->m_count + object.count > pExtra->m_capacity ? pExtra->m_capacity - pExtra->m_count + object.count : 0);
				//pExtra->m_count += object.count - overfillAmount;

				GameUpdatePacket packet;
				packet.type = NET_GAME_PACKET_ITEM_EFFECT;
				packet.intX = object.ID;
				packet.intY = object.ID;
				packet.netID = 1;
				packet.targetNetID = -1;
				packet.vecX = (pTile->GetIndex() % m_pTilemap->GetWidth() * 32.f) + 8.f;
				packet.vecY = (pTile->GetIndex() / m_pTilemap->GetWidth() * 32.f) + 8.f;
				packet.destX = object.x + 12.f;
				packet.destY = object.y + 12.f;
				packet.delay = 300;

				Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &packet, sizeof(GameUpdatePacket)); });

				//if (overfillAmount > 0) 
				//{
					//object.count = overfillAmount;
					//AddObject(object, bMagnet, bCenterize);
				//}

				SendTileUpdate(pTile);
				return;
			}
		}
	}

	if (GetBaseApp()->GetConfig().bCollideDrops)
	{
		for (int i = 0; i < m_pObjectmap->GetObjects().size(); i++)
		{
			WorldObject* pObject = m_pObjectmap->GetObjects().at(i);
			if (!pObject || pObject->ID != object.ID)
			{
				continue;
			}

			if (pObject->x < (object.x + 20) && pObject->x > (object.x - 12) && pObject->y < (object.y + 20) && pObject->y > (object.y - 12))
			{
				if (pItem->type == TYPE_GEMS && pObject->count < 100)
				{
					LogMsg("item is gem & not maxed");
					int gems = pObject->count;
					int remainingGems = object.count;
					if (remainingGems < 1)
					{
						return;
					}

					LogMsg("theres %d gems left to add", remainingGems);

					bool bMaxed = false;
					while (remainingGems > 5 && !bMaxed)
					{
						if (pObject->count < 5 && remainingGems > 5 - pObject->count)
						{
							int added = 5 - pObject->count;
							pObject->count += added;
							remainingGems -= added;

							ItemChange(pObject->ID, pObject->objectID, pObject->x, pObject->y, pObject->count);
							LogMsg("made blue gem");

							if (pObject->count >= 5 && pObject->count < 10 && remainingGems > 10 - pObject->count)
							{
								int added = 10 - pObject->count;
								pObject->count += added;
								remainingGems -= added;

								ItemChange(pObject->ID, pObject->objectID, pObject->x, pObject->y, pObject->count);
								LogMsg("made red gem");

								if (pObject->count >= 10 && pObject->count < 50 && remainingGems > 50 - pObject->count)
								{
									int added = 50 - pObject->count;
									pObject->count += added;
									remainingGems -= added;

									ItemChange(pObject->ID, pObject->objectID, pObject->x, pObject->y, pObject->count);
									LogMsg("made green gem");

									if (pObject->count >= 50 && pObject->count < 100 && remainingGems > 100 - pObject->count)
									{
										int added = 100 - pObject->count;
										pObject->count += added;
										remainingGems -= added;

										ItemChange(pObject->ID, pObject->objectID, pObject->x, pObject->y, pObject->count);
										LogMsg("made pink gem");
										bMaxed = true;
									}
								}
							}
						}
					}

					LogMsg("spawning %d remaining gems", remainingGems);
					SpawnGemsOnATile(remainingGems, object.x / 32.f, object.y / 32.f);
					remainingGems = 0;
					break;
				}
				else
				{
					bool bOverfill = (pObject->count + object.count > 200);
					if (bOverfill)
					{
						pObject->count = pObject->count + object.count - pItem->maxCount;
					}

					pObject->count = bOverfill ? pItem->maxCount : pObject->count + object.count;
					ItemChange(pObject->ID, pObject->objectID, pObject->x, pObject->y, pObject->count);
					if (!bOverfill)
					{
						return;
					}
				}
			}
		}
	}

	m_pObjectmap->AddObject(object);
	GameUpdatePacket spawnItem;
	spawnItem.type = NET_GAME_PACKET_ITEM_CHANGE_OBJECT;
	spawnItem.objectChangeType = CHANGETYPE_SPAWN;
	spawnItem.intData = object.ID;
	spawnItem.itemNetID = -1;
	spawnItem.vecX = object.x;
	spawnItem.vecY = object.y;
	spawnItem.objectAltCount = object.count;

	Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &spawnItem, sizeof(GameUpdatePacket)); });
}

void World::CollectObject(const int& netID, const int& objectID, const CL_Vec2f& interactedAt)
{
	if ((int)(interactedAt.X / 32.f) >= m_pTilemap->GetWidth() || (int)(interactedAt.X / 32.f) < 0 || (int)(interactedAt.Y / 32.f) >= m_pTilemap->GetHeight() || (int)(interactedAt.Y / 32.f) < 0)
	{
		return;
	}

	NetAvatar* pAvatar = m_pNetObjectManager->GetNetObject(netID);
	if (!pAvatar)
	{
		return;
	}

	Player* pPlayer = pAvatar->GetPlayer();
	if (!pPlayer)
	{
		return;
	}

	WorldObject* pObject = m_pObjectmap->GetObjectByID(objectID);
	if (!pObject)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pObject->ID);
	if (!pItem)
	{
		return;
	}

	if (pAvatar->GetPlayer()->GetItems()->GetCloth(HAND) == ITEM_ID_MAGIC_MAGNET && pAvatar->GetPlayer()->GetMagnetItem() != ITEM_ID_BLANK && pObject->ID != pAvatar->GetPlayer()->GetMagnetItem())
	{
		return;
	}

	CL_Vec2f distance = { std::abs(pAvatar->GetPosition().X - interactedAt.X), std::abs(pAvatar->GetPosition().Y - interactedAt.Y) };
	if (distance.X > 256 || distance.Y > 256 || distance.X < -256 || distance.Y < -256) 
	{
		pAvatar->GetPlayer()->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "(Too far away)", 0, 1 });
		return;
	}

	auto SendCollect = [&](int netID, int objectID) 
	{
		GameUpdatePacket itemPickup;
		itemPickup.type = NET_GAME_PACKET_ITEM_CHANGE_OBJECT;
		itemPickup.objectChangeType = CHANGETYPE_PICKUP;
		itemPickup.netID = pAvatar->GetNetID();
		itemPickup.objectID = objectID;
		itemPickup.itemNetID = -1;

		Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &itemPickup, sizeof(GameUpdatePacket)); });
		m_pObjectmap->RemoveObject(objectID);
	};

	auto ModifyItem = [&](int objectID, WorldObject* pObject) 
	{
		if (!pObject)
		{
			return;
		}

		GameUpdatePacket itemChange;
		itemChange.type = NET_GAME_PACKET_ITEM_CHANGE_OBJECT;
		itemChange.objectChangeType = CHANGETYPE_EDIT;
		itemChange.intData = pObject->ID;
		itemChange.itemNetID = objectID;
		itemChange.vecX = pObject->x;
		itemChange.vecY = pObject->y;
		itemChange.objectAltCount = (float)pObject->count;

		Broadcast([&](int netID, Player* pTargetPlayer) { pTargetPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &itemChange, sizeof(GameUpdatePacket)); });
	};

	Tile* pTile = m_pTilemap->GetTile((int)pObject->x / 32.f, (int)pObject->y / 32.f);
	if (!pTile || !pTile->GetItem())
	{
		return;
	}

	if (pTile->GetItem()->tileCollision == TILE_COLLISION_SOLID)
	{
		return;
	}

	if (IsTileObstacle(pAvatar->GetPlayer(), pTile))
	{
		return;
	}

	if ((pTile->GetItem()->ID == ITEM_ID_DISPLAY_BOX || pTile->GetItem()->ID == ITEM_ID_TRANSMATTER_FIELD) && (pTile->IsLocked() && !pTile->IsOwner(pPlayer) && !pTile->IsAdmin(pPlayer)))
	{
		return;
	}

	if (pItem->type == TYPE_GEMS) 
	{
		if (pAvatar->GetPlayer()->GetItems()->GetGems() >= INT_MAX - 100)
		{
			return;
		}

		pAvatar->GetPlayer()->GetItems()->SetGems(pAvatar->GetPlayer()->GetItems()->GetGems() + pObject->count);
		SendCollect(pAvatar->GetNetID(), objectID);
		return;
	}

	uint8_t count = pAvatar->GetPlayer()->GetInventoryItemCount(pItem->ID);
	uint8_t limit = pItem->maxCount - pAvatar->GetPlayer()->GetInventoryItemCount(pItem->ID);
	if (limit < 1)
	{
		return;
	}

	if (limit > pObject->count)
	{
		limit = pObject->count;
	}

	if (count != 0 && pObject->count - limit) 
	{
		if (!pAvatar->GetPlayer()->AddInventoryItem(pObject->ID, limit, true))
		{
			return;
		}

		pObject->count -= limit;
		pAvatar->GetPlayer()->SendVariantPacket({ "OnConsoleMessage", std::format("Collected `w{} {}``.{}", limit, pItem->name, pItem->rarity != 999 ? " Rarity: `w" + std::to_string(pItem->rarity) + "``" : "") });
		
		ModifyItem(objectID, pObject);
		return;
	}

	if (!pAvatar->GetPlayer()->AddInventoryItem(pObject->ID, pObject->count, false))
	{
		return;
	}

	pAvatar->GetPlayer()->SendVariantPacket({ "OnConsoleMessage", std::format("Collected `w{} {}``.{}", pObject->count, pItem->name, pItem->rarity != 999 ? " Rarity: `w" + std::to_string(pItem->rarity) + "``" : "") });
	SendCollect(pAvatar->GetNetID(), objectID);
}

bool World::IsTileObstacle(Player* pPlayer, Tile* pTile)
{
	if (!pPlayer || !pTile)
	{
		return true;
	}

	if (pPlayer->GetCharacterState().HasStateFlag(STATEFLAG_NOCLIP))
	{
		return false;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(pTile->GetForeground());
	if (!pItem)
	{
		return true;
	}

	switch (pItem->tileCollision)
	{
	case TILE_COLLISION_NONE: case TILE_COLLISION_ONE_WAY: case TILE_COLLISION_JUMP_THROUGH: case TILE_COLLISION_JUMP_DOWN:
	{
		return false;
	}

	case TILE_COLLISION_SOLID:
	{
		if (pTile->GetForeground() == 0)
		{
			return false;
		}

	} return true;

	case TILE_COLLISION_IF_OFF:
	{
		if (pTile->HasFlag(TILEFLAG_ENABLED))
		{
			return false;
		}

	} return true;

	case TILE_COLLISION_IF_ON:
	{
		return pTile->HasFlag(TILEFLAG_ENABLED);
	}

	case TILE_COLLISION_ADVENTURE:
	{
		if (pTile->HasFlag(TILEFLAG_ENABLED))
		{
			return false;
		}

	} return true;

	case TILE_COLLISION_GATEWAY:
	{
		bool bIsLocked = pTile->IsLocked();
		if (!bIsLocked)
		{
			return false;
		}

		Tile* pParent = GetTileMap()->GetTile(pTile->GetParent());
		if (!pParent)
		{
			return true;
		}

		bool bIsOwner = pTile->IsOwner(pPlayer);
		bool bIsAdmin = pTile->IsAdmin(pPlayer);
		if (bIsOwner || bIsAdmin || pParent->HasFlag(TILEFLAG_PUBLIC))
		{
			return false;
		}

		return true;
	}

	default:
	{
		return false;
	}
	}

	return true;
}

std::vector<CL_Vec2i> World::GetPathToPointB(Player* pPlayer, const CL_Vec2i& interactedFrom, const CL_Vec2i& interactedAt)
{
	std::vector<CL_Vec2i> path;
	if (!pPlayer)
	{
		return path;
	}

	if (interactedFrom == interactedAt)
	{
		return path;
	}

	Tile* pStartTile = GetTileMap()->GetTile(interactedFrom);
	Tile* pGoalTile = GetTileMap()->GetTile(interactedAt);

	if (!pStartTile || IsTileObstacle(pPlayer, pStartTile) || !pGoalTile || IsTileObstacle(pPlayer, pGoalTile))
	{
		return path;
	}

	std::array<CL_Vec2i, 4> directions =
	{
		CL_Vec2i{ -1, 0 },
		CL_Vec2i{ 0, -1 },
		CL_Vec2i{ 1, 0 },
		CL_Vec2i{ 0, 1 }
	};

	std::vector<Node*> openNodes;
	std::vector<Node*> closedNodes;
	openNodes.push_back(new Node{ interactedAt });
	Node* currentNode = nullptr;

	while (!openNodes.empty())
	{
		auto it = std::min_element(openNodes.begin(), openNodes.end(), [](Node* pNodeA, Node* pNodeB) { return (pNodeA->g_cost + pNodeA->h_cost) < (pNodeB->g_cost + pNodeB->h_cost); });
		currentNode = *it;
		if (currentNode->pos == interactedFrom)
		{
			break;
		}

		closedNodes.push_back(currentNode);
		openNodes.erase(it);

		Tile* pCurrentTile = GetTileMap()->GetTile(currentNode->pos.X, currentNode->pos.Y);
		ItemInfo* pCurrentInfo = GetItemInfoManager()->GetItem(pCurrentTile->GetForeground());
		if (!pCurrentInfo)
		{
			continue;
		}

		for (int i = 0; i < directions.size(); i++)
		{
			CL_Vec2i dir = directions[i];
			CL_Vec2i neighborPos = currentNode->pos + dir;
			if (FindNode(closedNodes, neighborPos) != nullptr)
			{
				continue;
			}

			Tile* pNeighborTile = GetTileMap()->GetTile(neighborPos);
			if (!pNeighborTile || IsTileObstacle(pPlayer, pNeighborTile))
			{
				continue;
			}

			ItemInfo* pNeighborInfo = GetItemInfoManager()->GetItem(pNeighborTile->GetForeground());
			if (!pNeighborInfo)
			{
				continue;
			}

			if (pNeighborInfo->tileCollision == TILE_COLLISION_ONE_WAY && !pCurrentTile->HasFlag(TILEFLAG_FLIPPED) && currentNode->pos.X < neighborPos.X)
			{
				continue;
			}

			if (pNeighborInfo->tileCollision == TILE_COLLISION_ONE_WAY && pCurrentTile->HasFlag(TILEFLAG_FLIPPED) && currentNode->pos.X > neighborPos.X)
			{
				continue;
			}

			if (pCurrentInfo->tileCollision == TILE_COLLISION_JUMP_THROUGH && currentNode->pos.Y > neighborPos.Y)
			{
				continue;
			}

			if (pCurrentInfo->tileCollision == TILE_COLLISION_JUMP_DOWN && currentNode->pos.Y < neighborPos.Y)
			{
				continue;
			}

			if (pCurrentInfo->tileCollision == TILE_COLLISION_ONE_WAY && !pCurrentTile->HasFlag(TILEFLAG_FLIPPED) && currentNode->pos.X < neighborPos.X)
			{
				continue;
			}

			if (pCurrentInfo->tileCollision == TILE_COLLISION_ONE_WAY && pCurrentTile->HasFlag(TILEFLAG_FLIPPED) && currentNode->pos.X > neighborPos.X)
			{
				continue;
			}

			std::uint32_t cost = currentNode->g_cost + 1;
			Node* pNeighborNode = FindNode(openNodes, neighborPos);
			if (!pNeighborNode)
			{
				pNeighborNode = new Node(neighborPos);
				pNeighborNode->parent = currentNode;
				pNeighborNode->g_cost = cost;
				pNeighborNode->h_cost = CalculateHeuristic(neighborPos, interactedFrom);
				openNodes.push_back(pNeighborNode);
			}
			else if (cost < pNeighborNode->g_cost)
			{
				pNeighborNode->parent = currentNode;
				pNeighborNode->g_cost = cost;
			}
		}
	}

	if (!currentNode || currentNode->pos != interactedFrom)
	{
		for (Node* node : openNodes)
		{
			delete node;
		}

		for (Node* node : closedNodes)
		{
			delete node;
		}

		return {};
	}

	while (currentNode)
	{
		path.push_back(currentNode->pos);
		currentNode = currentNode->parent;
	}

	for (Node* node : openNodes)
	{
		delete node;
	}

	for (Node* node : closedNodes)
	{
		delete node;
	}

	return path;
}

bool World::CanWalkThatWay(Player* pPlayer, GameUpdatePacket* pPacket, const bool& bMoving /* = false */)
{
	if (!pPlayer || !pPacket)
	{
		return false;
	}

	NetAvatar* pAvatar = GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return false;
	}

	CL_Vec2i startPos = { static_cast<int>((pAvatar->GetPosition().X + 10) / 32), static_cast<int>((pAvatar->GetPosition().Y + 15) / 32) };
	CL_Vec2i goalPos = { static_cast<int>((pPacket->vecX + 10) / 32), static_cast<int>((pPacket->vecY + 15) / 32) };

	if (std::abs(goalPos.X - startPos.X) >= 8 || std::abs(startPos.Y - goalPos.Y) >= 8 || std::abs(goalPos.X - startPos.X) <= -8 || std::abs(startPos.Y - goalPos.Y) <= -8)
	{
		if (bMoving)
		{
			pPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetPosition() }, pAvatar->GetNetID());
		}
		return false;
	}

	Tile* pTileArray[4] =
	{
		GetTileMap()->GetTile(static_cast<int>(pPacket->vecX / 32), static_cast<int>(pPacket->vecY / 32)),
		GetTileMap()->GetTile(static_cast<int>((pPacket->vecX + 19) / 32), static_cast<int>(pPacket->vecY / 32)),
		GetTileMap()->GetTile(static_cast<int>(pPacket->vecX / 32), static_cast<int>((pPacket->vecY + 29) / 32)),
		GetTileMap()->GetTile(static_cast<int>((pPacket->vecX + 19) / 32), static_cast<int>((pPacket->vecY + 29) / 32))
	};

	if (IsTileObstacle(pPlayer, pTileArray[0]) && IsTileObstacle(pPlayer, pTileArray[1]) && IsTileObstacle(pPlayer, pTileArray[2]) && IsTileObstacle(pPlayer, pTileArray[3]))
	{
		if (bMoving)
		{
			pPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetPosition() }, pAvatar->GetNetID());
		}
		return false;
	}

	else if (IsTileObstacle(pPlayer, pTileArray[0]) || IsTileObstacle(pPlayer, pTileArray[1]) || IsTileObstacle(pPlayer, pTileArray[2]) || IsTileObstacle(pPlayer, pTileArray[3]))
	{
		if (bMoving)
		{
			pPlayer->SendVariantPacket({ "OnSetPos", pAvatar->GetPosition() }, pAvatar->GetNetID());
		}
		return false;
	}
	else
	{
		Tile* pStartTile = GetTileMap()->GetTile(startPos.X, startPos.Y);
		Tile* pGoalTile = GetTileMap()->GetTile(goalPos.X, goalPos.Y);
		if (!pStartTile || !pGoalTile)
		{
			return false;
		}

		auto openNodes = GetPathToPointB(pPlayer, startPos, goalPos);
		if (openNodes.empty() && startPos != goalPos)
		{
			if (bMoving)
			{
				CL_Vec2f pos = pAvatar->GetPosition();
				ItemInfo* pItem = GetItemInfoManager()->GetItem(pGoalTile->GetForeground());
				if (!pItem)
				{
					pPlayer->SendVariantPacket({ "OnSetPos", pos }, pAvatar->GetNetID());
					return false;
				}

				if (pItem->tileCollision == TILE_COLLISION_JUMP_THROUGH)
				{
					pos.Y -= 20;
				}
				if (pItem->tileCollision == TILE_COLLISION_JUMP_DOWN)
				{
					pos.Y += 20;
				}

				pPlayer->SendVariantPacket({ "OnSetPos", pos }, pAvatar->GetNetID());
				return false;
			}
			return false;
		}
	}

	return true;
}