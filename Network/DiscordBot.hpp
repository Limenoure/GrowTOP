#ifndef NETWORK__DISCORDBOT__HPP
#define NETWORK__DISCORDBOT__HPP

#include <string>
#include <dpp/dpp.h>

class DiscordBot 
{
public:
	DiscordBot() = default;
	~DiscordBot()
	{
		if (m_pCluster)
		{
			delete m_pCluster;
		}
	}

	dpp::cluster* GetCluster() const { return m_pCluster; }
	std::chrono::steady_clock::time_point GetTick() { return m_lastTick; }
	void SetTick(const std::chrono::steady_clock::time_point& tp) { m_lastTick = tp; }

	void Init();

private:
	dpp::cluster* m_pCluster = nullptr;
	std::chrono::steady_clock::time_point m_lastTick;

};

DiscordBot* GetDiscordBot();

#endif // NETWORK__DISCORDBOT__HPP