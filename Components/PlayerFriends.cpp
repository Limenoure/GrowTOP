#include <BaseApp.hpp> //precomp

#include <Components/PlayerFriends.hpp>
#include <Player/Player.hpp>

bool PlayerFriends::IsFriend(const int& userID)
{
	for (int i = 0; i < m_friends.size(); i++)
	{
		Friend info = m_friends[i];
		if (info.userID != userID)
		{
			continue;
		}

		return true;
	}

	return false;
}

void PlayerFriends::Update(Player* pFriend)
{
	if (!pFriend || !IsFriend(pFriend->GetUserID()))
	{
		return;
	}

	for (int i = 0; i < m_friends.size(); i++)
	{
		Friend& info = m_friends[i];
		if (info.userID != pFriend->GetUserID())
		{
			continue;
		}

		info.lastLogon = std::chrono::steady_clock::now();
		info.lastName = pFriend->GetStaticDisplayName();
	}
}

void PlayerFriends::RemoveFriend(Player* pPlayer, Player* pFriend)
{
	if (!pPlayer || !pFriend)
	{
		return;
	}

	auto it = std::find_if(m_friends.begin(), m_friends.end(), [&pFriend](const Friend& info) { return info.userID == pFriend->GetUserID(); });
	if (it == m_friends.end())
	{
		return;
	}

	m_friends.erase(it);
	if (pFriend->GetFriends().bNotifications)
	{
		pFriend->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`3FRIENDS ALERT: ``" + pPlayer->GetStaticDisplayName() + " has removed you from their friends list.");
	}
}