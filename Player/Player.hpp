#ifndef PLAYER__PLAYER__HPP
#define PLAYER__PLAYER__HPP
#include <string>

#include <enet/enet.h>

#include <Player/TankInfo.hpp>
#include <Player/PlayerItems.hpp>
#include <Components/PlayerCharacterState.hpp>
#include <Components/PlayerAdventure.hpp>
#include <Components/PlayerFriends.hpp>

#include <Packet/GameUpdatePacket.hpp>
#include <ProtonSDK/Variant.h>

#define PLAYERGROWMOJI_FLAGS1_SIGH 0x00000001 // ď
#define PLAYERGROWMOJI_FLAGS1_MAD 0x00000002 // Đ
#define PLAYERGROWMOJI_FLAGS1_SMILE 0x00000004 // Ā
#define PLAYERGROWMOJI_FLAGS1_TONGUE 0x00000008 // Ĉ
#define PLAYERGROWMOJI_FLAGS1_WOW 0x00000010 // đ
#define PLAYERGROWMOJI_FLAGS1_NO 0x00000020 // ă
#define PLAYERGROWMOJI_FLAGS1_SHY 0x00000040 // Ć
#define PLAYERGROWMOJI_FLAGS1_WINK 0x00000080 // ć
#define PLAYERGROWMOJI_FLAGS1_MUSIC 0x00000100 // Č
#define PLAYERGROWMOJI_FLAGS1_LOL 0x00000200 // Ě
#define PLAYERGROWMOJI_FLAGS1_YES 0x00000400 // Ă
#define PLAYERGROWMOJI_FLAGS1_LOVE 0x00000800 // Ą
#define PLAYERGROWMOJI_FLAGS1_MEGAPHONE 0x00001000 // Ď
#define PLAYERGROWMOJI_FLAGS1_HEART 0x00002000 // ĕ
#define PLAYERGROWMOJI_FLAGS1_COOL 0x00004000 // Ĝ
#define PLAYERGROWMOJI_FLAGS1_KISS 0x00008000 // Ę
#define PLAYERGROWMOJI_FLAGS1_AGREE 0x00010000 // ĉ
#define PLAYERGROWMOJI_FLAGS1_SEE_NO_EVIL 0x00020000 // ē
#define PLAYERGROWMOJI_FLAGS1_DANCE 0x00020000 // Ē
#define PLAYERGROWMOJI_FLAGS1_BUILD 0x00040000 // č
#define PLAYERGROWMOJI_FLAGS1_OOPS 0x00080000 // ą
#define PLAYERGROWMOJI_FLAGS1_SLEEP 0x00100000 // Ċ
#define PLAYERGROWMOJI_FLAGS1_PUNCH 0x00200000 // ċ
#define PLAYERGROWMOJI_FLAGS1_BHEART 0x00400000 // Ĕ
#define PLAYERGROWMOJI_FLAGS1_CRY 0x00800000 // ĝ
#define PLAYERGROWMOJI_FLAGS1_PARTY 0x01000000 // Ĩ
#define PLAYERGROWMOJI_FLAGS1_WL 0x02000000 // ā
#define PLAYERGROWMOJI_FLAGS1_GROW 0x04000000 // Ė
#define PLAYERGROWMOJI_FLAGS1_GEMS 0x08000000 // ė
#define PLAYERGROWMOJI_FLAGS1_GTOKEN 0x10000000 // ę
#define PLAYERGROWMOJI_FLAGS1_VEND 0x20000000 // Ğ
#define PLAYERGROWMOJI_FLAGS1_BUNNY 0x40000000 // ě
#define PLAYERGROWMOJI_FLAGS1_CACTUS 0x80000000 // ğ

