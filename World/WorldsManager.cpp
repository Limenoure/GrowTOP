#include <BaseApp.hpp> //precomp

#include <vector>
#include <World/WorldsManager.hpp>

#include <Utils/WorldMenuBuilder.hpp>
#include <ProtonSDK/MiscUtils.h>

#include <Player/Player.hpp>
//#include <World/World.hpp>

typedef std::pair<World*, int> RandomWorld;
WorldsManager g_worldsManager;
WorldsManager* GetWorldsManager() { return &g_worldsManager; }

void WorldsManager::AddWorld(World* pWorld) 
{
	if (!pWorld)
	{
		return;
	}

	pWorld->SetID(m_worldID++);
	m_activeWorlds.push_back(std::move(pWorld));
}

World* WorldsManager::GetWorld(const int& mapID) {
	for (int i = 0; i < m_activeWorlds.size(); i++)
	{
		World* pWorld = m_activeWorlds[i];
		if (!pWorld || pWorld->GetID() != mapID)
		{
			continue;
		}

		return pWorld;
	}

	return nullptr;
}

World* WorldsManager::GetWorld(const std::string& fName) {
	std::string nameToUse = fName;
	std::transform(nameToUse.begin(), nameToUse.end(), nameToUse.begin(), ::toupper);

	for (int i = 0; i < m_activeWorlds.size(); i++)
	{
		World* pWorld = m_activeWorlds[i];
		if (!pWorld || pWorld->GetName() != nameToUse)
		{
			continue;
		}

		return pWorld;
	}

	return nullptr;
}


World* WorldsManager::GetWorldByName(const std::string& fName) {
	std::string nameToUse = fName;
	std::transform(nameToUse.begin(), nameToUse.end(), nameToUse.begin(), ::toupper);

	for (int i = 0; i < m_activeWorlds.size(); i++)
	{
		World* pWorld = m_activeWorlds[i];
		if (!pWorld || pWorld->GetName() != nameToUse)
		{
			continue;
		}

		return pWorld;
	}

	return nullptr;
}

void WorldsManager::ValidateWorld(Player* pPlayer, const std::string& worldName)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	if (!pPlayer)
	{
		return;
	}

	std::string name = worldName;
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	uint8_t validation = 1;

	if (name == "EXIT")
	{
		validation = -1;
	}
	if (!Utils::IsOnlyAlphabet(name))
	{
		validation = -1;
	}
	if (Utils::ContainsGTSwear(name))
	{
		validation = -1;
	}
	if (name == "SETH" || name == "HAMUMU")
	{
		validation = -1;
	}

	validation = 1; //TODO: check if world exists, if it does, set as 0, if not keep as 1
	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, std::format("action|world_validated\navailable|{}\nworld_name|{}", validation, worldName));
}

void WorldsManager::SendWorldOffers(Player* pPlayer)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	if (!pPlayer)
	{
		return;
	}

	bool bOldOffers = pPlayer->GetTankInfo()->gameVersion < 3.91; //old clients will receive them formatted like old gt
	bool bJammed = pPlayer->GetRole() < ROLE_MODERATOR;
	std::vector<RandomWorld> activeWorlds;

	for (int i = 0; i < m_activeWorlds.size(); i++)
	{ 
		//lets choose random worlds
		World* pWorld = m_activeWorlds[i];
		if (!pWorld || bJammed)
		{
			continue;
		}

		RandomWorld r;
		r.first = pWorld;
		r.second = pWorld->GetPlayersInWorld();
		activeWorlds.emplace_back(r);
	}

	WorldMenuBuilder menu;
	menu.set_default(pPlayer->GetLastVisitedWorld());
	if (bOldOffers)
	{
		menu.add_button("Showing: Random Worlds", "_-catselect-_", 0.3, 3529161471);
		for (int i = 0; i < activeWorlds.size(); i++)
		{
			RandomWorld info = activeWorlds[i];
			menu.add_floater("TEST", info.second, 0.5, 3529161471);
		}
		//TODO: last visited worlds
	}
	else
	{
		menu.add_heading("`9Special Worlds``<ROW2>");
		menu.add_heading("Top Worlds<ROW2>");
		for (int i = 0; i < activeWorlds.size(); i++)
		{
			RandomWorld info = activeWorlds[i];
			menu.add_floater("TEST", info.second, 0.5, 3529161471);
		}

		menu.add_heading("My Worlds<CR>");
		//TODO: owned worlds
		menu.add_heading("Recently Visited Worlds<CR>");
		//TODO: last visited worlds
	}

	pPlayer->SendVariantPacket({ "OnRequestWorldSelectMenu", menu.Get() });
}

