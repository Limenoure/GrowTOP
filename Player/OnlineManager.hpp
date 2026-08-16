#ifndef PLAYER__ONLINEMANAGER__HPP
#define PLAYER__ONLINEMANAGER__HPP
#include <string>
#include <vector>
#include <chrono>

#include <enet/enet.h>

class Player;
class OnlineManager
{
public:
	OnlineManager() = default;
	~OnlineManager() = default;


	int GetOnlineID() const { return m_onlineID; }
	void SetOnlineID(const int& id) { m_onlineID = id; }

	int GetUserID(const bool& bIncrement = false) { return bIncrement ? ++m_onlineID : m_onlineID; }
	void SetUserID(const int& id) { m_onlineID = id; }

	int GetLogonsThisSecond() const { return m_logonsThisSecond; }
	void SetLogons(const int& logons) { m_logonsThisSecond = logons; }

	std::chrono::steady_clock::time_point GetTick() const { return m_lastTickUpdate; }
	void SetTick(const std::chrono::steady_clock::time_point& t) { m_lastTickUpdate = t; }

	int GetPlayersOnline();

	Player* MakePlayer(ENetPeer* pPeer);
	void RemovePlayer(Player* pPlayer);

	int GetOnlinePlayersByIP(const char* address);

	Player* GetPlayer(const int& onlineID);
	
	Player* GetOnlinePlayerByID(const int& userID);
	Player* GetOnlinePlayerByName(const std::string& fName);

private:
	int m_onlineID = 0;
	int m_userID = 0;
	int m_logonsThisSecond = 0;
	std::chrono::steady_clock::time_point m_lastTickUpdate = std::chrono::steady_clock::now();

	std::vector<Player*> m_onlinePlayers;
};

OnlineManager* GetOnlineManager();

#endif // PLAYER__ONLINEMANAGER__HPP