#define PLAYERGROWMOJI_FLAGS2_PEACE 0x00000001 // ģ
#define PLAYERGROWMOJI_FLAGS2_TERROR 0x00000002 // ġ
#define PLAYERGROWMOJI_FLAGS2_TROLL 0x00000004 // Ġ
#define PLAYERGROWMOJI_FLAGS2_HALO 0x00000008 // Į
#define PLAYERGROWMOJI_FLAGS2_NUKE 0x00000010 // ĭ
#define PLAYERGROWMOJI_FLAGS2_PINE 0x00000020 // Ĥ
#define PLAYERGROWMOJI_FLAGS2_FOOTBALL 0x00000040 // ĥ
#define PLAYERGROWMOJI_FLAGS2_FIREWORKS 0x00000080 // Ħ
#define PLAYERGROWMOJI_FLAGS2_SONGPYEON 0x00000100 // ī
#define PLAYERGROWMOJI_FLAGS2_GHOST 0x00000200 // Ĭ
#define PLAYERGROWMOJI_FLAGS2_EVIL 0x00000400 // Ģ
#define PLAYERGROWMOJI_FLAGS2_PIZZA 0x00000800 // ĩ
#define PLAYERGROWMOJI_FLAGS2_ALIEN 0x00001000 // ħ
#define PLAYERGROWMOJI_FLAGS2_CLAP 0x00002000 // Ī
#define PLAYERGROWMOJI_FLAGS2_TURKEY 0x00004000 // į
#define PLAYERGROWMOJI_FLAGS2_GIFT 0x00008000 // İ
#define PLAYERGROWMOJI_FLAGS2_CAKE 0x00010000 // ı
#define PLAYERGROWMOJI_FLAGS2_HEART_ARROW 0x00020000 // Ĳ
#define PLAYERGROWMOJI_FLAGS2_SHAMROCK 0x00040000 // Ĵ
#define PLAYERGROWMOJI_FLAGS2_GRIN 0x00080000 // ĵ
#define PLAYERGROWMOJI_FLAGS2_ILL 0x00100000 // Ķ
#define PLAYERGROWMOJI_FLAGS2_EYES 0x00200000 // ķ
#define PLAYERGROWMOJI_FLAGS2_WEARY 0x00400000 // ĸ
#define PLAYERGROWMOJI_FLAGS2_LUCKY 0x00800000 // ĳ
#define PLAYERGROWMOJI_FLAGS2_MOYAI 0x01000000 // ļ
#define PLAYERGROWMOJI_FLAGS2_PLEAD 0x02000000 // Ľ
#define PLAYERGROWMOJI_FLAGS2_TROPHY 0x04000000 // Ļ
#define PLAYERGROWMOJI_FLAGS2_SIGNAL 0x08000000 // ĺ
#define PLAYERGROWMOJI_FLAGS2_INFO 0x10000000 // Ĺ


#define PLAYERTEMPFLAG_UPDATING_ITEMS 0x1
#define PLAYERTEMPFLAG_INGAME 0x2
#define PLAYERTEMPFLAG_FACING_LEFT 0x4
#define PLAYERTEMPFLAG_INWORLD 0x8
#define PLAYERTEMPFLAG_INVIS 0x10

#define PLAYERFLAG_SUPPORTER 0x1
#define PLAYERFLAG_SUPER_SUPPORTER 0x2
#define PLAYERFLAG_NAMELOCK 0x4 //when bit enabled, user has forced name which he CANNOT change, applied by SMOD via /rename
#define PLAYERFLAG_PURPLE_SKIN 0x8
#define PLAYERFLAG_BLUE_SKIN 0x10

#define PLAYERFLAG_BLUE_NAME_TITLE 0x20
#define PLAYERFLAG_BLUE_NAME_TITLE_ENABLED 0x40


class World;
class Player
{
public:
	Player(ENetPeer* pPeer);
	~Player();


	void Disconnect(const enet_uint32& timeout, const bool& bNow = false);

	void SendHello();
	int GetOnlineID() const { return m_onlineID; }
	void SetOnlineID(const int& id) { m_onlineID = id; }


	int GetUserID() const { return m_userID; }
	void SetUserID(const int& id) { m_userID = id; }

	int GetNetID() const { return m_netID; }
	void SetNetID(const int& id) { m_netID = id; }


	uint16_t GetAccountID() const { return m_accountID; }
	void SetAccountID(const uint16_t& id) { m_accountID = id; }


	uint8_t GetRole() const { return m_role; }
	void SetRole(const uint8_t& role) { m_role = role; }


