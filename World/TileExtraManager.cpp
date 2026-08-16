#include <BaseApp.hpp> //precomp

#include <World/TileExtraManager.hpp>
#include <Items/ItemInfo.hpp>


TileExtraManager g_tileExtraManager;
TileExtraManager* GetTileExtraManager() { return &g_tileExtraManager; }

bool TileExtraManager::Supported(const uint8_t& type, const uint16_t& mapVersion)
{
	switch (type)
	{
	case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
	{
		//type 1
		return true;
	}
	case TYPE_SIGN: case TYPE_SECURITY_CAMERA: case TYPE_DONATION_BOX:
	{
		//type 2
		//TODO: list the rest type 2 items: mailboxes, bulletin board, storage boxes
		return true;
	}
	case TYPE_LOCK:
	{
		//type 3
		return true;
	}
	case TYPE_SEED:
	{
		//type 4
		return true;
	}
	case TYPE_DICE:
	{
		//type 8
		return true;
	}
	case TYPE_DISPLAY_BLOCK:
	{
		//type 23
		return true;
	}

	case TYPE_ITEM_SUCKER:
	{
		//type 62
		return true;
	}
	case TYPE_AUTO_ACTION_BREAK: case TYPE_AUTO_ACTION_HARVEST: case TYPE_AUTO_ACTION_HARVEST_SUCK:
	{
		return mapVersion >= 8;
	}
	}

	return false;
}