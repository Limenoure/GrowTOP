#include <BaseApp.hpp> //precomp
#include <Player/Player.hpp>

#include <spdlog/fmt/fmt.h>
#include <Packet/GameUpdatePacket.hpp>
#include <ProtonSDK/Variant.h>

#include <World/World.hpp>

Player::Player(ENetPeer* pPeer) : m_pPeer(pPeer)
{
	m_pPeer->data = this;
	enet_address_get_host_ip(&m_pPeer->address, m_tank.address, 16);

	m_pItems = new PlayerItems();
}

Player::~Player()
{
	if (m_pPeer->data)
	{
		delete m_pPeer->data;
	}

	if (m_pItems)
	{
		delete m_pItems;
	}
}

void Player::Disconnect(const enet_uint32 & timeout, const bool& bNow /* = false */)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	g_lastPlayer = m_userID;
	if (!m_pPeer)
	{
		return;
	}

	if (bNow)
	{
		enet_peer_disconnect_now(m_pPeer, timeout);
	} 
	else
	{
		enet_peer_disconnect(m_pPeer, timeout);
	}
}

std::string Player::GetName()
{
	if (m_accountID == -1)
	{
		return m_tank.tankIDName;
	}

	return m_tank.requestedName + "_" + std::to_string(m_accountID);
}
std::string Player::GetDisplayName(World* pWorld)
{
	if (!m_nameOverride.empty())
	{
		return "`w" + m_nameOverride + "``";
	}

	std::string name = GetName();
	RoleInfo role = GetRoleInfo((ePlayerRole)m_role);
	std::string prefix = "`w";
	std::string suffix = role.suffix;

	if (pWorld && pWorld->GetLockIndex() != 0)
	{
		Tile* pParent = pWorld->GetTileMap()->GetTile(pWorld->GetLockIndex());
		if (!pParent)
		{
			goto skip;
		}

		TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pParent->GetTileExtra());
		if (!pExtra)
		{
			goto skip;
		}

		if (pExtra->IsAdmin(this))
		{
			prefix = "`^";
		}

		if (pExtra->m_ownerID == m_userID)
		{
			prefix = "`2";
		}
	}

skip:;

	if (!role.fullPrefix.empty())
	{
		prefix = role.fullPrefix;
	}

	return prefix + name + suffix + "``";
}

std::string Player::GetStaticDisplayName()
{
	if (!m_nameOverride.empty())
	{
		return "`w" + m_nameOverride + "``";
	}

	std::string name = GetName();
	RoleInfo role = GetRoleInfo((ePlayerRole)m_role);
	std::string prefix = "`w";
	std::string suffix = role.suffix;

	if (!role.fullPrefix.empty())
	{
		prefix = role.fullPrefix;
	}

	return prefix + name + "``";
}

std::string Player::GetCountryState()
{
	std::string state = m_tank.country;
	//TODO: if player has honor title on, set state to "lg"
	if (HasBit(PLAYERFLAG_BLUE_NAME_TITLE) && HasBit(PLAYERFLAG_BLUE_NAME_TITLE_ENABLED))
	{
		state.append("|maxLevel");
	}

	//TODO: titles here
	return state;
}

std::string Player::GetTitleIcon()
{
	std::string icon = "||";

	return icon;
}

void Player::SetWorld(World* pWorld)
{
	if (pWorld && !m_pWorld)
	{
		m_pWorld = pWorld;
	}
}

uint8_t Player::GetInventoryItemCount(const uint16_t& itemID)
{
	if (!m_pItems)
	{
		return 0;
	}

	auto it = std::find_if(m_pItems->GetItems().begin(), m_pItems->GetItems().end(), [itemID](const PlayerInventoryItem& inventoryItem) { return inventoryItem.itemID == itemID; });
	if (it == m_pItems->GetItems().end())
	{
		return 0;
	}

	return it->count;
}