	int GetTempBits() const { return m_tempBits; }
	void SetTempBits(const int& bits) { m_tempBits = bits; }
	bool HasTempBit(const int& bit) const { return m_tempBits & bit; }
	void ToggleTempBit(const int& bit, const bool& bEnable)
	{
		if (HasTempBit(bit) && !bEnable)
		{
			m_tempBits &= ~bit;
		}
		if (!HasTempBit(bit) && bEnable)
		{
			m_tempBits |= bit;
		}
	}


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

	int GetTokens() const { return m_tokens; }
	void SetTokens(const int& tokens) { m_tokens = tokens; }


	ENetPeer* GetPeer() const { return m_pPeer; }
	TankInfo* GetTankInfo() { return &m_tank; }
	std::string GetName();
	std::string GetDisplayName(World* pWorld);
	std::string GetStaticDisplayName();
	std::string GetCountryState();
	std::string GetTitleIcon();


	World* GetWorld() const { return m_pWorld; }
	void SetWorld(World* pWorld);


	PlayerCharacterState& GetCharacterState() { return m_characterState; }
	PlayerAdventure& GetAdventure() { return m_adventure; }
	PlayerItems* GetItems() { return m_pItems; }
	PlayerFriends& GetFriends() { return m_friends; }


	int GetHealth() const { return m_health; }
	void SetHealth(const int& health) { m_health = health; }


	uint8_t GetHitPower() const { return m_hitPower; }
	void SetHitPower(const uint8_t& hitPower) { m_hitPower = hitPower; }


	int GetLevel() const { return m_level; }
	void SetLevel(const int& level) { m_level = level; }


	int GetXP() const { return m_xp; }
	void SetXP(const int& xp) { m_xp = xp; }


	unsigned int GetSkinColor() const { return m_skinColor; }
	void SetSkinColor(unsigned int color) { m_skinColor = color; }


	std::string GetHome() const { return m_homeWorld; }
	void SetHome(const std::string& homeWorld) { m_homeWorld = homeWorld; }


	std::string GetCreatorCode() const { return m_creatorCode; }
	void SetCreatorCode(const std::string& creatorCode) { m_creatorCode = creatorCode; }

	std::string GetLastVisitedWorld() const { return m_lastVisitedWorld == "EXIT" ? "START" : m_lastVisitedWorld; }
	void SetLastVisitedWorld(const std::string& lastWorld) { m_lastVisitedWorld = lastWorld; }

	uint16_t GetMagnetItem() const { return m_magnet; }
	void SetMagnetItem(const uint16_t& magnet) { m_magnet = magnet; }

	std::chrono::steady_clock::time_point GetRegenTick() const { return m_regenTick; }
	void OnRegenTick() { m_regenTick = std::chrono::steady_clock::now(); }


	std::chrono::steady_clock::time_point GetHomeTick() const { return m_homeTick; }
	void OnHomeTick() { m_homeTick = std::chrono::steady_clock::now(); }



	//inventory related funcs
	uint8_t GetInventoryItemCount(const uint16_t& itemID);
	bool AddInventoryItem(const uint16_t& itemID, const uint8_t& count, const bool& bUpdateState = false);
	bool RemoveInventoryItem(const uint16_t& itemID, const uint8_t& count, const bool& bUpdateState = false);
	size_t GetInventoryMemoryEstimated();


	void SendPacketRaw(eNetMessageType type, const void* pData, const uintmax_t& dataLength, const enet_uint32& flags = ENET_PACKET_FLAG_RELIABLE);
	void SendPacket(eNetMessageType type, const std::string& text);
	void SendVariantPacket(VariantList variant, const int& netID = -1, const int& delayMS = 0);
	void OnEmoticonDataChanged();
	void SendInventoryState();
	void SendTileSelect(CL_Vec2f vec = { -1.f, -1.f }, std::vector<int> indexes = {});
	void SendWrench(const int& netID, Player* pTargetPlayer);
	void OnSetBux(const bool& bIncrement = false);

public:
	int m_growmojiFlags = 0;
	int m_growmojiFlags2 = 0;

private:
	ENetPeer* m_pPeer = nullptr;
	int m_onlineID = 0;
	int m_userID = -1;
	int m_netID = -1;
	int m_bits = 0;
	int m_tempBits = 0;

