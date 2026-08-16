#ifndef WORLD__TILE__HPP
#define WORLD__TILE__HPP

#include <string>
#include <chrono>

#include <ProtonSDK/Math.h>

#include <World/TileExtra.hpp>

//eTileFlags
#define TILEFLAG_NONE 0x0000
#define TILEFLAG_EXTRA_DATA 0x0001
#define TILEFLAG_LOCKED 0x0002
#define TILEFLAG_SPLICED_TREE 0x0004
#define TILEFLAG_TREE_WILL_DROP_SEED 0x0008
#define TILEFLAG_TREE 0x0010
#define TILEFLAG_FLIPPED 0x0020
#define TILEFLAG_ENABLED 0x0040
#define TILEFLAG_PUBLIC 0x0080
#define TILEFLAG_EXTRA_FRAME 0x0100
#define TILEFLAG_SILENCED 0x0200
#define TILEFLAG_WATER 0x0400
#define TILEFLAG_GLUE 0x0800
#define TILEFLAG_FIRE 0x1000
#define TILEFLAG_RED 0x2000
#define TILEFLAG_GREEN 0x4000
#define TILEFLAG_BLUE 0x8000

//eGameBattleFlags
#define GAMEBATTLEFLAG_NONE 0x0000
#define GAMEBATTLEFLAG_RESPAWN_ON_STORE 0x0001
#define GAMEBATTLEFLAG_RESET_ON_SCORE 0x0002
#define GAMEBATTLEFLAG_OWNER_PLAYS 0x0004
#define GAMEBATTLEFLAG_LATE_JOIN 0x0008
#define GAMEBATTLEFLAG_SMASH_ENEMY_BLOCK 0x0010
#define GAMEBATTLEFLAG_SMASH_OWN_BLOCK 0x0020
#define GAMEBATTLEFLAG_NO_MUSIC 0x0040
#define GAMEBATTLEFLAG_NO_LOG 0x0080
#define GAMEBATTLEFLAG_ENDLESS 0x0100


class ItemInfo;
class TileExtraManager;
class Tile 
{
public:
	Tile(const uint16_t& foreground, const uint16_t& background, const uint16_t& lockIndex, const uint16_t& flags) : m_foreground(foreground), m_background(background), m_parent(lockIndex), m_flags(flags) 
	{ 
		//
	}
	Tile() = default;
	~Tile() = default;

	uint16_t GetForeground() const { return m_foreground; }
	void SetForeground(const uint16_t& tileID);

	uint16_t GetBackground() const { return m_background; }
	void SetBackground(const uint16_t& tileID) { m_background = tileID; }

	uint16_t GetParent() const { return m_parent; }
	void SetParent(const uint16_t& lockIndex) { m_parent = lockIndex; }

	uint16_t GetFlags() const { return m_flags; }
	void SetFlags(const uint16_t& flags) { m_flags = flags; }
	bool HasFlag(const uint16_t& flag) { return m_flags & flag; }
	void ToggleFlag(const uint16_t& flag, const bool& bEnable) {
		if (!bEnable && HasFlag(flag))
			m_flags &= ~flag;
		if (bEnable && !HasFlag(flag))
			m_flags |= flag;
	}

	uint16_t GetLockIndex() const { return m_lockIndex; }
	void SetLockIndex(const uint16_t& lockIndex) { m_lockIndex = lockIndex; }

	bool IsLocked();
	bool IsAdmin(Player* pPlayer);
	bool IsOwner(Player* pPlayer);

	uint16_t GetIndex() const { return m_index; }
	void SetIndex(const uint16_t& index) { m_index = index; }

	void ResetExtra() 
	{
		if (HasFlag(TILEFLAG_EXTRA_DATA)) 
		{
			delete m_pExtra;
			ToggleFlag(TILEFLAG_EXTRA_DATA, false);
		}
	}

	uint8_t GetDamage(const bool& bIncrement = false) { return bIncrement ? ++m_damage : m_damage; }
	void SetDamage(const uint8_t& damage) { m_damage = damage; }

	ItemInfo* GetItem();
	TileExtra* GetTileExtra() { return m_pExtra; }
	bool IsSteamPunk() const;

	std::chrono::steady_clock::time_point GetTick() { return m_tick; }
	void SetTick(const std::chrono::steady_clock::time_point& tick) { m_tick = tick; }

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23);
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23);
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23);

private:
	uint16_t m_foreground = 0;
	uint16_t m_background = 0;
	uint16_t m_parent = 0;
	uint16_t m_flags = 0;

	uint16_t m_lockIndex = 0;
	uint16_t m_index = 0;

	TileExtra* m_pExtra = nullptr;

	uint8_t m_damage = 0;
	std::chrono::steady_clock::time_point m_tick;
};

#endif // WORLD__TILE__HPP