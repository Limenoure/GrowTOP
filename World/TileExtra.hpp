#ifndef WORLD__TILEEXTRA__HPP
#define WORLD__TILEEXTRA__HPP

#include <string>
#include <array>
#include <random>
#include <chrono>
#include <vector>
#include <unordered_map>

#include <ProtonSDK/MiscUtils.h>


enum eTileExtraType 
{
	TILEEXTRA_NONE,
	TILEEXTRA_DOOR,
	TILEEXTRA_SIGN,
	TILEEXTRA_LOCK,
	TILEEXTRA_TREE,
	TILEEXTRA_UNK_5,
	TILEEXTRA_MAILBOX,
	TILEEXTRA_BULLETIN,
	TILEEXTRA_DICE,
	TILEEXTRA_PROVIDER,
	TILEEXTRA_ACHIEVEMENT_BLOCK,
	TILEEXTRA_HEART_MONITOR,
	TILEEXTRA_DONATION_BOX,
	TILEEXTRA_TOY_BOX, //unhandled
	TILEEXTRA_MANNEQUIN,
	TILEEXTRA_MAGIC_EGG,
	TILEEXTRA_GAME_BLOCK,
	TILEEXTRA_GAME_GENERATOR,
	TILEEXTRA_XENONITE,
	TILEEXTRA_DRESSUP,
	TILEEXTRA_CRYSTAL,
	TILEEXTRA_BURGLAR, //unhandled
	TILEEXTRA_SPOTLIGHT,
	TILEEXTRA_DISPLAY_BLOCK,
	TILEEXTRA_VENDING_MACHINE,
	TILEEXTRA_FISHTANK,
	TILEEXTRA_SOLAR,
	TILEEXTRA_FORGE,
	TILEEXTRA_GIVING_TREE,
	TILEEXTRA_GIVING_TREE_STUMP, //unhandled
	TILEEXTRA_STEAM_ORGAN, //unhandled
	TILEEXTRA_SILKWORM,
	TILEEXTRA_SEWING_MACHINE,
	TILEEXTRA_FLAG,
	TILEEXTRA_LOBSTER_TRAP,
	TILEEXTRA_ART_CANVAS,
	TILEEXTRA_BATTLE_CAGE,
	TILEEXTRA_PET_TRAINER, //unhandled
	TILEEXTRA_STEAM_ENGINE,
	TILEEXTRA_LOCK_BOT,
	TILEEXTRA_BACKGROUND_WEATHER,
	TILEEXTRA_SPIRIT_STORAGE,
	TILEEXTRA_DATA_BEDROCK, //unhandled because of different serializing on every game version
	TILEEXTRA_DISPLAY_SHELF,
	TILEEXTRA_VIP_TIMER,
	TILEEXTRA_CHALLENGE_TIMER,
	TILEEXTRA_UNK_46,
	TILEEXTRA_FISH_MOUNT,
	TILEEXTRA_PORTRAIT,
	TILEEXTRA_STUFF_WEATHER,
	TILEEXTRA_FOSSIL_PREP,
	TILEEXTRA_DNA_MACHINE,
	TILEEXTRA_TRICKSTER,
	TILEEXTRA_CHEMTANK,
	TILEEXTRA_STORAGE,
	TILEEXTRA_OVEN,
	TILEEXTRA_SUPER_MUSIC,
	TILEEXTRA_GEIGER_CHARGER,
	TILEEXTRA_ADVENTURE_RESET,
	TILEEXTRA_TOMB_ROBBER,
	TILEEXTRA_FACTION, //unhandled
	TILEEXTRA_TRAINING_FISH_PORT,
	TILEEXTRA_ITEM_SUCKER,
	TILEEXTRA_ROBOT,
	TILEEXTRA_TICKET, //unhandled
	TILEEXTRA_GUILD_ITEM,
	TILEEXTRA_STATS_BLOCK,
	TILEEXTRA_FIELD_NODE,  //unhandled
	TILEEXTRA_OUIJA_BOARD, //unhandled
	TILEEXTRA_AUTO_BREAK,
	TILEEXTRA_AUTO_HARVEST,
	TILEEXTRA_AUTO_HARVEST_SUCKER,
	TILEEXTRA_LIGHTNING_IF_ON, //unhandled
	TILEEXTRA_PHASED_BLOCK,
	TILEEXTRA_SAFE_VAULT,
	TILEEXTRA_PHASED_BLOCK2,
	TILEEXTRA_PVE_NPC, //unhandled
	TILEEXTRA_INFINITY_WEATHER,  //TODO: from here to end
	TILEEXTRA_COMPLETIONIST, //unhandled
	TILEEXTRA_FEEDING_BLOCK, //unhandled
	TILEEXTRA_KRANKENS_BLOCK,
	TILEEXTRA_FRIENDS_ENTRANCE

};

enum eBulletinFlags 
{
	BULLETINFLAG_NONE = -1,
	BULLETINFLAG_PUBLIC = 0x0,
	BULLETINFLAG_HIDE_NAMES = 0x1

};

