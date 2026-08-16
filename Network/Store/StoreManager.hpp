#ifndef NETWORK__STORE__STOREMANAGER__HPP
#define NETWORK__STORE__STOREMANAGER__HPP

#include <string>
#include <vector>

#include <Network/Store/StoreOffer.hpp>

class Player;
class StoreManager
{
public:
	StoreManager() = default;
	~StoreManager() = default;

	std::vector<StoreOffer>& GetOffers() { return m_offers; }

	void Init();
	void ShowStore(Player* pPlayer, const uint8_t& tab);

private:
	std::vector<StoreOffer> m_offers;
};

StoreManager* GetStoreManager();

#endif // NETWORK__STORE__STOREMANAGER__HPP