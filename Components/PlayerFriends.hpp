#ifndef COMPONENTS__PLAYERFRIENDS__HPP
#define COMPONENTS__PLAYERFRIENDS__HPP

#include <string>
#include <vector>
#include <chrono>

struct Friend
{
	int userID;
	std::string lastName;

	std::chrono::steady_clock::time_point lastLogon;

	//options
	bool bMuted = false;
	bool bTradingDisabled = false;

};

class Player;
class PlayerFriends
{
public:
	PlayerFriends() = default;
	~PlayerFriends() = default;

	std::vector<Friend>& GetFriends() { return m_friends; }
	bool IsFriend(const int& userID);

	void Update(Player* pFriend);
	void RemoveFriend(Player* pPlayer, Player* pFriend);

public:
	bool bNotifications = true;
	bool bHideLocation = false;

private:
	std::vector<Friend> m_friends;

};

#endif // COMPONENTS__PLAYERFRIENDS__HPP