void WorldsManager::EnterWorld(Player* pPlayer, World* pWorld, CL_Vec2f spawnXY /* = CL_Vec2f{ -1.f, -1.f } */)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	if (!pPlayer || !pWorld)
	{
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (pAvatar)
	{
		pWorld->GetNetObjectManager()->RemoveObject(pAvatar->GetNetID());
		return;
	}

	if (spawnXY == CL_Vec2f{ -1.f, -1.f })
	{
		spawnXY = pWorld->GetTileMap()->GetSpawnPoint();
	}

	int offset = 0;
	uint32_t dataLength = (uint32_t)pWorld->GetMemoryEstimated(true, (float)pPlayer->GetTankInfo()->gameVersion, pWorld->GetVersion());

	GameUpdatePacket* pPacket = (GameUpdatePacket*)malloc(sizeof(GameUpdatePacket) + dataLength);
	if (!pPacket)
	{
		return;
	}

	pPacket->type = NET_GAME_PACKET_SEND_MAP_DATA;
	pPacket->netID = -1;
	pPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pPacket->dataLength = dataLength;
	
	pWorld->SerializeToMem(pPacket->data, offset, true, (float)pPlayer->GetTankInfo()->gameVersion, pWorld->GetVersion());
	pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	free(pPacket);

	pAvatar = pWorld->GetNetObjectManager()->MakeObject(pPlayer);
	if (!pAvatar) 
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|Something went wrong trying to enter this world, contact developers for more information.");
		ExitWorld(pPlayer, pWorld);
		return;
	}

	pWorld->GetNetObjectManager()->SetCount(pWorld->GetNetObjectManager()->GetCount() + 1);
	pPlayer->ToggleTempBit(PLAYERTEMPFLAG_INWORLD, true);
	pAvatar->SetWorld(pWorld);
	pAvatar->SetPosition(pWorld->GetTileMap()->GetSpawnPoint());
	pAvatar->SetRespawnPoint(pWorld->GetTileMap()->GetSpawnPoint());

	pPlayer->SendVariantPacket({ "OnSpawn", pAvatar->GetSpawnData(true) }, -1, -1);
	pPlayer->SendVariantPacket({ "OnFlagMay2019", 256 }, pAvatar->GetNetID());
	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");

	pPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pPlayer->GetDisplayName(pWorld) + "``" }, pAvatar->GetNetID());
	pPlayer->SendVariantPacket({ "OnCountryState", pPlayer->GetTankInfo()->country }, pAvatar->GetNetID());
	pAvatar->SendCharacterState(pPlayer);
	pAvatar->SendClothes(pPlayer, true);
	pPlayer->SendVariantPacket({ "OnMagicCompassTrackingItemIDChanged", pPlayer->GetMagnetItem() });
	pPlayer->OnEmoticonDataChanged();

	//`0[```2NOPUNCH``, `2IMMUNE``, `4JAMMED``, `2NOWAR``, `2ANTIGRAVITY```0]`` ``
	std::string extra = "";
	if (pWorld->GetCategory() != WORLDCATEGORY_NONE)
	{
		extra = " `w[`9" + pWorld->GetCategoryName() + "``]``";
	}

	pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|World `w" + pWorld->GetName() + "" + extra + " ``(size: `$" + pWorld->GetSize(dataLength) + "``) entered. There are `w" + std::to_string(pWorld->GetPlayersInWorld() - 1) + "`` other people here, `w" + std::to_string(GetOnlineManager()->GetPlayersOnline()) + "`` online.");
	
	if (pWorld->GetLockIndex() == 0)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`oLock this world with world lock.``");
	}
	else
	{
		std::string msg = "";
		Tile* pLock = pWorld->GetTileMap()->GetTile(pWorld->GetLockIndex());
		if (pLock)
		{
			if (pLock->HasFlag(TILEFLAG_PUBLIC))
			{
				msg = " (`$PUBLIC``)";
			}

			TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pLock->GetTileExtra());
			if (pExtra && (pExtra->m_ownerID == pPlayer->GetUserID() || pExtra->IsAdmin(pPlayer)))
			{
				msg = " (`2ACCESS GRANTED``)";
			}
		}

		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`5[```w" + pWorld->GetName() + "`` `$World Locked`` by DeletedUser" + msg + "`5]``");
	}

	if (pWorld->GetName() == "START")
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|Use `$WASD`` or the `$arrow keys`` to move.  Click on things with the `$mouse`` to `$punch``/`$edit``.");
	}

	if (false)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|The name of this world has recently been changed!");
	}

	for (int i = 0; i < pWorld->GetNetObjectManager()->GetObjects().size(); i++) 
	{
		NetAvatar* pTargetAvatar = pWorld->GetNetObjectManager()->GetObjects().at(i);
		if (!pTargetAvatar || pTargetAvatar->GetNetID() == pAvatar->GetNetID())
		{
			continue;
		}

		Player* pTargetPlayer = pTargetAvatar->GetPlayer();

		pPlayer->SendVariantPacket({ "OnSpawn", pTargetAvatar->GetSpawnData() }, -1, -1);
		pPlayer->SendVariantPacket({ "OnFlagMay2019", 256 }, pTargetAvatar->GetNetID());
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");

		pPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pTargetPlayer->GetDisplayName(pWorld) + "``", pTargetPlayer->GetTitleIcon() }, pTargetAvatar->GetNetID());
		pPlayer->SendVariantPacket({ "OnCountryState", pTargetPlayer->GetTankInfo()->country }, pTargetAvatar->GetNetID());
		pTargetAvatar->SendCharacterState(pPlayer);
		pTargetAvatar->SendClothes(pPlayer);

		if (true)
		{
			pPlayer->SendVariantPacket({ "OnTalkBubble", pTargetAvatar->GetNetID(), pTargetAvatar->GetPlayer()->GetStaticDisplayName() });
		}

		//other players

		pTargetPlayer->SendVariantPacket({ "OnSpawn", pAvatar->GetSpawnData() }, -1, -1);
		pTargetPlayer->SendVariantPacket({ "OnFlagMay2019", 256 }, pAvatar->GetNetID());
		pTargetPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");

		pTargetPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pPlayer->GetDisplayName(pWorld) + "``", pPlayer->GetTitleIcon() }, pAvatar->GetNetID());
		pTargetPlayer->SendVariantPacket({ "OnCountryState", pPlayer->GetTankInfo()->country }, pAvatar->GetNetID());
		pAvatar->SendCharacterState(pTargetPlayer);
		pAvatar->SendClothes(pTargetPlayer);

		if (true)
		{
			pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`5<`w" + pPlayer->GetDisplayName(pWorld) + "`` entered, `w" + std::to_string(pWorld->GetPlayersInWorld() - 1) + "`` others here>``" });
			pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5<`w" + pPlayer->GetDisplayName(pWorld) + "`` entered, `w" + std::to_string(pWorld->GetPlayersInWorld() - 1) + "`` others here>``" });
			pTargetPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");
		}
	}
}

