#ifndef ITEMS__ITEMINFOMANAGER__HPP
#define ITEMS__ITEMINFOMANAGER__HPP
#include <vector>

#include <Items/ItemInfo.hpp>
#include <Packet/GameUpdatePacket.hpp>

struct SplicingRecipe 
{
	uint16_t seed1;
	uint16_t seed2;

	uint16_t result;

};


class TextScanner;
class ItemInfoManager 
{
public:
	ItemInfoManager() = default;
	~ItemInfoManager() = default;


	uint32_t GetHash() { return m_hash; }
	GameUpdatePacket* GetPacket() { return m_packet; }
	std::vector<ItemInfo*> GetItems() { return m_items; }



	ItemInfo* GetItem(const uint16_t& ID);
	ItemInfo* GetItemByName(std::string fName);

	ItemInfo* CreateSeedVersionOfLastAddedItem(const uint16_t& tileID);
	ItemInfo* CreateNullItemOfLastItemAdded(const uint16_t& tileID);

	int StringToItemFlag(const std::string& str);
	std::string ItemFlagToString(const int& itemFlag);

	int StringToItemFlag2(const std::string& str);
	std::string ItemFlag2ToString(const int& itemFlag);

	int StringToItemFxFlag(const std::string& str);
	std::string ItemFxFlagToString(const int& fxFlag);

	bool Load(TextScanner& t);
	bool LoadFromFile();
	bool LoadSplicing();

	void Serialize(const uint16_t& version);
	void DumpDefinitions();

	uint16_t GetSplicingInfo(const uint16_t& seed1, const uint16_t& seed2);

private:
	std::vector<char> m_data;
	uint32_t m_hash = 0;
	uint16_t m_version = 18;
	int m_itemCount = 0;

	GameUpdatePacket* m_packet = nullptr;

	std::vector<ItemInfo*> m_items;
	std::vector<SplicingRecipe> m_splices;

};

ItemInfoManager* GetItemInfoManager();

#endif // ITEMS__ITEMINFOMANAGER__HPP