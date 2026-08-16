#include <BaseApp.hpp> //precomp

#include <algorithm>
#include <Player/OnlineManager.hpp>

#include <Player/Player.hpp>



OnlineManager g_online_manager;
OnlineManager* GetOnlineManager() { return &g_online_manager; }


int OnlineManager::GetPlayersOnline()
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	int onlineUsers = 0;
	for (int i = 0; i < m_onlinePlayers.size(); i++)
	{
		Player* pPlayer = m_onlinePlayers[i];
		if (!pPlayer || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME))
		{
			continue;
		}

		++onlineUsers;
	}

	return onlineUsers;
}

Player* OnlineManager::MakePlayer(ENetPeer* pPeer)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	Player* pPlayer = new Player(pPeer);
	m_onlinePlayers.push_back(pPlayer);
	return pPlayer;
}

void OnlineManager::RemovePlayer(Player* pPlayer)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	if (!pPlayer)
	{
		return;
	}

	auto it = std::remove(m_onlinePlayers.begin(), m_onlinePlayers.end(), pPlayer);
	if (it != m_onlinePlayers.end())
	{
		delete pPlayer->GetPeer()->data;
		m_onlinePlayers.erase(it);
	}
}

int OnlineManager::GetOnlinePlayersByIP(const char* address)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	int count = 0;
	for (int i = 0; i < m_onlinePlayers.size(); i++)
	{
		Player* pPlayer = m_onlinePlayers[i];
		if (!pPlayer || pPlayer->GetTankInfo()->address != address)
		{
			continue;
		}

		count += 1;
	}

	return count;
}

Player* OnlineManager::GetPlayer(const int& onlineID)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	for (int i = 0; i < m_onlinePlayers.size(); i++)
	{
		Player* pPlayer = m_onlinePlayers[i];
		if (!pPlayer || pPlayer->GetOnlineID() != onlineID)
		{
			continue;
		}

		return pPlayer;
	}

	return nullptr;
}

Player* OnlineManager::GetOnlinePlayerByID(const int& userID)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	for (int i = 0; i < m_onlinePlayers.size(); i++)
	{
		Player* pPlayer = m_onlinePlayers[i];
		if (!pPlayer || pPlayer->GetUserID() != userID)
		{
			continue;
		}

		return pPlayer;
	}

	return nullptr;
}

Player* OnlineManager::GetOnlinePlayerByName(const std::string& fName)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	std::string nameToUse = fName;
	std::transform(nameToUse.begin(), nameToUse.end(), nameToUse.begin(), ::tolower);
	for (int i = 0; i < m_onlinePlayers.size(); i++)
	{
		Player* pPlayer = m_onlinePlayers[i];
		if (!pPlayer)
		{
			continue;
		}

		std::string nameToCheck = pPlayer->GetName();
		std::transform(nameToCheck.begin(), nameToCheck.end(), nameToCheck.begin(), ::tolower);
		if (nameToCheck != nameToUse)
		{
			continue;
		}

		return pPlayer;
	}

	return nullptr;
}