void WorldsManager::ExitWorld(Player* pPlayer, World* pWorld, const bool& bSendOffers /* = true */)
{
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif

	if (!pPlayer || !pWorld)
	{
		return;
	}

	NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
	if (!pAvatar)
	{
		return;
	}

	pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
	{
		if (pTargetPlayer->GetNetID() != pPlayer->GetNetID())
		{
			pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`5<`w" + pPlayer->GetDisplayName(pWorld) + "`` left, `w" + std::to_string(pWorld->GetPlayersInWorld() - 1) + "`` others here>``" });
			pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5<`w" + pPlayer->GetDisplayName(pWorld) + "`` left, `w" + std::to_string(pWorld->GetPlayersInWorld() - 1) + "`` others here>``" });
			pTargetPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_close.wav\ndelayMS|0");
		}

		pTargetPlayer->SendVariantPacket({ "OnRemove", std::format("netID|{}", pAvatar->GetNetID()), std::format("pId|{}", pPlayer->GetUserID()) });
	});

	pWorld->GetNetObjectManager()->SetCount(pWorld->GetNetObjectManager()->GetCount() - 1);
	pWorld->GetNetObjectManager()->RemoveObject(pAvatar->GetNetID());
	pPlayer->SetWorld(nullptr);
	pPlayer->SetNetID(-1);
	pPlayer->ToggleTempBit(PLAYERTEMPFLAG_INWORLD, false);

	if (bSendOffers)
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, std::format("action|log\nmsg|Where would you like to go? (`w{}`` others online).", GetOnlineManager()->GetPlayersOnline()));
		SendWorldOffers(pPlayer);
	}

	//TODO: if worrld has no players left, cache & unload
}