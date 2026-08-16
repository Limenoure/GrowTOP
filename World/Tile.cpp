#include <BaseApp.hpp> //precomp

#include <World/Tile.hpp>
#include <World/TileExtraManager.hpp>


#include <ProtonSDK/MiscUtils.h>
#include <Items/ItemInfoManager.hpp>
#include <Player/Player.hpp>
#include <World/World.hpp>


void Tile::SetForeground(const uint16_t& tileID) 
{
	ItemInfo* pItem = GetItemInfoManager()->GetItem(tileID);
	if (!pItem)
	{
		return;
	}

	switch (pItem->type) 
	{
		//TODO
	case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
	{
		bool bIsMainDoor = pItem->type == TYPE_MAIN_DOOR;
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraDoor();
		m_pExtra->SetType(TILEEXTRA_DOOR);

		TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_label = bIsMainDoor ? "EXIT" : "";
		pExtra->m_flags = 0;
		pExtra->m_id = "";
		pExtra->m_destination = "";
		pExtra->m_password = "";

	} break;

	case TYPE_SIGN: case TYPE_SECURITY_CAMERA:
	{
		bool bIsSecurityCamera = pItem->type == TYPE_SECURITY_CAMERA;
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraSign();
		m_pExtra->SetType(TILEEXTRA_SIGN);

		TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_label = "";
		pExtra->m_id = "";
		pExtra->m_flags = bIsSecurityCamera ? 0 : -1;

	} break;

	case TYPE_DONATION_BOX:
	{
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraDonationBox();
		m_pExtra->SetType(TILEEXTRA_SIGN);

		TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_donations.clear();

	} break;

	case TYPE_LOCK:
	{
		bool bIsAreaLock = pItem->lockPower != 0;
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraLock();
		m_pExtra->SetType(TILEEXTRA_LOCK);

		TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_flags = bIsAreaLock ? LOCKFLAG_AREA_LOCK : LOCKFLAG_NONE;
		pExtra->m_ownerID = -1;
		pExtra->m_tempo = 100;
		pExtra->m_admins.clear();

	} break;

	case TYPE_SEED:
	{
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		ToggleFlag(TILEFLAG_TREE, true);
		ToggleFlag(TILEFLAG_SPLICED_TREE, false);
		m_pExtra = new TileExtraSeed();
		m_pExtra->SetType(TILEEXTRA_TREE);

		float seedChance = 3.5f / (pItem->rarity + 13);
		if ((float)rand() / (float)RAND_MAX <= seedChance)
		{
			ToggleFlag(TILEFLAG_TREE_WILL_DROP_SEED, true);
		}

		TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_growingSince = std::chrono::steady_clock::now();
		pExtra->m_fruits = Randomizer::Get(1, (int)pItem->fruits);
	} break;

	case TYPE_DICE:
	{
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraRoll();
		m_pExtra->SetType(TILEEXTRA_DICE);

		TileExtraRoll* pExtra = dynamic_cast<TileExtraRoll*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_roll = 0;
	} break;

	case TYPE_DISPLAY_BLOCK:
	{
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraDisplayBlock();
		m_pExtra->SetType(TILEEXTRA_DISPLAY_BLOCK);

		TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_placedAt = std::chrono::steady_clock::now();
		pExtra->m_itemID = ITEM_ID_BLANK;
	} break;

	case TYPE_ITEM_SUCKER:
	{
		bool bIsMagplant = pItem->ID == ITEM_ID_MAGPLANT_5000;
		ToggleFlag(TILEFLAG_EXTRA_DATA, true);
		m_pExtra = new TileExtraItemSucker();
		m_pExtra->SetType(TILEEXTRA_ITEM_SUCKER);

		TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(m_pExtra);
		if (!pExtra)
		{
			break;
		}

		pExtra->m_itemID = ITEM_ID_BLANK;
		pExtra->m_count = 0;
		pExtra->m_bMagnet = false;
		pExtra->m_bBuildingMode = false;
		pExtra->m_capacity = bIsMagplant ? 5000 : 1500;

	} break;

	default: 
	{ 
		ToggleFlag(TILEFLAG_EXTRA_DATA, false); 
	} break;
	}

	m_foreground = tileID;
}


