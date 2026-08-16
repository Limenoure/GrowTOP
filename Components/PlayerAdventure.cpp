#include <BaseApp.hpp> //precomp

#include <Components/PlayerAdventure.hpp>

#include <Items/ItemInfoManager.hpp>
#include <Player/Player.hpp>

void PlayerAdventure::AddItem(Player* pPlayer, const uint16_t& tileID, const CL_Vec2f& vec, const bool& bUpdate /* = false */)
{
	ItemInfo* pItem = GetItemInfoManager()->GetItem(tileID);
	if (!pItem)
	{
		return;
	}

	PlayerAdventureItem item;
	item.ID = tileID;
	item.vec = vec;

	m_items.push_back(item);
	if (pPlayer && bUpdate)
	{
		GameUpdatePacket adventurePacket;
		adventurePacket.type = NET_GAME_PACKET_SPECIAL;
		adventurePacket.netID = (int32_t)m_items.size();
		adventurePacket.intData = tileID;
		adventurePacket.tilePosX = (uint32_t)vec.X;
		adventurePacket.tilePosY = (uint32_t)vec.Y;

		pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &adventurePacket, sizeof(GameUpdatePacket));
	}
}

void PlayerAdventure::RemoveItem(Player* pPlayer, const uint16_t& tileID, const bool& bUpdate /* = false */)
{
	m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&tileID](const PlayerAdventureItem& item) 
	{
		return item.ID == tileID;
    }), m_items.end());
}

void PlayerAdventure::RemoveItem(Player* pPlayer, const CL_Vec2f& vec, const bool& bUpdate /* = false */)
{
	m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&vec](const PlayerAdventureItem& item)
	{
		return item.vec == vec;
	}), m_items.end());
}