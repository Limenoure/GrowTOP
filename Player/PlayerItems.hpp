#ifndef PLAYER__PLAYERITEMS__HPP
#define PLAYER__PLAYERITEMS__HPP

#include <string>
#include <vector>
#include <array>
#include <Items/Enums.hpp>

#define BACKPACK_MAX_SIZE 396 //the max slots backpack can have
#define BACKPACK_INITIAL_SIZE 16 //the default backpack slots

//eInventoryItemFlags
#define INVENTORY_ITEMFLAG_EQUIPPED 1

struct PlayerInventoryItem
{
public:
	uint16_t itemID;
	uint8_t count;
	uint8_t flags = 0;

};


class PlayerItems
{
public:
	PlayerItems();
	~PlayerItems();

	int GetGems() const { return m_gems; }
	void SetGems(const int& gems) { m_gems = gems; }

	int GetBackpackSlots() const { return m_backpackSlots; }
	void SetBackpackSlots(const int& slots) { m_backpackSlots = slots; }

	std::vector<PlayerInventoryItem>& GetItems() { return m_items; }
	void ClearItems() 
	{
		m_items.clear();
	}

	bool AddInventoryItem(const uint16_t& itemID, const uint8_t& count);
	bool RemoveInventoryItem(const uint16_t& itemID, const uint8_t& count);

	std::array<short, NUM_CLOTHES> GetClothes() { return m_clothes; }
	std::array<short, NUM_CLOTHES> GetTempClothes() { return m_tempClothes; }

	short GetCloth(const eClothingType& type, const bool& bTempItem = true);
	void SetCloth(const eClothingType& type, const short& clothing, const bool& bTempItem = false);

private:
	int m_gems = 0;

	std::array<short, NUM_CLOTHES> m_clothes;
	std::array<short, NUM_CLOTHES> m_tempClothes;
	int m_backpackSlots = BACKPACK_INITIAL_SIZE;
	std::vector<PlayerInventoryItem> m_items;

};

#endif // PLAYER__PLAYERITEMS__HPP