bool Tile::IsLocked()
{
	return m_lockIndex != 0 || m_parent != 0;
}

bool Tile::IsAdmin(Player* pPlayer)
{
	if (!IsLocked() || !pPlayer || !pPlayer->GetWorld())
	{
		return false;
	}

	if (pPlayer->GetRole() >= ROLE_ADMINISTRATOR)
	{
		return true;
	}

	World* pWorld = pPlayer->GetWorld();
	Tile* pLock = pWorld->GetTileMap()->GetTile(m_parent == 0 ? m_lockIndex : m_parent);
	if (!pLock)
	{
		return false;
	}

	TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pLock->m_pExtra);
	if (!pExtra)
	{
		return false;
	}

	std::vector<int> access = pExtra->m_admins;
	if (pExtra->m_ownerID == pPlayer->GetUserID() || std::find(access.begin(), access.end(), pPlayer->GetUserID()) != access.end())
	{
		return true;
	}

	return false;
}

bool Tile::IsOwner(Player* pPlayer)
{
	if (!IsLocked() || !pPlayer || !pPlayer->GetWorld())
	{
		return false;
	}

	if (pPlayer->GetRole() >= ROLE_ADMINISTRATOR)
	{
		return true;
	}

	World* pWorld = pPlayer->GetWorld();
	Tile* pLock = pWorld->GetTileMap()->GetTile(m_parent == 0 ? m_lockIndex : m_parent);
	if (!pLock)
	{
		return false;
	}

	TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pLock->m_pExtra);
	if (!pExtra)
	{
		return false;
	}

	return pExtra->m_ownerID == pPlayer->GetUserID();
}

ItemInfo* Tile::GetItem() 
{
	return GetItemInfoManager()->GetItem(m_foreground != ITEM_ID_BLANK ? m_foreground : m_background);
}

bool Tile::IsSteamPunk() const 
{
	ItemInfo* pItem = GetItemInfoManager()->GetItem(m_foreground);
	if (!pItem)
	{
		return false;
	}

	return pItem->type == TYPE_STEAMPUNK || pItem->ID == ITEM_ID_STEAM_PIPE || pItem->ID == ITEM_ID_STEAM_LAUNCHER || pItem->ID == ITEM_ID_STEAM_VENT || pItem->ID == ITEM_ID_STEAM_LAMP || pItem->ID == ITEM_ID_STEAM_DOOR;
}

