#ifndef NETWORK__STOREOFFER__HPP
#define NETWORK__STOREOFFER__HPP

#include <string>
#include <vector>

enum eStoreTabs
{
	STORETAB_NONE,
	STORETAB_GLOBAL_OFFERS,
	STORETAB_ITEM_PACKS,
	STORETAB_LOCKS,
	STORETAB_AWESOME_ITEMS,
	STORETAB_TECH_ITEMS,
	STORETAB_SPEND_TOKENS,
	STORETAB_MARKETPLACE

};

enum eStoreCurrency
{
	CURRENCY_NONE,
	CURRENCY_GEMS,	
	CURRENCY_WORLD_LOCKS,
	CURRENCY_GROWTOKENS,
	CURRENCY_GROWVOUCHERS,
	CURRENCY_MONEY

};

enum eStoreHeader
{
	HEADER_NONE,
	HEADER_EVENT,
	HEADER_DAILY,
	HEADER_FEATURED,
	HEADER_GEMS,
	HEADER_VIP,
	HEADER_RANKS

};


struct StoreItem
{
	uint16_t itemID;
	uint8_t count;

};

class StoreOffer
{
public:
	StoreOffer() = default;
	~StoreOffer() = default;

	int GetID() const { return m_ID; }
	void SetID(const int& ID) { m_ID = ID; }

	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	uint8_t GetHeader() const { return m_header; }
	void SetHeader(const uint8_t& header) { m_header = header; }

	uint8_t GetTab() const { return m_tab; }
	void SetTab(const uint8_t& tab) { m_tab = tab; }

	uint8_t GetCurrency() const { return m_currency; }
	void SetCurrency(const uint8_t& currency) { m_currency = currency; }

	std::string GetDescription() const { return m_description; }
	void SetDescription(const std::string& desccription) { m_description = desccription; }

	int GetPrice() const { return m_price; }
	void SetPrice(const int& price) { m_price = price; }

	std::string GetTexture() const { return m_texture; }
	void SetTexture(const std::string& texture) { m_texture = texture; }

	uint8_t GetTextureX() const { return m_textureX; }
	void SetTextureX(const uint8_t& X) { m_textureX = X; }

	uint8_t GetTextureY() const { return m_textureY; }
	void SetTextureY(const uint8_t& Y) { m_textureY = Y; }

	std::vector<StoreItem> GetItems() { return m_items; }

private:
	int m_ID;
	std::string m_name;

	uint8_t m_header = 0;
	uint8_t m_tab = 0;
	uint8_t m_currency = 0;

	std::string m_description;
	int m_price = 0;

	std::string m_texture;
	uint8_t m_textureX;
	uint8_t m_textureY;

	std::vector<StoreItem> m_items;
};

#endif // NETWORK__STOREOFFER__HPP