enum eLockFlags : unsigned char 
{
	LOCKFLAG_NONE = 0x0,
	LOCKFLAG_AREA_LOCK = 0x1,
	LOCKFLAG_DISABLE_MUSIC_NOTE = 0x10,
	LOCKFLAG_INVISIBLE_MUSIC_NOTE = 0x20,
	LOCKFLAG_IGNORE_EMPTY_AIR = 0x20,
	LOCKFLAG_ONLY_BUILDING = 0x40,
	LOCKFLAG_SILENCED_PLEASANTS = 0x40,
	LOCKFLAG_ROYAL_RAINBOW = 0x80,
	LOCKFLAG_RESTRICT_ADMIN = 0x80

};

enum eFishFlags 
{
	FISHFLAG_NONE = 0x0,
	FISHFLAG_GLOW_PERFECT_FISH = 0x10

};

enum eSolarFlags 
{
	SOLARFLAG_NONE = 0x0

};

enum eSilkwormFlags 
{
    SILKWORM_FLAG_NONE = 0,
    SILKWORM_FLAG_DEAD = 1,
    SILKWORM_FLAG_DEVIL_HORNS = 8,
    SILKWORM_FLAG_GOLDEN_HALO = 16

};

enum eSilkwormStates 
{
    SILKWORM_STATE_NONE = 0,
    SILKWORM_STATE_HAPPY = 1,
    SILKWORM_STATE_DANCING = 2,
    SILKWORM_STATE_SMILING_FLOAT = 3,
    SILKWORM_STATE_SMILING = 4,

};

struct DonationInfo
{
	int userID;
	std::string lastName;
	
	uint16_t itemID;
	uint8_t count;
	std::string message;

};

class TileExtra 
{
public:
	TileExtra() = default;
	~TileExtra() = default;

	uint8_t GetType() const { return m_type; }
	void SetType(const uint8_t& type) { m_type = type; }


	virtual size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) = 0;
    virtual void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) = 0;
	virtual void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) = 0;

private:
	uint8_t m_type = 0;

};

class TileExtraDoor : public TileExtra
{
public:
	TileExtraDoor() = default;
	~TileExtraDoor() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	std::string m_label = "";
	uint8_t m_flags = 0;

	std::string m_id = "";
	std::string m_destination = "";

	//password door
	std::string m_password = "";
};

class TileExtraSign : public TileExtra
{
public:
	TileExtraSign() = default;
	~TileExtraSign() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	std::string m_label = "";
	int m_flags = 0;

	//server side
	std::string m_id = "";
};

class TileExtraDonationBox : public TileExtra
{
public:
	TileExtraDonationBox() = default;
	~TileExtraDonationBox() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	int GetGifts(const int& userID) const;

	uint8_t m_donationID = 0;
	std::unordered_map<uint8_t, DonationInfo> m_donations;
};

class TileExtraLock : public TileExtra
{
public:
	TileExtraLock() = default;
	~TileExtraLock() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	bool IsAdmin(Player* pPlayer);

	uint16_t GetFlags() const { return m_flags; }
	void SetFlags(const uint8_t& flags) { m_flags = flags; }
	bool HasFlag(const uint8_t& flag) { return m_flags & flag; }
	void ToggleFlag(const uint8_t& flag, const bool& bEnable) 
	{
		if (!bEnable && HasFlag(flag))
		{
			m_flags &= ~flag;
		}

		if (bEnable && !HasFlag(flag))
		{
			m_flags |= flag;
		}
	}

	uint8_t m_flags = LOCKFLAG_NONE;
	int m_ownerID = -1;

	std::vector<int> m_admins;

	//V8
	int m_tempo = 100; //tempo is written as negative integer in packet(m-tempo * -1)
	int m_entryLevel = 1; //world entry level

};

class TileExtraSeed : public TileExtra
{
public:
	TileExtraSeed() = default;
	~TileExtraSeed() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	uint8_t m_fruits = 4;
	std::chrono::steady_clock::time_point m_growingSince = std::chrono::steady_clock::now();

};

class TileExtraRoll : public TileExtra
{
public:
	TileExtraRoll() = default;
	~TileExtraRoll() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	uint8_t m_roll = 0;

};

class TileExtraDisplayBlock : public TileExtra
{
public:
	TileExtraDisplayBlock() = default;
	~TileExtraDisplayBlock() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	int m_itemID = ITEM_ID_BLANK;

	//server side
	std::chrono::steady_clock::time_point m_placedAt = std::chrono::steady_clock::now();

};

class TileExtraItemSucker: public TileExtra
{
public:
	TileExtraItemSucker() = default;
	~TileExtraItemSucker() = default;

	size_t GetMemoryEstimated(const bool& bPacket = true, const float& fGameVersion = 4.59f, const uint16_t& mapVersion = 23) override;
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true, const float& fGameVersion = 4.51f, const uint16_t& mapVersion = 23) override;
	void SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket = false, const uint16_t& mapVersion = 23) override;

	int m_itemID = ITEM_ID_BLANK;
	int m_count = 0;

	bool m_bMagnet = false;
	bool m_bBuildingMode = false;
	int m_capacity = 1500;

};

#endif // WORLD__TILEEXTRA__HPP