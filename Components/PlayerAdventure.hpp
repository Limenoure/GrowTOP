#ifndef COMPONENTS__PLAYERADVENTURE__HPP
#define COMPONENTS__PLAYERADVENTURE__HPP

#include <string>
#include <vector>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/Math.h>


#define ADVENTURE_ITEMS_MAX_SIZE 5

struct PlayerAdventureItem
{
	uint16_t ID;
	CL_Vec2f vec;
};

class Player;
class ItemInfoManager;
class PlayerAdventure
{
public:
	PlayerAdventure() = default;
	~PlayerAdventure() = default;

	
	std::vector<PlayerAdventureItem>& GetItems() { return m_items; }

	void AddItem(Player* pPlayer, const uint16_t& tileID, const CL_Vec2f& vec, const bool& bUpdate = false);
	void RemoveItem(Player* pPlayer, const uint16_t& tileID, const bool& bUpdate = false);
	void RemoveItem(Player* pPlayer, const CL_Vec2f& vec, const bool& bUpdate = false);

private:
	std::vector<PlayerAdventureItem> m_items;

};


#endif // COMPONENTS__PLAYERADVENTURE__HPP