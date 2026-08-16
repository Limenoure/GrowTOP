#ifndef WORLDS__WORLDSMANAGER__HPP
#define WORLDS__WORLDSMANAGER__HPP
#include <string>
#include <vector>

#include <World/World.hpp>

class Player;
class WorldsManager
{
public:
	WorldsManager() = default;
	~WorldsManager() = default;


	int GetWorldID() const { return m_worldID; }
	void SetWorldID(const int& id) { m_worldID = id; }

	std::vector<World*>& GetWorlds() { return m_activeWorlds; }

	void AddWorld(World* pWorld);

	World* GetWorld(const int& mapID);
	World* GetWorld(const std::string& fName);
	World* GetWorldByName(const std::string& fName);

	void ValidateWorld(Player* pPlayer, const std::string& worldName);
	void SendWorldOffers(Player* pPlayer);
	void EnterWorld(Player* pPlayer, World* pWorld, CL_Vec2f spawnXY = CL_Vec2f{ -1.f, -1.f });
	void ExitWorld(Player* pPlayer, World* pWorld, const bool& bSendOffers = true);

private:
	int m_worldID = 0;

	std::vector<World*> m_activeWorlds;

};

WorldsManager* GetWorldsManager();
#endif // WORLDS__WORLDSMANAGER__HPP