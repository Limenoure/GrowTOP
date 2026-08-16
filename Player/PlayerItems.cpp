#include <BaseApp.hpp> //precomp

#include <Player/PlayerItems.hpp>

PlayerItems::PlayerItems()
{
	//fixing missing fist, wrench if exist
	PlayerInventoryItem fist;
	fist.itemID = ITEM_ID_FIST;
	fist.count = 1;

	PlayerInventoryItem wrench;
	wrench.itemID = ITEM_ID_WRENCH;
	wrench.count = 1;

	m_items.push_back(fist);
	m_items.push_back(wrench);
}

PlayerItems::~PlayerItems()
{
	m_items.clear();
}

bool PlayerItems::AddInventoryItem(const uint16_t& itemID, const uint8_t& count)
{
	ItemInfo* pItem = GetItemInfoManager()->GetItem(itemID);
	if (!pItem)
	{
		return false;
	}

	auto it = std::find_if(m_items.begin(), m_items.end(), [itemID](const PlayerInventoryItem& inventoryItem) { return inventoryItem.itemID == itemID; });
	if (it != m_items.end()) 
	{
		if (it->count + count > pItem->maxCount)
		{
			return false;
		}

		it->count += count;
		return true;
	}
	
	if (m_items.size() >= m_backpackSlots)
	{
		return false;
	}

	PlayerInventoryItem item;
	item.itemID = itemID;
	item.count = count;
	item.flags = 0;

	m_items.push_back(item);
	return true;
}

bool PlayerItems::RemoveInventoryItem(const uint16_t& itemID, const uint8_t& count)
{
	if (count < 1)
	{
		return false;
	}

	ItemInfo* pItem = GetItemInfoManager()->GetItem(itemID);
	if (!pItem)
	{
		return false;
	}

	auto it = std::find_if(m_items.begin(), m_items.end(), [itemID](const PlayerInventoryItem& inventoryItem) { return inventoryItem.itemID == itemID; });
	if (it == m_items.end())
	{
		return false;
	}

	if (count > it->count)
	{
		return false;
	}

	it->count -= count;
	if (it->count < 1)
	{
		for (int i = 0; i < NUM_CLOTHES; i++) 
		{
			if (m_clothes[i] == itemID)
				m_clothes[i] = 0;
		}

		m_items.erase(it);
	}

	return true;
}

short PlayerItems::GetCloth(const eClothingType& type, const bool& bTempItem /* = true */)
{
	if (type < 0 || type > NUM_CLOTHES)
	{
		return 0;
	}
	
	return bTempItem ? m_tempClothes[type] : m_clothes[type];
}

void PlayerItems::SetCloth(const eClothingType& type, const short& clothing, const bool& bTempItem /* = false */)
{
	if (type < 0 || type > NUM_CLOTHES)
	{
		return;
	}

	if (bTempItem)
	{
		m_tempClothes[type] = clothing;
	}
	else
	{
		m_clothes[type] = clothing;
	}
}