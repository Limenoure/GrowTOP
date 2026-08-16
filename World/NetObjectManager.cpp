#include <BaseApp.hpp> //precomp

#include <World/NetObjectManager.hpp>
#include <Player/Player.hpp>
#include <World/World.hpp>

NetObjectManager::NetObjectManager(World* pWorld)
{
	m_pWorld = pWorld;
}

NetAvatar* NetObjectManager::GetNetObject(const int& netID) 
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		NetAvatar* pAvatar = m_objects[i];
		if (!pAvatar || pAvatar->GetNetID() != netID)
		{
			continue;
		}

		return pAvatar;
	}

	return nullptr;
}

NetAvatar* NetObjectManager::GetNetObject(Player* pPlayer) 
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (int i = 0; i < m_objects.size(); i++)
	{
		NetAvatar* pAvatar = m_objects[i];
		if (!pAvatar || pAvatar->GetPlayer() != pPlayer)
		{
			continue;
		}

		return pAvatar;
	}

	return nullptr;
}

void NetObjectManager::AddObject(Player* pPlayer) 
{
	if (!pPlayer)
	{
		return;
	}

	NetAvatar* pAvatar = new NetAvatar(pPlayer);
	pAvatar->SetNetID(m_netID++);
	m_objects.push_back(std::move(pAvatar));
}

NetAvatar* NetObjectManager::MakeObject(Player* pPlayer)
{
	if (!pPlayer)
	{
		return nullptr;
	}

	NetAvatar* pAvatar = new NetAvatar(pPlayer);
	pAvatar->SetNetID(m_netID++);
	pAvatar->SetWorld(m_pWorld);

	m_objects.push_back(std::move(pAvatar));
	return pAvatar;
}

void NetObjectManager::RemoveObject(const int& netID) 
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&](NetAvatar* pAvatar) 
    {
		return pAvatar && pAvatar->GetNetID() == netID;
	});

	if (it == m_objects.end()) 
	{
		return;
	}

	delete* it;
	m_objects.erase(it);
}