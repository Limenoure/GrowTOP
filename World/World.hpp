#ifndef WORLD__WORLD__HPP
#define WORLD__WORLD__HPP

#include <string>
#include <functional>
#include <World/NetObjectManager.hpp>

#include <ProtonSDK/Math.h>
#include <World/Tile.hpp>
#include <World/WorldTileMap.hpp>
#include <World/WorldObjectMap.hpp>

enum eWorldCategory : uint8_t
{
	WORLDCATEGORY_NONE,
	WORLDCATEGORY_ADVENTURE,
	WORLDCATEGORY_ART,
	WORLDCATEGORY_FARM,
	WORLDCATEGORY_GAME,
	WORLDCATEGORY_GUILD,
	WORLDCATEGORY_INFORMATION,
	WORLDCATEGORY_MUSIC,
	WORLDCATEGORY_PARKOUR,
	WORLDCATEGORY_PUZZLE,
	WORLDCATEGORY_ROLEPLAY,
	WORLDCATEGORY_SHOP,
	WORLDCATEGORY_SOCIAL,
	WORLDCATEGORY_STORAGE,
	WORLDCATEGORY_STORY,
	WORLDCATEGORY_TRADE

};

//eWorldBits
#define WORLDBIT_NONE 0x0
#define WORLDBIT_JAMMED 0x1
#define WORLDBIT_IMMUNE 0x2
#define WORLDBIT_NOPUNCH 0x4
#define WORLDBIT_NOCONSUME 0x8
#define WORLDBIT_NODROP 0x10
#define WORLDBIT_ANTIGRAVITY 0x20
#define WORLDBIT_NOGO 0x40
#define WORLDBIT_NOLOCKS 0x80
#define WORLDBIT_NORATING 0x100
#define WORLDBIT_NOWAR 0x200
#define WORLDBIT_HAUNTED 0x400
#define WORLDBIT_INACCESSIBLE 0x800


class World
{
public:
	World(const std::string& name, const int& width = 100, const int& height = 60);

	~World();

	void Broadcast(std::function<void(int, Player*)> pair);

	int GetBits() const { return m_bits; }
	void SetBits(const int& bits) { m_bits = bits; }
	bool HasBit(const int& bit) const { return m_bits & bit; }
	void ToggleBit(const int& bit, const bool& bEnable)
	{
		if (HasBit(bit) && !bEnable)
		{
			m_bits &= ~bit;
		}
		if (!HasBit(bit) && bEnable)
		{
			m_bits |= bit;
		}
	}

	uint16_t GetVersion() const { return m_version; }
	void SetVersion(const uint16_t& version) { m_version = version; }

	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	int GetID() const { return m_ID; }
	void SetID(const int& id) { m_ID = id; }

	WorldTileMap* GetTileMap() const { return m_pTilemap; }
	Tile* GetTile(const uint16_t& index);
	Tile* GetTile(const uint16_t& x, const uint16_t& y);
	Tile* GetTile(const float& fX, const float& fY);
	Tile* GetTile(const CL_Vec2f& vec);
	Tile* GetTile(const CL_Vec2i& vec);

	WorldObjectMap* GetObjectMap() const { return m_pObjectmap; }
	NetObjectManager* GetNetObjectManager() const { return m_pNetObjectManager; }

	int GetWeather() const { return m_weather; }
	void SetWeather(const int& weather) { m_weather = weather; }

	int GetBaseWeather() const { return m_baseWeather; }
	void SetBaseWeather(const int& weather) { m_baseWeather = weather; }

	int GetOwnerID() const { return m_ownerID; }
	void SetOwnerID(const int& userID) { m_ownerID = userID; }

	uint16_t GetLockIndex() const { return m_lockIndex; }
	void SetLockIndex(const uint16_t& index) { m_lockIndex = index; }

	std::string GetDescription() const { return m_description; }
	void SetDescription(const std::string& desc) { m_description = desc; }

	eWorldCategory GetCategory() const { return m_category; }
	void SetCategory(const eWorldCategory& category) { m_category = category; }
	std::string GetCategoryName();

	uint8_t GetTimer() const { return m_timer; }
	void SetTimer(const uint8_t& timer) { m_timer = timer; }

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23);
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23);
	std::string GetSize(const size_t& size);

	int GetPlayersInWorld();


	//tile change request
	void HandlePacketTileChangeRequest(Player* pPlayer, GameUpdatePacket* pPacket);
	void HandlePacketTileChangePunch(Player* pPlayer, GameUpdatePacket* pPacket);
	void HandlePacketTileChangePlace(Player* pPlayer, GameUpdatePacket* pPacket);
	void HandlePacketTileChangeConsume(Player* pPlayer, GameUpdatePacket* pPacket);
	void HandlePacketTileChangeWrench(Player* pPlayer, GameUpdatePacket* pPacket);


	void OnBlockSmashSpawnThings(ItemInfo* pItem, bool* bLucky, bool* bBlock, bool* bSeed, int* gems);
	void OnTreeSmashSpawnThings(Tile* pTile, ItemInfo* pItem, bool bLucky, const int& netID);
	void OnSmashGoodie(Player* pPlayer, Tile* pTile);

	//locks
	void SendLock(Player* pPlayer, Tile* pLock);

	void SendTileUpdate(Tile* pTile, const int& delay = 0);
	void SendTilesUpdate(std::vector<Tile> tiles, const int& delay = 0);

	//area permissions
	bool CanWrench(Tile* pTile, Player* pPlayer);


	//WorldObjectMap
	void SpawnGemsOnATile(int gems, const float& x, const float& y);
	void AddObject(WorldObject& object, const bool& bMagnet = false, const bool& bCenterize = false);
	void CollectObject(const int& netID, const int& objectID, const CL_Vec2f& interactedAt);


	//Anticheat
	bool IsTileObstacle(Player* pPlayer, Tile* pTile);
	std::vector<CL_Vec2i> GetPathToPointB(Player* pPlayer, const CL_Vec2i& interactedFrom, const CL_Vec2i& interactedAt);
	bool CanWalkThatWay(Player* pPlayer, GameUpdatePacket* pPacket, const bool& bMoving = false);

private:
	uint16_t m_version = 5; //23
	int m_bits = WORLDBIT_NONE;
	std::string m_name;
	int m_ID = -1;

	eWorldCategory m_category;

	WorldTileMap* m_pTilemap = nullptr;
	WorldObjectMap* m_pObjectmap = nullptr;
	NetObjectManager* m_pNetObjectManager = nullptr;

	int m_weather = 4;
	int m_baseWeather = 4;

	//server side info
	int m_ownerID = -1;
	uint16_t m_lockIndex = 0;
	std::string m_description = "";

	uint8_t m_timer = 0;

};

#endif // WORLD__WORLD__HPP