bool Player::AddInventoryItem(const uint16_t& itemID, const uint8_t& count, const bool& bUpdateState /* = false */)
{
	if (!m_pItems)
	{
		return 0;
	}

	bool bSuccess = GetItems()->AddInventoryItem(itemID, count);
	
	if (bSuccess && bUpdateState) 
	{
		GameUpdatePacket update;
		update.type = NET_GAME_PACKET_MODIFY_ITEM_INVENTORY;
		update.gainCount = count;
		update.intData = itemID;

		SendPacketRaw(NET_MESSAGE_GAME_PACKET, &update, sizeof(GameUpdatePacket));
	}

	return bSuccess;
}

bool Player::RemoveInventoryItem(const uint16_t& itemID, const uint8_t& count, const bool& bUpdateState /* = false */)
{
	if (!m_pItems)
	{
		return 0;
	}

	bool bSuccess = GetItems()->RemoveInventoryItem(itemID, count);

	if (bSuccess && bUpdateState)
	{
		GameUpdatePacket update;
		update.type = NET_GAME_PACKET_MODIFY_ITEM_INVENTORY;
		update.lostCount = count;
		update.intData = itemID;

		SendPacketRaw(NET_MESSAGE_GAME_PACKET, &update, sizeof(GameUpdatePacket));
	}

	return bSuccess;
}

size_t Player::GetInventoryMemoryEstimated()
{
	if (!m_pItems)
	{
		return 0;
	}

	bool bUsingOldPacket = m_tank.gameVersion < 3.02;
	size_t estimated = 0;

	estimated += sizeof(uint8_t); //inventory version
	estimated += sizeof(int); //backpack slots
	estimated += bUsingOldPacket ? sizeof(uint8_t) : sizeof(uint16_t); //size of items

	for (int i = 0; i < m_pItems->GetItems().size(); i++) 
	{
		estimated += sizeof(uint16_t); //itemID
		estimated += sizeof(uint8_t); //count
		estimated += sizeof(uint8_t); //flags
	}

	return estimated;
}

void Player::SendPacketRaw(eNetMessageType type, const void* pData, const uintmax_t& dataLength, const enet_uint32& flags /* = ENET_PACKET_FLAG_RELIABLE */)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	g_lastPlayer = m_userID;
	if (!m_pPeer)
	{
		return;
	}

	if (m_pPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		return;
	}

	ENetPacket* pPacket = enet_packet_create(nullptr, 5 + dataLength, flags);
	if (!pPacket)
	{
		return;
	}

	std::memcpy(pPacket->data, &type, 4);
	pPacket->data[dataLength + 4] = 0;

	if (pData)
	{
		std::memcpy(pPacket->data + 4, pData, dataLength);
	}

	if (enet_peer_send(m_pPeer, 0, pPacket) != 0)
	{
		enet_packet_destroy(pPacket);
	}
}

void Player::SendPacket(eNetMessageType type, const std::string& text)
{
	SendPacketRaw(type, text.data(), text.size(), ENET_PACKET_FLAG_RELIABLE);
}

void Player::SendVariantPacket(VariantList variant, const int& netID /* = -1 */, const int& delayMS /* = 0 */)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	g_lastPlayer = m_userID;
	int32_t dataLength = 0;
	uint8_t* pData = variant.SerializeToMem(&dataLength);
	if (!pData)
	{
		return;
	}

	GameUpdatePacket* pVariantPacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + dataLength);
	if (pVariantPacket) {
		pVariantPacket->type = NET_GAME_PACKET_CALL_FUNCTION;
		pVariantPacket->netID = netID;
		pVariantPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
		pVariantPacket->delay = delayMS;
		pVariantPacket->dataLength = dataLength;
		memcpy(&pVariantPacket->data, pData, dataLength);

		SendPacketRaw(eNetMessageType::NET_MESSAGE_GAME_PACKET, pVariantPacket, sizeof(GameUpdatePacket) + pVariantPacket->dataLength);
		free(pVariantPacket);
	}

	free(pData);
}

