#ifndef WORLD__NETOBJECTMANAGER__HPP
#define WORLD__NETOBJECTMANAGER__HPP

#include <string>
#include <vector>

#include <Player/NetAvatar.hpp>

class Player;
class World;
class NetObjectManager 
{
public:
	NetObjectManager(World* pWorld);
	~NetObjectManager() = default;

	World* GetWorld() const { return m_pWorld; }

	int GetNetID() const { return m_netID; }
	void SetNetID(const int& netID) { m_netID = netID; }

	int GetCount() const { return m_count; }
	void SetCount(const int& count) { m_count = count; }

	std::vector<NetAvatar*>& GetObjects() { return m_objects; }
	NetAvatar* GetNetObject(const int& netID);
	NetAvatar* GetNetObject(Player* pPlayer);

	void AddObject(Player* pPlayer);
	NetAvatar* MakeObject(Player* pPlayer);
	void RemoveObject(const int& netID);

private:
	int m_netID = 0;
	int m_count = 0;

	std::vector<NetAvatar*> m_objects;
	World* m_pWorld = nullptr;
};

#endif // WORLD__NETOBJECTMANAGER__HPP