#include <BaseApp.hpp> //precomp
#include <Network/Store/StoreManager.hpp>


#include <ProtonSDK/TextScanner.h>
#include <ProtonSDK/MiscUtils.h>

#include <Player/Player.hpp>

StoreManager g_store;
StoreManager* GetStoreManager() { return &g_store; }

void StoreManager::Init()
{
	TextScanner t;
	t.LoadFile("essential/store.txt");
	if (!t.IsLoaded())
	{
		LogError("failed to load store essentials.");
		return;
	}

	int offers = 0;
	int items = 0;
	for (int currentLine = 0; currentLine < t.GetLineCount(); currentLine++)
	{
		std::string lineString = t.GetLine(currentLine);
		std::vector<std::string> line = t.TokenizeLine(currentLine);
		if (line.empty() || lineString.empty() || lineString.starts_with("#"))
		{
			continue;
		}

		if (line[0] == "add_offer")
		{
			if (line.size() < 8)
			{
				continue;
			}

			StoreOffer offer;
			offer.SetID(std::atoi(line[1].c_str()));
			offer.SetName(line[2]);
			offer.SetHeader(std::atoi(line[3].c_str()));
			offer.SetTab(std::atoi(line[4].c_str()));
			offer.SetCurrency(std::atoi(line[5].c_str()));
			offer.SetDescription(line[6]);
			offer.SetPrice(std::atoi(line[7].c_str()));

			std::string texture = line[8];
			std::vector<std::string> textureCrap = Utils::StringTokenize(texture, ":");
			if (textureCrap.size() < 3)
			{
				continue;
			}

			offer.SetTexture(textureCrap[0]);
			offer.SetTextureX(std::atoi(textureCrap[1].c_str()));
			offer.SetTextureY(std::atoi(textureCrap[2].c_str()));

		    m_offers.emplace_back(offer);
			++offers;
		}

		if (line[0] == "add_item")
		{
			if (line.size() < 3)
			{
				continue;
			}

			int id = std::stoi(line[1].c_str());
			for (int i = 0; i < m_offers.size(); i++)
			{
				StoreOffer& offer = m_offers[i];
				if (offer.GetID() != id)
				{
					continue;
				}

				int itemID = std::stoi(line[2].c_str());
				int count = std::stoi(line[3].c_str());
				
				StoreItem item;
				item.itemID = itemID;
				item.count = count;

				offer.GetItems().emplace_back(item);
				++items;
				break;
			}
		}
	}

	LogMsg("loaded store essentials, %d offers, %d items.", offers, items);
}

void StoreManager::ShowStore(Player* pPlayer, const uint8_t& tab)
{
	if (!pPlayer)
	{
		return;
	}

	switch (tab)
	{
	    case STORETAB_GLOBAL_OFFERS:
		{
			//add ur store here
		} break;
		
		default:
		{
			//add ur store here
		} break;
	}
}