void Player::OnEmoticonDataChanged()
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	g_lastPlayer = m_userID;

	std::string growmojiDataRaw = "(wl)|ā|{}&(yes)|Ă|{}&(no)|ă|{}&(love)|Ą|{}&(oops)|ą|{}&(shy)|Ć|{}&(wink)|ć|{}&(tongue)|Ĉ|{}&(agree)|ĉ|{}&(sleep)|Ċ|{}&(punch)|ċ|{}&(music)|Č|{}&(build)|č|{}&(megaphone)|Ď|{}&(sigh)|ď|{}&(mad)|Đ|{}&(wow)|đ|{}&(dance)|Ē|{}&(see-no-evil)|ē|{}&(bheart)|Ĕ|{}&(heart)|ĕ|{}&(grow)|Ė|{}&(gems)|ė|{}&(kiss)|Ę|{}&(gtoken)|ę|{}&(lol)|Ě|{}&(smile)|Ā|{}&(cool)|Ĝ|{}&(cry)|ĝ|{}&(vend)|Ğ|{}&(bunny)|ě|{}&(cactus)|ğ|{}&(pine)|Ĥ|{}&(peace)|ģ|{}&(terror)|ġ|{}&(troll)|Ġ|{}&(evil)|Ģ|{}&(fireworks)|Ħ|{}&(football)|ĥ|{}&(alien)|ħ|{}&(party)|Ĩ|{}&(pizza)|ĩ|{}&(clap)|Ī|{}&(song)|ī|{}&(ghost)|Ĭ|{}&(nuke)|ĭ|{}&(halo)|Į|{}&(turkey)|į|{}&(gift)|İ|{}&(cake)|ı|{}&(heartarrow)|Ĳ|{}&(lucky)|ĳ|{}&(shamrock)|Ĵ|{}&(grin)|ĵ|{}&(ill)|Ķ|{}&(eyes)|ķ|{}&(weary)|ĸ|{}&(moyai)|ļ|{}&(plead)|Ľ|{}&(trophy)|Ļ|{}&(signal)|ĺ|{}&(info)|Ĺ|{}&";
	std::string growmojiData = fmt::format(fmt::runtime(growmojiDataRaw), m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_WL ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_YES ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_NO ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_LOVE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_OOPS ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_SHY ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_WINK ? 1 : 0,
			m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_TONGUE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_AGREE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_SLEEP ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_PUNCH ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_MUSIC ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_BUILD ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_MEGAPHONE ? 1 : 0,
			m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_SIGH ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_MAD ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_WOW ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_DANCE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_SEE_NO_EVIL ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_BHEART ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_HEART ? 1 : 0,
			m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_GROW ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_GEMS ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_KISS ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_GTOKEN ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_LOL ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_SMILE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_COOL ? 1 : 0,
			m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_CRY ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_VEND ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_BUNNY ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_CACTUS ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_PINE ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS2_PEACE ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_TERROR ? 1 : 0,
			m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_TROLL ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_EVIL ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_FIREWORKS ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_FOOTBALL ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_ALIEN ? 1 : 0, m_growmojiFlags & PLAYERGROWMOJI_FLAGS1_PARTY ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_PIZZA ? 1 : 0,
			m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_CLAP ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_SONGPYEON ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_GHOST ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_NUKE ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_HALO ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_TURKEY ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_GIFT ? 1 : 0,
			m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_CAKE ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_HEART_ARROW ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_LUCKY ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_SHAMROCK ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_GRIN ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_ILL ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_EYES ? 1 : 0,
			m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_WEARY ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_MOYAI ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_PLEAD ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_TROPHY ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_SIGNAL ? 1 : 0, m_growmojiFlags2 & PLAYERGROWMOJI_FLAGS2_INFO ? 1 : 0);

	SendVariantPacket({ "OnEmoticonDataChanged", 0, growmojiData });
}