	int m_tokens = 0;



	uint16_t m_accountID = -1;
	uint8_t m_role = ROLE_PLAYER;



	TankInfo m_tank;
	World* m_pWorld = nullptr;
	PlayerCharacterState m_characterState;
	PlayerAdventure m_adventure;
	PlayerItems* m_pItems = nullptr;
	PlayerFriends m_friends;


	uint8_t m_hitPower = 6;
	int m_health = 200;



	int m_level = 1;
	int m_xp = 0;
	unsigned int m_skinColor = 2022356223;



	std::string m_homeWorld = "EXIT";
	std::string m_creatorCode = "";
	std::string m_nameOverride = "";
	std::string m_lastVisitedWorld = "";



	uint16_t m_magnet = ITEM_ID_BLANK;



	//ticks
	std::chrono::steady_clock::time_point m_regenTick = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point m_homeTick = std::chrono::steady_clock::now();

};

std::pair<size_t, uint8_t*> Player::PackPlayerData(const uint8_t& dataType)
{
	switch (dataType)
	{
	case PLAYERDATA_INVENTORY:
	{
		size_t dataLength = 0;
		dataLength += sizeof(int); //PlayerItems::m_backpackSlots
		dataLength += sizeof(uint16_t); //PlayerItems::m_items.size()
		for (const auto& it : m_items.GetItems())
		{
			dataLength += sizeof(uint16_t); //PlayerInventoryItem::itemID
			dataLength += sizeof(uint8_t); //PlayerInventoryItem::count
			dataLength += sizeof(uint8_t); //PlayerInventoryItem::flags
		}

		uint8_t* pData = new (std::nothrow) uint8_t[dataLength];
		int offset = 0;
		int slots = GetItems()->GetBackpackSlots();
		MemorySerializeRaw(slots, pData, offset, true);

		uint16_t size = (uint16_t)m_items.GetItems().size();
		MemorySerializeRaw(size, pData, offset, true);
		for (int i = 0; i < size; i++)
		{
			PlayerInventoryItem it = m_items.GetItems().at(i);
			ItemInfo* pItem = GetItemInfoManager()->GetItem(it.itemID);

			MemorySerializeRaw(it.itemID, pData, offset, true);
			MemorySerializeRaw(it.count, pData, offset, true);

			uint8_t flags = 0;
			if (pItem && pItem->type == TYPE_CLOTHES)
			{
				for (int i = 0; i < NUM_CLOTHES; i++)
				{
					if (m_items.GetCloth((eClothingType)i) == it.itemID)
					{
						flags = 0x1;
					}
				}
			}

			MemorySerializeRaw(it.flags, pData, offset, true);
		}

		return std::make_pair(dataLength, pData);
	}

	case PLAYERDATA_CLOTHES:
	{
		size_t dataLength = sizeof(short) * NUM_CLOTHES;
		uint8_t* pData = new (std::nothrow) uint8_t[dataLength];
		int offset = 0;

		for (int i = 0; i < NUM_CLOTHES; i++)
		{
			short item = m_items.GetCloth((eClothingType)i);
			MemorySerializeRaw(item, pData, offset, true);
		}

		return std::make_pair(dataLength, pData);
	}

	case PLAYERDATA_CHARACTER_STATE:
	{
		size_t dataLength = sizeof(int) * 2;
		uint8_t* pData = new (std::nothrow) uint8_t[dataLength];
		int offset = 0;

		int state = m_cState.GetStateFlags();
		int effect = m_cState.GetEffectFlags();
		MemorySerializeRaw(state, pData, offset, true);
		MemorySerializeRaw(effect, pData, offset, true);

		return std::make_pair(dataLength, pData);
	}

	case PLAYERDATA_PLAYER_MODS:
	{
		size_t dataLength = 4;

		for (int i = 0; i < m_activeEffects.size(); i++)
		{
			dataLength += sizeof(int); // modID
			dataLength += sizeof(uint64_t); // time point
			dataLength += sizeof(int); // duration
			dataLength += sizeof(bool) * 2; // permanent / active
		}

		uint8_t* pPlayerModsData = new (std::nothrow) uint8_t[dataLength];
		int offset = 0;
		int mods = (int)m_activeEffects.size();
		MemorySerializeRaw(mods, pPlayerModsData, offset, true);
		for (int i = 0; i < m_activeEffects.size(); i++)
		{
			PlayerMod mod = m_activeEffects[i];
			int ID = mod.GetID();
			MemorySerializeRaw(ID, pPlayerModsData, offset, true);

			uint64_t time = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(mod.GetTick().time_since_epoch()).count());
			MemorySerializeRaw(time, pPlayerModsData, offset, true);

			int dur = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(mod.GetDuration()).count());
			MemorySerializeRaw(dur, pPlayerModsData, offset, true);

			bool bPermanent = mod.IsPermanent();
			bool bActive = mod.IsActive();
			MemorySerializeRaw(bPermanent, pPlayerModsData, offset, true);
			MemorySerializeRaw(bActive, pPlayerModsData, offset, true);
		}

		return std::make_pair(dataLength, pPlayerModsData);
	}

	case PLAYERDATA_ACHIEVEMENTS:
	{
		size_t dataLength = 4;
		for (int i = 0; i < m_aList.GetAchievements().size(); i++)
		{
			const PlayerAchievement& alist = m_aList.GetAchievements().at(i);
			dataLength += sizeof(int); // alist.type
			dataLength += sizeof(int); // alist.progress
			dataLength += sizeof(bool); // alist.bAchieved
		}

		uint8_t* pData = new (std::nothrow) uint8_t[dataLength];
		int offset = 0;

		int size = m_aList.GetAchievements().size();
		MemorySerializeRaw(size, pData, offset, true);
		for (int i = 0; i < m_aList.GetAchievements().size(); i++)
		{
			PlayerAchievement alist = m_aList.GetAchievements().at(i);
			MemorySerializeRaw(alist.type, pData, offset, true);
			MemorySerializeRaw(alist.progress, pData, offset, true);
			MemorySerializeRaw(alist.bAchieved, pData, offset, true);
		}

		return std::make_pair(dataLength, pData);
	}
	}

	return std::make_pair(0, nullptr);
}

