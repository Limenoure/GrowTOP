#ifndef COMPONENTS__DAILYQUEST__HPP
#define COMPONENTS__DAILYQUEST__HPP

#include <vector>


struct DailyQuestItem
{
	uint16_t ID;
	uint8_t count;

};

class DailyQuest
{
public:
	DailyQuest() = default;
	~DailyQuest() = default;

	std::vector<int>& GetCompletedList() { return m_completed; }
	bool HasCompleted(const int& userID);

	void Init();
	void Reset();

public:
	DailyQuestItem m_item;
	DailyQuestItem m_item2;

	std::chrono::steady_clock::time_point m_endPoint = std::chrono::steady_clock::now() + std::chrono::hours(24);

private:
	std::vector<uint16_t> m_items;
	std::vector<int> m_completed;

};

DailyQuest* GetDailyQuest();

#endif // COMPONENTS__DAILYQUEST__HPP