#include <BaseApp.hpp> //precomp

#include <Components/DailyQuest.hpp>

DailyQuest g_dailyQuest;
DailyQuest* GetDailyQuest() { return &g_dailyQuest; }


bool DailyQuest::HasCompleted(const int& userID)
{
	return std::find(m_completed.begin(), m_completed.end(), userID) != m_completed.end();
}

void DailyQuest::Init()
{
	m_items.clear();
	for (int i = 0; i < GetItemInfoManager()->GetItems().size(); i++)
	{
		ItemInfo* pItem = GetItemInfoManager()->GetItems().at(i);
		if (!pItem)
		{
			continue;
		}

		if (pItem->rarity > 50 || pItem->rarity < 1 || pItem->editableTypes & UNTRADEABLE || pItem->editableTypes & MOD)
		{
			//item is untradeable or does not match rarity criteria
			continue;
		}

		if (pItem->spliceOne == 0 && pItem->spliceTwo == 0 && pItem->type != TYPE_SEED)
		{
			//item is not spliceable
			continue;
		}

		m_items.emplace_back(pItem->ID);
	}

	LogMsg("Daily Quest contains %d items in list.", (int)m_items.size());
}

void DailyQuest::Reset()
{
	if (m_items.empty())
	{
		Init();
	}

	m_completed.clear();

	m_item.ID = m_items[Randomizer::Get(0, (int)m_items.size())];
	m_item.count = 200;

	m_item2.ID = m_items[Randomizer::Get(0, (int)m_items.size())];
	m_item2.count = Randomizer::Get(25, 150);

	m_endPoint = std::chrono::steady_clock::now() + std::chrono::hours(24);
}