void Player::LoadPlayerData(const uint8_t& dataType, uint8_t* pData)
{
	if (pData == NULL)
	{
		return;
	}

	int offset = 0;
	switch (dataType)
	{
	case PLAYERDATA_INVENTORY:
	{
		int slots = BACKPACK_INITIAL_SIZE;
		MemorySerializeRaw(slots, pData, offset, false);
		m_items.SetBackpackSlots(slots);

		uint16_t sizeOfItems = 0;
		MemorySerializeRaw(sizeOfItems, pData, offset, false);

		for (int i = 0; i < sizeOfItems; i++)
		{
			PlayerInventoryItem it;
			MemorySerializeRaw(it.itemID, pData, offset, false);
			MemorySerializeRaw(it.count, pData, offset, false);
			MemorySerializeRaw(it.flags, pData, offset, false);

			m_items.AddInventoryItem(it.itemID, it.count);
		}

		delete[] pData;
		break;
	}

	case PLAYERDATA_CLOTHES:
	{
		for (int i = 0; i < NUM_CLOTHES; i++)
		{
			short item = 0;
			MemorySerializeRaw(item, pData, offset, false);
			m_items.SetCloth((eClothingType)i, item);
		}

		delete[] pData;
		break;
	}

	case PLAYERDATA_CHARACTER_STATE:
	{
		int states = 0;
		int effects = 0;
		MemorySerializeRaw(states, pData, offset, false);
		MemorySerializeRaw(effects, pData, offset, false);

		m_cState.SetStateFlags(states);
		m_cState.SetEffectFlags(effects);
		delete[] pData;
		break;
	}

	case PLAYERDATA_PLAYER_MODS:
	{
		int mods = 0;
		MemorySerializeRaw(mods, pData, offset, false);

		for (int i = 0; i < mods; i++)
		{
			int ID = 0;
			uint64_t time = 0;
			int durr = 0;
			bool bPermanent = false;
			bool bActive = false;

			MemorySerializeRaw(ID, pData, offset, false);
			MemorySerializeRaw(time, pData, offset, false);
			MemorySerializeRaw(durr, pData, offset, false);
			MemorySerializeRaw(bPermanent, pData, offset, false);
			MemorySerializeRaw(bActive, pData, offset, true);

			PlayerMod mod(ID, std::chrono::steady_clock::time_point{ std::chrono::seconds(time) }, std::chrono::steady_clock::duration(std::chrono::seconds(durr)));
			mod.SetIsPermanent(bPermanent);
			mod.SetIsActive(bActive);
			m_activeEffects.push_back(mod);
		}

		delete[] pData;
		break;
	}

	case PLAYERDATA_ACHIEVEMENTS:
	{
		int achievements = 0;
		MemorySerializeRaw(achievements, pData, offset, false);

		for (int i = 0; i < achievements; i++)
		{
			PlayerAchievement alist;
			MemorySerializeRaw(alist.type, pData, offset, false);
			MemorySerializeRaw(alist.progress, pData, offset, false);
			MemorySerializeRaw(alist.bAchieved, pData, offset, false);
			m_aList.GetAchievements().at(i) = alist;
		}

		delete[] pData;
		break;
	}
	}
}