void Player::SendInventoryState()
{
	if (!m_pItems)
	{
		return;
	}
	
	bool bUsingOldPacket = m_tank.gameVersion < 3.02;

	uint32_t dataLength = (uint32_t)GetInventoryMemoryEstimated();
	GameUpdatePacket* pPacket = (GameUpdatePacket*)malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SEND_INVENTORY_STATE;
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->dataLength = dataLength;

	int offset = 0;
	auto now = std::chrono::high_resolution_clock::now();
	uint8_t ver = bUsingOldPacket ? 0 : 1;
	MemorySerialize(&ver, pPacket->data, offset, true);

	int slots = GetItems()->GetBackpackSlots();
	MemorySerialize(&slots, pPacket->data, offset, true);

	if (bUsingOldPacket)
	{
		uint8_t size = (uint8_t)m_pItems->GetItems().size();
		MemorySerialize(&size, pPacket->data, offset, true);
	}
	else
	{
		uint16_t size = (uint16_t)m_pItems->GetItems().size();
		MemorySerialize(&size, pPacket->data, offset, true);
	}

	for (int i = 0; i < m_pItems->GetItems().size(); i++)
	{
		PlayerInventoryItem it = m_pItems->GetItems().at(i);

		MemorySerialize(&it.itemID, pPacket->data, offset, true);
		MemorySerialize(&it.count, pPacket->data, offset, true);

		uint8_t flags = 0;
		ItemInfo* pItem = GetItemInfoManager()->GetItem(it.itemID);
		if (pItem && pItem->type == TYPE_CLOTHES)
		{
			for (int i = 0; i < NUM_CLOTHES; i++)
			{
				if (m_pItems->GetCloth((eClothingType)i) == it.itemID)
				{
					flags = INVENTORY_ITEMFLAG_EQUIPPED;
				}
			}
		}

		it.flags = flags;
		MemorySerialize(&it.flags, pPacket->data, offset, true);
	}

	SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	free(pPacket);
}

void Player::SendTileSelect(CL_Vec2f vec /* = { -1.f, -1.f } */, std::vector<int> indexes /* = {} */)
{
	if (vec == CL_Vec2f(-1.f, -1.f) && indexes == std::vector<int>())
	{
		//m_pTileSelected1 = nullptr;
		//m_pTileSelected2 = nullptr;
	}

	size_t dataLength = 4 + (indexes.size() * 4);
	GameUpdatePacket* pPacket = (GameUpdatePacket*)malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SELECT_TILE_INDEX;
	pPacket->netID = static_cast<int>(vec.X) + (static_cast<int>(vec.Y) * m_pWorld->GetTileMap()->GetWidth());
	pPacket->itemID = -1;
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->dataLength = (uint32_t)dataLength;

	int indexSize = static_cast<int>(indexes.size());
	int offset = 4;

	MemorySerialize(&indexSize, pPacket->data, offset, true);
	memcpy(pPacket->data, &indexSize, 4);
	for (int i = 0; i < indexes.size(); i++)
	{
		MemorySerialize(&i, pPacket->data, offset, true);
	}

	SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	free(pPacket);
}

