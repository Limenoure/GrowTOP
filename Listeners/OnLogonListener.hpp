#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	bool ValidateLogon(Player* pPlayer, TextScanner& t)
	{
		g_lastCall = "Listeners::ValidateLogon";
		if (!pPlayer)
		{
			return false;
		}

		//if something is missing, tell us in our discord server so we will handle it
		std::string tankIDName = t.GetParmString("tankIDName", 1);
		std::string tankIDPass = t.GetParmString("tankIDPass", 1);
		std::string requestedName = t.GetParmString("requestedName", 1);
		uint32_t f = t.GetParmInt("f", 1);
		uint32_t protocol = t.GetParmInt("protocol", 1);
		std::string game_version = t.GetParmString("game_version", 1);
		uint32_t fz = t.GetParmInt("fz", 1);
		uint32_t lmode = t.GetParmInt("lmode", 1);
		uint32_t cbits = t.GetParmInt("cbits", 1);
		uint32_t player_age = t.GetParmInt("player_age", 1);
		std::string category = t.GetParmString("category", 1);
		uint32_t totalPlaytime = t.GetParmInt("totalPlaytime", 1);
		std::string klv = t.GetParmString("klv", 1);
		uint32_t hash2 = t.GetParmInt("hash2", 1);
		std::string meta = t.GetParmString("meta", 1);
		uint32_t fhash = t.GetParmInt("fhash", 1);
		std::string rid = t.GetParmString("rid", 1);
		std::string platformID = t.GetParmString("platformID", 1);
		uint32_t deviceVersion = t.GetParmInt("deviceVersion", 1);
		std::string country = t.GetParmString("country", 1);
		uint32_t hash = t.GetParmInt("hash", 1);
		std::string mac = t.GetParmString("mac", 1);
		std::string wk = t.GetParmString("wk", 1);
		uint32_t zf = t.GetParmInt("zf", 1);

		//validation
		if (platformID.length() < 1 || !std::isdigit(platformID[0]))
		{
			return false; //platform id isnt integer
		}

		pPlayer->GetTankInfo()->platformID = platformID[0] - '0';

		if (mac.length() != 17)
		{
			return false; //mac doesn't match the length or 
		}

		if (rid.length() != 32)
		{
			return false; //rid doesnt match the length
		}

		if (meta.empty())
		{
			return false; //meta is empty
		}

		if (game_version.length() < 3 || game_version.length() > 5)
		{
			return false; //game version doesn't match length
		}

		pPlayer->GetTankInfo()->gameVersion = std::stod(game_version);
		if (country.length() != 2)
		{
			country = "us";
		}

		//generating klv with given logon info to check if it's spoofed or not
		std::string securedRid = ""; //TODO: Utils::GenerateRid
		/*if (rid != securedRid)
		{
			return false; //rid is spoofed
		}*/

		return true;
	}

	void OnLogonListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnLogonListener";
		if (!pPlayer || pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME))
		{
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textData.c_str());

		if (!ValidateLogon(pPlayer, t))
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Oops``: Something went wrong with validating your request, try again later. If the issue still persist, message developers for more info.");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|logon_fail");
			return;
		}

		std::string tankIDName = t.GetParmString("tankIDName", 1);
		std::string tankIDPass = t.GetParmString("tankIDPass", 1);
		std::string requestedName = t.GetParmString("requestedName", 1);
		bool bGuestLogon = tankIDName.empty();

		if (pPlayer->GetTankInfo()->platformID == 0/*PLATFORM_ID_WINDOWS*/ && t.GetParmString("mac", 1) == "02:00:00:00:00:00")
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unable to log on: ``Unfortunately your computer has a Mac address of 02:00:00:00:00:00 which is invalid. Are you using a virtualized network card? Unfortunately that doesn't work with Growtopia.");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|logon_fail");
			return;
		}

		if (bGuestLogon && requestedName.length() < 3)
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|You'll need a name `$3 chars`` or longer to play online with. (select cancel and enter a longer name)");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|logon_fail");
			return;
		}

		pPlayer->GetTankInfo()->tankIDName = tankIDName;
		pPlayer->GetTankInfo()->tankIDPass = tankIDPass;
		pPlayer->GetTankInfo()->requestedName = requestedName;

		if (!bGuestLogon) 
		{
			pPlayer->SetAccountID(-1);
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unable to log on: ``That `wGrowID`` doesn't seem valid, or the password is wrong. If you don't have one, press `wCancel``, un-check `w'I have a GrowID'``, then click `wConnect``.");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|set_url\nurl|https://glowtopia.com/account.php\nlabel|`$Retrieve lost password``");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|logon_fail");
			return;
		}

		pPlayer->SetAccountID(100 + Randomizer::Get(0, 899));
		pPlayer->SetUserID(GetOnlineManager()->GetUserID(true));

		pPlayer->SendVariantPacket({ "OnConsoleMessage", "One moment, updating items data..." });
		std::string prefix = pPlayer->GetTankInfo()->gameVersion >= 3.91f ? "www." : "";
		pPlayer->SendVariantPacket({ Utils::GetLogonVariantString((float)pPlayer->GetTankInfo()->gameVersion),
			GetItemInfoManager()->GetHash(),
			prefix + "growtopia2.com",
			"cache/",
			"cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
			"proto=207|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|clash_active=0|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|"
		});
	}

	void OnRefreshItemsListener(Player* pPlayer)
	{
		if (!pPlayer || pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME))
		{
			return;
		}

		GameUpdatePacket* pPacket = GetItemInfoManager()->GetPacket();
		if (!pPacket)
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unable to log on: ``Something went wrong trying to update items, contact support for more information.");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|set_url\nurl|https://glowtopia.com/support.php\nlabel|`$Contact Support``");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|logon_fail");
			return;
		}

		if (GetBaseApp()->GetConfig().bBeta)
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|using beta items.");
		}
		pPlayer->ToggleTempBit(PLAYERTEMPFLAG_UPDATING_ITEMS, true);
		pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	}

	void EnterGameListener(Player* pPlayer)
	{
		if (!pPlayer)
		{
			return;
		}

		pPlayer->ToggleTempBit(PLAYERTEMPFLAG_UPDATING_ITEMS, false);
		pPlayer->ToggleTempBit(PLAYERTEMPFLAG_INGAME, true);

		if (GetBaseApp()->GetConfig().bBeta)
		{
			pPlayer->AddInventoryItem(ITEM_ID_DOOR, 200);
			pPlayer->AddInventoryItem(ITEM_ID_DIRT, 200);
			pPlayer->AddInventoryItem(ITEM_ID_WORLD_LOCK, 100);
			pPlayer->AddInventoryItem(ITEM_ID_SMALL_LOCK, 5);
			pPlayer->AddInventoryItem(ITEM_ID_BIG_LOCK, 5);
			pPlayer->AddInventoryItem(ITEM_ID_HUGE_LOCK, 5);
			pPlayer->AddInventoryItem(ITEM_ID_BUILDER_S_LOCK, 5);
			pPlayer->AddInventoryItem(ITEM_ID_ROYAL_LOCK, 1);
			pPlayer->AddInventoryItem(ITEM_ID_CAVE_BLAST, 10);
			pPlayer->AddInventoryItem(ITEM_ID_MAGIC_MAGNET, 1);
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|Welcome to `$" + GetBaseApp()->GetConfig().growtopia + "``, you've been rewarded items for our beta test.");
		}

		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|Welcome back, `w" + pPlayer->GetName() + "``.");
		pPlayer->GetItems()->SetGems(10000000);
		pPlayer->OnSetBux();
		pPlayer->OnEmoticonDataChanged();
		pPlayer->SendInventoryState();
		GetWorldsManager()->SendWorldOffers(pPlayer);

		if (pPlayer->GetAccountID() != -1)
		{
			pPlayer->SendVariantPacket({ "SetHasGrowID", 0, "", "" });
		}

		if (GetBaseApp()->GetConfig().bBeta)
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + GetBaseApp()->GetConfig().betaMessage + "");
		}

		pPlayer->SendVariantPacket({ "OnDialogRequest", GetBaseApp()->GetGazette() });
	}
	
} // namespace Listeners