bool Player::InsertIntoDB()
{
	try
	{
		sql::PreparedStatement* pQuery = GetDatabase()->GetConnection()->prepareStatement("INSERT INTO players(loginSecret, tankIDName, requestedName, tankIDPass, rawName, lastLoginSecret, flags, level, xp, gems, lastVisitedWorld, lastServerAttended, inventory, clothes, characterState, accountID, skinColor, hoursPlayed, createdAt, playerMods, alist, adminLevel) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		if (!pQuery)
		{
			Error("Cache::InsertOrUpdatePlayer insert error");
			return false;
		}

		bool bIsGuestAccount = m_accountID != -1;
		std::string secret = base64_encode("" + m_data.rid + "|" + m_data.mac + "|");

		pQuery->setString(1, secret); //identifies 2fa crap and guest account ownership
		pQuery->setString(2, m_data.tankIDName);
		pQuery->setString(3, m_data.requestedName);
		pQuery->setString(4, m_data.tankIDPass);
		pQuery->setString(5, GetStaticDisplayName());
		pQuery->setString(6, secret); //backup secret to verify prev 2fa
		pQuery->setInt(7, m_bits);
		pQuery->setInt(8, m_level);
		pQuery->setInt(9, m_xp);
		pQuery->setInt(10, m_items.GetGems());
		pQuery->setString(11, m_lastVisitedWorld);
		pQuery->setInt(12, m_serverID);

		auto inventoryData = PackPlayerData(PLAYERDATA_INVENTORY);
		auto clothesData = PackPlayerData(PLAYERDATA_CLOTHES);
		auto stateData = PackPlayerData(PLAYERDATA_CHARACTER_STATE);
		auto modsData = PackPlayerData(PLAYERDATA_PLAYER_MODS);
		auto alistData = PackPlayerData(PLAYERDATA_ACHIEVEMENTS);
		std::stringstream inventoryStream = std::stringstream(std::string(reinterpret_cast<const char*>(inventoryData.second), inventoryData.first));
		std::stringstream clothesStream = std::stringstream(std::string(reinterpret_cast<const char*>(clothesData.second), clothesData.first));
		std::stringstream stateStream = std::stringstream(std::string(reinterpret_cast<const char*>(stateData.second), stateData.first));
		std::stringstream modsStream = std::stringstream(std::string(reinterpret_cast<const char*>(modsData.second), modsData.first));
		std::stringstream alistStream = std::stringstream(std::string(reinterpret_cast<const char*>(alistData.second), alistData.first));

		pQuery->setBlob(13, &inventoryStream);
		pQuery->setBlob(14, &clothesStream);
		pQuery->setBlob(15, &stateStream);
		pQuery->setInt(16, m_accountID);
		pQuery->setUInt(17, m_skinColor);
		pQuery->setUInt(18, m_hoursPlayed);
		pQuery->setInt(19, std::chrono::duration_cast<std::chrono::seconds>(m_createdAt.time_since_epoch()).count());
		pQuery->setBlob(20, &modsStream);
		pQuery->setBlob(21, &alistStream);
		pQuery->setInt(22, m_adminLevel);

		pQuery->execute();
		delete[] inventoryData.second;
		delete[] clothesData.second;
		delete[] stateData.second;
		delete pQuery;

		sql::ResultSet* pResult = nullptr;
		if (m_accountID == -1)
		{
			pResult = GetDatabase()->Query("SELECT * FROM `players` WHERE tankIDName='" + m_data.tankIDName + "' AND accountID='-1'");
		}
		else
		{
			pResult = GetDatabase()->Query("SELECT * FROM `players` WHERE loginSecret='" + secret + "' AND accountID!='-1'");
		}

		if (!pResult)
		{
			return false;
		}

		if (pResult->rowsCount() < 1 || !pResult->next())
		{
			delete pResult;
			return false;
		}

		MyResult res(pResult);
		m_userID = res.GetInt("ID");
		delete pResult;
		return true;
	}
	catch (const sql::SQLException& e)
	{
		Error("Player::InsertToDB SQL error > %s (MySQL error code: %d)", e.what(), e.getErrorCode());
		return false;
	}
	catch (const std::exception& e)
	{
		Error("Player::InsertToDB error > %s", e.what());
		return false;
	}

	return false;
}

void Player::SaveToDB()
{
	try
	{
		std::stringstream ss;
		ss << "UPDATE `players` SET " <<
			"`tankIDName` = (?), " <<
			"`requestedName` = (?), " <<
			"`tankIDPass` = (?), " <<
			"`rawName` = (?), " <<
			"`lastLoginSecret` = (?), " <<
			"`flags` = (?), " <<
			"`level` = (?), " <<
			"`xp` = (?), " <<
			"`gems` = (?), " <<
			"`lastVisitedWorld` = (?), " <<
			"`lastServerAttended` = (?), " <<
			"`inventory` = (?), " <<
			"`clothes` = (?), " <<
			"`characterState` = (?), " <<
			"`accountID` = (?), " <<
			"`skinColor` = (?), " <<
			"`hoursPlayed` = (?), " <<
			"`playerMods` = (?), " <<
			"`alist` = (?), " <<
			"`adminLevel` = (?) " <<
			"WHERE `ID` = (?)";

		sql::PreparedStatement* pQuery = GetDatabase()->GetConnection()->prepareStatement(ss.str().c_str());
		if (!pQuery)
		{
			return;
		}

		pQuery->setString(1, m_data.tankIDName);
		pQuery->setString(2, m_data.requestedName);
		pQuery->setString(3, m_data.tankIDPass);
		pQuery->setString(4, GetStaticDisplayName());
		pQuery->setString(5, base64_encode(m_data.rid + "|" + m_data.mac + "|"));
		pQuery->setInt(6, m_bits);
		pQuery->setInt(7, m_level);
		pQuery->setInt(8, m_xp);
		pQuery->setInt(9, m_items.GetGems());
		pQuery->setString(10, m_lastVisitedWorld);
		pQuery->setInt(11, m_serverID);

		auto inventoryData = PackPlayerData(PLAYERDATA_INVENTORY);
		auto clothesData = PackPlayerData(PLAYERDATA_CLOTHES);
		auto stateData = PackPlayerData(PLAYERDATA_CHARACTER_STATE);
		auto modsData = PackPlayerData(PLAYERDATA_PLAYER_MODS);
		auto alistData = PackPlayerData(PLAYERDATA_ACHIEVEMENTS);
		std::stringstream inventoryStream = std::stringstream(std::string(reinterpret_cast<const char*>(inventoryData.second), inventoryData.first));
		std::stringstream clothesStream = std::stringstream(std::string(reinterpret_cast<const char*>(clothesData.second), clothesData.first));
		std::stringstream stateStream = std::stringstream(std::string(reinterpret_cast<const char*>(stateData.second), stateData.first));
		std::stringstream modsStream = std::stringstream(std::string(reinterpret_cast<const char*>(modsData.second), modsData.first));
		std::stringstream alistStream = std::stringstream(std::string(reinterpret_cast<const char*>(alistData.second), alistData.first));

		pQuery->setBlob(12, &inventoryStream);
		pQuery->setBlob(13, &clothesStream);
		pQuery->setBlob(14, &stateStream);
		pQuery->setInt(15, m_accountID);
		pQuery->setUInt(16, m_skinColor);
		pQuery->setUInt(17, m_hoursPlayed);
		pQuery->setBlob(18, &modsStream);
		pQuery->setBlob(19, &alistStream);
		pQuery->setInt(20, m_adminLevel);
		pQuery->setInt(21, m_userID);

		pQuery->executeUpdate();
		delete[] inventoryData.second;
		delete[] clothesData.second;
		delete[] stateData.second;
		delete pQuery;
	}
	catch (const sql::SQLException& e)
	{
		Error("Player::SaveToDB SQL error > %s (MySQL error code: %d)", e.what(), e.getErrorCode());
		return;
	}
	catch (const std::exception& e)
	{
		Error("Player::SaveToDB error > %s", e.what());
		return;
	}
}

bool Player::LoadFromDB(const bool& bGuestAccount, const std::string& tankIDPass)
{
	try
	{
		sql::ResultSet* pResult = nullptr;
		std::string secret = base64_encode(m_data.rid + "|" + m_data.mac + "|");
		if (!bGuestAccount)
		{
			pResult = GetDatabase()->Query("SELECT * FROM `players` WHERE tankIDName='" + m_data.tankIDName + "' AND accountID='-1'");
		}
		else
		{
			pResult = GetDatabase()->Query("SELECT * FROM `players` WHERE loginSecret='" + secret + "' AND accountID!='-1'");
		}

		if (!pResult)
		{
			return false;
		}

		if (pResult->rowsCount() < 1 || !pResult->next())
		{
			delete pResult;
			return false;
		}

		MyResult sql(pResult);
		if (bGuestAccount && sql.GetString("loginSecret") != secret)
		{
			delete pResult;
			return false;
		}

		if (!bGuestAccount)
		{
			std::string lowName = Utils::NormalizeString(m_data.tankIDName);
			std::string lowDBName = Utils::NormalizeString(sql.GetString("tankIDName"));
			//std::string hash = Utils::HashPassword(sql.GetInt("ID"), tankIDPass);
			if (lowName != lowDBName || tankIDPass != sql.GetString("tankIDPass"))
			{
				delete pResult;
				return false;
			}

			m_data.tankIDName = sql.GetString("tankIDName");
		}

		// loading normal data
		m_userID = sql.GetInt("ID");
		m_bits = sql.GetInt("flags");
		m_level = sql.GetInt("level");
		m_xp = sql.GetInt("xp");
		m_items.SetGems(sql.GetInt("gems"));
		m_lastVisitedWorld = sql.GetString("lastVisitedWorld");
		m_accountID = sql.GetInt("accountID");
		m_skinColor = sql.GetUInt("skinColor");
		m_hoursPlayed = sql.GetUInt("hoursPlayed");
		uint64_t createdAt = sql.GetLongLong("createdAt");
		m_createdAt = std::chrono::steady_clock::time_point(std::chrono::seconds(createdAt));
		m_adminLevel = sql.GetInt("adminLevel");

		// loading blob datas
		size_t zero = 0;
		LoadPlayerData(PLAYERDATA_INVENTORY, sql.GetBuffer("inventory", zero));
		zero = 0;
		LoadPlayerData(PLAYERDATA_CLOTHES, sql.GetBuffer("clothes", zero));
		zero = 0;
		LoadPlayerData(PLAYERDATA_CHARACTER_STATE, sql.GetBuffer("characterState", zero));
		zero = 0;
		LoadPlayerData(PLAYERDATA_PLAYER_MODS, sql.GetBuffer("playerMods", zero));
		zero = 0;
		LoadPlayerData(PLAYERDATA_ACHIEVEMENTS, sql.GetBuffer("alist", zero));

		delete pResult;
		return true;
	}
	catch (const sql::SQLException& e)
	{
		Error("Player::LoadFromDB SQL error > %s (MySQL error code: %d)", e.what(), e.getErrorCode());
		return false;
	}
	catch (const std::exception& e)
	{
		Error("Player::LoadFromDB error > %s", e.what());
		return false;
	}

	return false;
}

#endif // PLAYER__PLAYER__HPP