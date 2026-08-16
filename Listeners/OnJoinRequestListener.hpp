#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnJoinRequestListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnJoinRequestListener";
		if (!pPlayer || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME) || pPlayer->HasTempBit(PLAYERTEMPFLAG_INWORLD))
		{
			return;
		}

		auto SendFail = [&](const bool& bResetCamera, const std::string& msg) 
		{
			if (!msg.empty())
			{
				pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + msg);
			}

			pPlayer->SendVariantPacket({ "OnFailedToEnterWorld" });
			if (bResetCamera) 
			{
				pPlayer->SendVariantPacket({ "OnZoomCamera", 10000, 1000 });
				pPlayer->SendVariantPacket({ "OnSetFreezeState", 0 });
			}
		};

		TextScanner t;
		t.SetupFromMemoryAddress(textData.c_str());

		std::string worldName = t.GetParmString("name", 1);
		std::transform(worldName.begin(), worldName.end(), worldName.begin(), ::toupper);

		Utils::RemoveExtraSpaces(worldName);
		Utils::TrimSpaceBothSidesOfString(worldName);

		if (worldName.empty())
		{
			worldName = "START";
		}

		if (worldName == "EXIT")
		{
			SendFail(true, "Exit from what? Press back if you're done playing.");
			return;
		}

		if (!Utils::IsOnlyAlphabet(worldName)) 
		{
			SendFail(true, "Sorry, spaces and special characters are not allowed in world or door names.  Try again.");
			return;
		}

		if (Utils::ContainsGTSwear(worldName))
		{
			if (pPlayer->GetRole() < ROLE_KING)
			{
				worldName = "DISNEYLAND";
			}
			else
			{
				pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Note: ``Allowing illegal world name because you're a studly smod");
			}
		}

		if (worldName.length() > 24)
		{
			SendFail(true, "`4World name too long, try again.");
			return;
		}

		//Oops, you've created too many worlds today.  Try again tomorrow!
		//Only the developers can create a world with that name.
		//Oops, `5%s`` already has `4%s`` people in it. Try again later.
		//You're not legendary enough for that mountain.
		//You are not ready to battle... Come back with a Battle Leash..
		//That world is beyond you at this time.
		//Growganoth won't let you enter!
		//OnJoiNWorld - current no world active
		//OnJoiNWorld - World %s active
		//`4To reduce confusion, that is not a valid world name.``  Try another?
		//`4Door ID too long, try again.
		//`4World name too long, try again.
		//`2You already kissed this Blarney Stone today!``

		World* pWorld = GetWorldsManager()->GetWorldByName(worldName);
		if (!pWorld)
		{
			pWorld = new World(worldName, 100, 60);
			pWorld->GetTileMap()->GenerateTerrain(TERRATYPE_SUNNY, 100, 60);
			GetWorldsManager()->AddWorld(pWorld);
			GetWorldsManager()->EnterWorld(pPlayer, pWorld);
			return;
		}

		bool bLocked = pWorld->GetLockIndex() != 0 && pWorld->GetOwnerID() != -1;	
		
		if (pWorld->HasBit(WORLDBIT_NOGO) && pPlayer->GetRole() < ROLE_MODERATOR) {
			SendFail(true, "`4To reduce confusion, that is not a valid world name.`` Try another?");
			return;
		}

		if (pWorld->HasBit(WORLDBIT_INACCESSIBLE) && pPlayer->GetRole() < ROLE_MODERATOR) {
			SendFail(true, "That world is inaccessible.");
			return;
		}
		
		if (pWorld->GetPlayersInWorld() >= GetBaseApp()->GetConfig().maxWorldPlayers && pPlayer->GetRole() < ROLE_MODERATOR)
		{
			SendFail(true, "Oops, `5" + pWorld->GetName() + "`` already has `4" + std::to_string(pWorld->GetPlayersInWorld()) + "`` people in it. Try again later.");
			return;
		}

		if (pWorld->GetName() == "TOURNAMENT" && pPlayer->GetItems()->GetCloth(HAND) != ITEM_ID_BATTLE_LEASH)
		{
			SendFail(true, "You are not ready to battle... Come back with a Battle Leash..");
			return;
		}

		if (pWorld->GetName() == "TOURNAMENT" && pPlayer->GetLevel() < 40 && true)
		{
			SendFail(true, "You're not legendary enough for that mountain.");
			return;
		}

		if (bLocked)
		{
			Tile* pParent = pWorld->GetTileMap()->GetTile(pWorld->GetLockIndex());
			if (pParent)
			{
				TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pParent->GetTileExtra());
				if (pExtra)
				{
					if (pExtra->m_entryLevel > pPlayer->GetLevel() && pPlayer->GetRole() < ROLE_MODERATOR)
					{
						SendFail(true, "Players lower than level <world level> can't enter " + pWorld->GetName() + ".");
						return;
					}
				}
			}
		}

		GetWorldsManager()->EnterWorld(pPlayer, pWorld);
	}

} // namespace Listeners