size_t Tile::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */) 
{
	ItemInfo* pItem = GetItemInfoManager()->GetItem(m_foreground);
	if (!pItem)
	{
		return 0;
	}

	size_t estimated = 0;
	estimated += sizeof(uint16_t) * 4; //foreground, background, lockIndex and flags

	if (bPacket && m_flags & TILEFLAG_LOCKED)
	{
		estimated += sizeof(uint16_t); //when tile is locked by area lock it adds additional parent which represents the lock's index
	}

	if (!bPacket)
	{
		estimated += sizeof(uint16_t) * 2; //server side info, it's the index of the tile which is used to determine it's position and parent(stored because it's not always in the packet)
	}

	if (m_flags & TILEFLAG_EXTRA_DATA && GetTileExtraManager()->Supported(pItem->type, mapVersion))
	{
		//GetTileExtraManager()->Supported checks if the item type is required in the current map version to prevent crashs for older clients when serializing tileextras
		estimated += sizeof(uint8_t); //extra tile type

		switch (pItem->type) 
		{
		case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
		{
			TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_SIGN: case TYPE_SECURITY_CAMERA:
		{
			TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);

		} break;

		case TYPE_DONATION_BOX:
		{
			TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);

		} break;

		case TYPE_LOCK:
		{
			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_SEED:
		{
			TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_DICE:
		{
			TileExtraRoll* pExtra = dynamic_cast<TileExtraRoll*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_DISPLAY_BLOCK:
		{
			TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_ITEM_SUCKER:
		{
			TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			estimated += pExtra->GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
		} break;
		}
	}

	return estimated;
}

void Tile::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */) 
{
	if (!pMem)
	{
		return;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(m_foreground);
	if (!pItem)
	{
		return;
	}

	MemorySerialize(&m_foreground, pMem, offset, true);
	MemorySerialize(&m_background, pMem, offset, true);
	MemorySerialize(&m_parent, pMem, offset, true);
	MemorySerialize(&m_flags, pMem, offset, true);

	if (bPacket && m_flags & TILEFLAG_LOCKED)
	{
		MemorySerialize(&m_parent, pMem, offset, true);
	}

	if (!bPacket) 
	{
		MemorySerialize(&m_index, pMem, offset, true);
		MemorySerialize(&m_lockIndex, pMem, offset, true);
	}

	if (m_flags & TILEFLAG_EXTRA_DATA && GetTileExtraManager()->Supported(pItem->type, mapVersion))
	{
		//GetTileExtraManager()->Supported checks if the item type is required in the current map version to prevent crashs for older clients when serializing tileextras
		uint8_t extraType = m_pExtra->GetType();
		MemorySerialize(&extraType, pMem, offset, true);

		switch (pItem->type)
		{
		case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
		{
			TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_SIGN: case TYPE_SECURITY_CAMERA:
		{
			TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);

		} break;

		case TYPE_DONATION_BOX:
		{
			TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);

		} break;

		case TYPE_LOCK:
		{
			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_SEED:
		{
			TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_DICE:
		{
			TileExtraRoll* pExtra = dynamic_cast<TileExtraRoll*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_DISPLAY_BLOCK:
		{
			TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;

		case TYPE_ITEM_SUCKER:
		{
			TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
		} break;
		}
	}
}

void Tile::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_foreground, pMem, offset, false);
	MemorySerialize(&m_background, pMem, offset, false);
	MemorySerialize(&m_parent, pMem, offset, false);
	MemorySerialize(&m_flags, pMem, offset, false);

	if (bPacket && m_flags & TILEFLAG_LOCKED)
	{
		MemorySerialize(&m_parent, pMem, offset, false);
	}

	if (!bPacket)
	{
		MemorySerialize(&m_index, pMem, offset, false);
		MemorySerialize(&m_lockIndex, pMem, offset, false);
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(m_foreground);
	if (!pItem)
	{
		return;
	}

	if (m_flags & TILEFLAG_EXTRA_DATA && GetTileExtraManager()->Supported(pItem->type, mapVersion))
	{
		//GetTileExtraManager()->Supported checks if the item type is required in the current map version to prevent crashs for older clients when serializing tileextras
		uint8_t extraType = 0;
		MemorySerialize(&extraType, pMem, offset, false);

		switch (pItem->type)
		{
		case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
		{
			m_pExtra = new TileExtraDoor();
			TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_SIGN: case TYPE_SECURITY_CAMERA:
		{
			m_pExtra = new TileExtraSign();
			TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_DONATION_BOX:
		{
			m_pExtra = new TileExtraDonationBox();
			TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_LOCK:
		{
			m_pExtra = new TileExtraLock();
			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_SEED:
		{
			m_pExtra = new TileExtraSeed();
			TileExtraSeed* pExtra = dynamic_cast<TileExtraSeed*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_DICE:
		{
			m_pExtra = new TileExtraRoll();
			TileExtraRoll* pExtra = dynamic_cast<TileExtraRoll*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_DISPLAY_BLOCK:
		{
			m_pExtra = new TileExtraDisplayBlock();
			TileExtraDisplayBlock* pExtra = dynamic_cast<TileExtraDisplayBlock*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;

		case TYPE_ITEM_SUCKER:
		{
			m_pExtra = new TileExtraItemSucker();
			TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(m_pExtra);
			if (!pExtra)
			{
				break;
			}

			pExtra->SerializeFromMem(pMem, offset, bPacket, mapVersion);
		} break;
		}

		m_pExtra->SetType(extraType);
	}
}