void Player::SendWrench(const int& netID, Player* pTargetPlayer)
{
	if (!pTargetPlayer)
	{
		return;
	}

	World* pWorld = pTargetPlayer->GetWorld();
	if (!pWorld)
	{
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pTargetPlayer);
	if (!pAvatar)
	{
		return;
	}

	std::string dialog = "set_default_color|`$\n";
	if (pTargetPlayer == this)
	{
		dialog += "disable_resize|\n";
		dialog += "start_custom_tabs|\n";
		dialog += "add_custom_button|main|image:interface/large/btn_tabs1.rttex;image_size:228,92;frame:1,0;width:0.13;min_width:60;|\n";
		dialog += "add_custom_button|levelup|image:interface/large/btn_tabs1.rttex;image_size:228,92;frame:0,1;width:0.13;min_width:60;|\n";
		dialog += "add_custom_button|achievements|image:interface/large/btn_tabs1.rttex;image_size:228,92;frame:0,2;width:0.13;min_width:60;|\n";
		dialog += "add_custom_button|skills|image:interface/large/btn_tabs1.rttex;image_size:228,92;frame:0,3;width:0.13;min_width:60;|\n";
		dialog += "add_custom_button|owned_worlds|image:interface/large/btn_tabs1.rttex;image_size:228,92;frame:0,4;width:0.13;min_width:60;|\n";
		dialog += "end_custom_tabs|\n";
		dialog += "add_custom_textbox|`w" + pTargetPlayer->GetName() + "``|size:large;color:255,255,255,255;left:0.5;icon:18;|\n";
		dialog += "add_spacer|small|\n";
		dialog += "add_button|goals|`$Goals & Quests``|noflags|0|0|\n";
		dialog += "add_custom_textbox|`oComplete 5 Life Goals and reach Level 5 to unlock Daily Bonuses!``|size:tiny;color:52,235,131,200;icon:3902;left:0.1;|\n";
		//dialog += "add_button|bonuses|`$Daily Bonuses(0/0)``|noflags|0|0|\n";
		dialog += "add_spacer|small|\n";
		dialog += "add_custom_textbox|`$You have `w" + std::to_string(pTargetPlayer->GetItems()->GetBackpackSlots()) + "`` backpack slots.``|size:small;color:255,255,255,255;left:0.5;icon:448;left:0.1;|\n";

		int worldX = static_cast<int>(pAvatar->GetPosition().X / 32.f) + 1 > pWorld->GetTileMap()->GetWidth() ? pWorld->GetTileMap()->GetWidth() : static_cast<int>(pAvatar->GetPosition().X / 32.f) + 1;
		int worldY = static_cast<int>(pAvatar->GetPosition().Y / 32.f) + 1 > pWorld->GetTileMap()->GetHeight() ? pWorld->GetTileMap()->GetHeight() : static_cast<int>(pAvatar->GetPosition().Y / 32.f) + 1;
		dialog += "add_custom_textbox|`$Current world: `w" + pWorld->GetName() + "`` (`w" + std::to_string(worldX) + "``, `w" + std::to_string(worldY) + "``) (`w" + std::to_string(pWorld->GetPlayersInWorld()) + "`` person)``|size:small;color:255,255,255,255;left:0.5;icon:3802;left:0.1;|\n";
		std::string status = "You are not yet a `2Supporter`` or `5Super Supporter``.";
		if (pTargetPlayer->HasBit(PLAYERFLAG_SUPPORTER))
		{
			status = "You are a `2Supporter`` and have the `wRecycler``.";
		}
		if (pTargetPlayer->HasBit(PLAYERFLAG_SUPER_SUPPORTER))
		{
			status = "You are a `5Super Supporter`` and have the `wRecycler`` and `w/warp``.";
		}

		dialog += "add_custom_textbox|`$" + status + "``|size:small;color:255,255,255,255;left:0.5;icon:12436;left:0.1;|\n";
		dialog += "add_custom_textbox|`$You are standing on the note \"" + pAvatar->GetMusicNoteStandingOn() + "\".``|size:small;color:255,255,255,255;left:0.5;icon:1712;left:0.1;|\n";
		dialog += "end_dialog|popup|||\n";
		dialog += "add_quick_exit|";
	}
	else
	{

	}

	SendVariantPacket({ "OnDialogRequest", dialog });
}

void Player::OnSetBux(const bool& bIncrement /* = false */)
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::chrono::milliseconds time(now);
	std::chrono::seconds timeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(time);

	int days = (int)timeInSeconds.count() / (60 * 60 * 24);
	int hours = (int)(timeInSeconds.count() / 3600) % 24;
	int minutes = (int)(timeInSeconds.count() / 60) % 60;
	int seconds = (int)timeInSeconds.count() % 60;

	int timeFMT = (hours * 60 * 60) + (minutes * 60) + seconds;
	int supporter = (HasBit(PLAYERFLAG_SUPPORTER) ? 3 : 0);

	CL_Vec3f vec{ static_cast<float>(timeFMT), static_cast<float>(timeFMT), static_cast<float>(timeFMT) };

	if (HasBit(PLAYERFLAG_SUPER_SUPPORTER)) 
	{
		SendVariantPacket({ "OnSetBux", m_pItems->GetGems(), (int)bIncrement, supporter, vec });
		return;
	}

	SendVariantPacket({ "OnSetBux", m_pItems->GetGems(), (int)bIncrement, supporter });
}