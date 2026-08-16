#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnDialogReturnListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnDialogReturnListener";
		if (!pPlayer)
		{
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textData.c_str());

		std::string dialogName = t.GetParmString("dialog_name", 1);
		std::string buttonClicked = t.GetParmString("buttonClicked", 1);
		switch (HashStringFNV(dialogName))
		{
		    case "door_edit"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile || pTile->GetItem()->type != TYPE_DOOR)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "Huh? The door is gone!", 0, 1 });
					return;
				}

				if (!pWorld->CanWrench(pTile, pPlayer))
				{
					return;
				}

				TileExtraDoor* pExtra = dynamic_cast<TileExtraDoor*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				std::string label = t.GetParmString("door_name", 1); //100
				std::string target = t.GetParmString("door_target", 1); //24
				std::string ID = t.GetParmString("door_id", 1); //23
				bool bOpenToPublic = (bool)t.GetParmInt("checkbox_locked", 1);

				if (label.length() > 100 || target.length() > 24 || ID.length() > 23)
				{
					if (ID.length() > 23)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "That door ID is too long!", 0, 1 });
					}

					return;
				}

				if (ID.length() < 4 && pTile->GetForeground() != ITEM_ID_PASSWORD_DOOR)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "`4Warning:`` That doorID is easy to guess.  People can use a doorID to warp directly in to this point from another world!", 0, 1 });
				}

				if (pTile->GetForeground() == ITEM_ID_PASSWORD_DOOR)
				{
					pExtra->m_password = ID;
				}
				else
				{
					pExtra->m_id = ID;
				}

				pExtra->m_label = label;
				pExtra->m_destination = target;
				pExtra->m_flags = bOpenToPublic ? 0 : 8;
				pWorld->SendTileUpdate(pTile);
			} break;

			case "sign_edit"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile || (pTile->GetItem()->type != TYPE_SIGN && pTile->GetItem()->ID != ITEM_ID_PATH_MARKER))
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Huh? The sign is gone!", 0, 1 });
					return;
				}

				if (!pWorld->CanWrench(pTile, pPlayer))
				{
					return;
				}

				TileExtraSign* pExtra = dynamic_cast<TileExtraSign*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				std::string label = t.GetParmString("sign_text", 1); //128
				if (label.length() > 128)
				{
					return;
				}

				if (pTile->GetForeground() == ITEM_ID_PATH_MARKER)
				{
					pExtra->m_id = label;
				}
				else
				{
					pExtra->m_label = label;
				}

				pWorld->SendTileUpdate(pTile);
			} break;

			case "donation_box_edit"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile || pTile->GetItem()->type != TYPE_DONATION_BOX)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "The box is gone!  Maybe it was put into another box?", 0, 1 });
					return;
				}

				TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				bool bIsOwner = pTile->IsOwner(pPlayer);
				int gifts = pExtra->GetGifts(pPlayer->GetUserID());
				int size = (int)pExtra->m_donations.size();

				int itemID = t.GetParmInt("itemid", 1);
				if (itemID != ITEM_ID_BLANK)
				{
					if (size >= 20)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You aren't able to fit another gift inside, it's jammed full.", 0, 1 });
						return;
					}

					if (!bIsOwner && gifts >= 3)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`4Woah nelly, you don't want to be TOO nice.  Try again later.``", 0, 1 });
						return;
					}

					ItemInfo* pItem = GetItemInfoManager()->GetItem(itemID);
					if (!pItem)
					{
						return;
					}

					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You don't have that to give!", 0, 1 });
						return;
					}

					if (pItem->editableTypes & UNTRADEABLE || pItem->editableTypes & MOD || pItem->ID == ITEM_ID_WORLD_KEY || pItem->ID == ITEM_ID_GUILD_KEY)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`7[```4You can't place that in the box, you need it!`7]``", 0, 1 });
						return;
					}

					if (pItem->ID == ITEM_ID_WOLF_WHISTLE && false)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't give that away while wolfed out!", 0, 1 });
						return;
					}

					if ((pItem->ID == ITEM_ID_SPRING_CLASH_FINALE_TICKET || pItem->ID == ITEM_ID_SUMMER_CLASH_FINALE_TICKET || pItem->ID == ITEM_ID_WINTER_CLASH_FINALE_TICKET) && false)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't give that away while running the Clash Finale!", 0, 1 });
						return;
					}

					if (pItem->type == TYPE_PETFISH)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't put a live fish in the box!  That would just be mean.", 0, 1 });
						return;
					}

					if (pItem->rarity < 2)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`7[```4This box only accepts items rarity 1+ or greater`7]``", 0, 1 });
						return;
					}

					std::string dialog = "set_default_color|`o\n";
					dialog += "add_label_with_icon|big|" + pItem->name + "|left|" + std::to_string(pItem->ID) + "|\n";
					dialog += "add_textbox|How many to put in the box as a gift? (Note: You will `4LOSE`` the items you give!)|left|\n";
					dialog += "add_text_input|count|Count:|" + std::to_string(pPlayer->GetInventoryItemCount(pItem->ID)) + "|5|\n";
					dialog += "add_text_input|personal_note|Optional Note:||54|\n";

					dialog += "embed_data|itemID|" + std::to_string(pItem->ID) + "\n";
					dialog += "embed_data|tilex|" + std::to_string(tileX) + "\n";
					dialog += "embed_data|tiley|" + std::to_string(tileY) + "\n";
					dialog += "add_button|give|`4Give the item(s)``|noflags|0|0|\n";
					dialog += "add_spacer|small|\n";
					dialog += "add_button|cancel|`wCancel``|noflags|0|0|\n";
					dialog += "end_dialog|give_item|||\n";

					pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
					return;
				}

				if (!bIsOwner && pTile->IsLocked())
				{
					return;
				}

				if (buttonClicked == "clear")
				{
					for (const auto& [ID, info] : pExtra->m_donations)
					{
						ItemInfo* pItem = GetItemInfoManager()->GetItem(info.itemID);
						if (!pItem)
						{
							continue;
						}

						if (!pPlayer->AddInventoryItem(info.itemID, info.count, true))
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Hmm, I'll need to drop the " + pItem->name + " items I already have before I can get the ones in the box, they won't fit.", 0, 1 });
							continue;
						}

						pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
						{
							pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5[```w" + pPlayer->GetName() + "`` receives `5" + std::to_string(info.count) + "`` `2" + pItem->name + "`` from `w" + info.lastName + "```5]``" });
						});

						auto it = pExtra->m_donations.find(ID);
						if (it != pExtra->m_donations.end())
						{
							pExtra->m_donations.erase(it);
						}
					}

					if (pExtra->m_donations.empty())
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`2Box emptied.``", 0, 1 });
					}

					bool bStuffed = pExtra->m_donations.size() > 0;
					pTile->ToggleFlag(TILEFLAG_ENABLED, bStuffed);
					pWorld->SendTileUpdate(pTile);
					return;
				}
				
				if (buttonClicked == "select")
				{
					for (const auto& [ID, info] : pExtra->m_donations)
					{
						ItemInfo* pItem = GetItemInfoManager()->GetItem(info.itemID);
						if (!pItem)
						{
							continue;
						}

						bool bSelected = (bool)t.GetParmInt(std::to_string(ID), 1);
						if (!bSelected)
						{
							continue;
						}

						if (!pPlayer->AddInventoryItem(info.itemID, info.count, true))
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Hmm, I'll need to drop the " + pItem->name + " items I already have before I can get the ones in the box, they won't fit.", 0, 1 });
							continue;
						}

						auto it = pExtra->m_donations.find(ID);
						if (it != pExtra->m_donations.end())
						{
							pExtra->m_donations.erase(it);
						}

						pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
						{
								pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`5[```w" + pPlayer->GetName() + "`` receives `5" + std::to_string(info.count) + "`` `2" + pItem->name + "`` from `w" + info.lastName + "```5]``" });
						});
					}

					if (pExtra->m_donations.empty())
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`2Box emptied.``", 0, 1 });
					}

					bool bStuffed = pExtra->m_donations.size() > 0;
					pTile->ToggleFlag(TILEFLAG_ENABLED, bStuffed);
					pWorld->SendTileUpdate(pTile);
					return;
				}
			} break;

			case "give_item"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				int itemID = t.GetParmInt("itemID", 1);
				int count = t.GetParmInt("count", 1);
				std::string personal_note = t.GetParmString("personal_note", 1);

				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile || pTile->GetItem()->type != TYPE_DONATION_BOX)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "The box is gone!  Maybe it was put into another box?", 0, 1 });
					return;
				}

				TileExtraDonationBox* pExtra = dynamic_cast<TileExtraDonationBox*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				bool bIsOwner = pTile->IsOwner(pPlayer);
				int gifts = pExtra->GetGifts(pPlayer->GetUserID());
				int size = (int)pExtra->m_donations.size();

				if (size >= 20)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You aren't able to fit another gift inside, it's jammed full.", 0, 1 });
					return;
				}

				if (!bIsOwner && gifts >= 3)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`4Woah nelly, you don't want to be TOO nice.  Try again later.``", 0, 1 });
					return;
				}

				ItemInfo* pItem = GetItemInfoManager()->GetItem(itemID);
				if (!pItem)
				{
					return;
				}

				if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You don't have that to give!", 0, 1 });
					return;
				}

				if (pItem->editableTypes & UNTRADEABLE || pItem->editableTypes & MOD || pItem->ID == ITEM_ID_WORLD_KEY || pItem->ID == ITEM_ID_GUILD_KEY)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`7[```4You can't place that in the box, you need it!`7]``", 0, 1 });
					return;
				}

				if (pItem->ID == ITEM_ID_WOLF_WHISTLE && false)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't give that away while wolfed out!", 0, 1 });
					return;
				}

				if ((pItem->ID == ITEM_ID_SPRING_CLASH_FINALE_TICKET || pItem->ID == ITEM_ID_SUMMER_CLASH_FINALE_TICKET || pItem->ID == ITEM_ID_WINTER_CLASH_FINALE_TICKET) && false)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't give that away while running the Clash Finale!", 0, 1 });
					return;
				}

				if (pItem->type == TYPE_PETFISH)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You can't put a live fish in the box!  That would just be mean.", 0, 1 });
					return;
				}

				if (pItem->rarity < 2)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`7[```4This box only accepts items rarity 1+ or greater`7]``", 0, 1 });
					return;
				}

				if (!pPlayer->RemoveInventoryItem(pItem->ID, count, true))
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "You don't have that to give!", 0, 1 });
					return;
				}

				if (pPlayer->GetItems()->GetGems() - count < 0)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pPlayer->GetNetID(), "You don't have enough gems to give!", 0, 1 });
					return;
				}

				if (buttonClicked == "give")
				{
					pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
					{
						pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "[`5[```w" + pPlayer->GetName() + " places `5" + std::to_string(count) + "`` `2" + pItem->name + "`` into the " + pTile->GetItem()->name + "`5]````]" });
						pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`5[```w" + pPlayer->GetName() + " places `5" + std::to_string(count) + "`` `2" + pItem->name + "`` into the " + pTile->GetItem()->name + "`5]``", 0, 1 });
					});

					DonationInfo info;
					info.userID = pPlayer->GetUserID();
					info.lastName = pPlayer->GetName();
					info.itemID = pItem->ID;
					info.count = count;
					info.message = personal_note;

					pExtra->m_donations.insert_or_assign(pExtra->m_donationID++, info);
					bool bStuffed = pExtra->m_donations.size() > 0;

					pTile->ToggleFlag(TILEFLAG_ENABLED, bStuffed);
					pWorld->SendTileUpdate(pTile);
				}
			} break;

			case "lock_edit"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile || pTile->GetItem()->type != TYPE_LOCK)
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I was looking at a lock but now it's gone.  Magic is real!", 0, 1 });
					return;
				}

				TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				if (pExtra->m_ownerID != pPlayer->GetUserID())
				{
					return;
				}

				int netID = -1; //playerNetID
				if (textData.find("playerNetID") != std::string::npos)
				{
					netID = t.GetParmInt("playerNetID", 1);
					if (netID == -1)
					{
						goto rest;
					}

					for (int i = 0; i < pWorld->GetNetObjectManager()->GetObjects().size(); i++)
					{
						NetAvatar* pTargetAvatar = pWorld->GetNetObjectManager()->GetObjects().at(i);
						if (!pTargetAvatar || pTargetAvatar->GetNetID() != netID)
						{
							continue;
						}

						bool bIsAdmin = pExtra->IsAdmin(pTargetAvatar->GetPlayer());
						bool bIsOwner = pExtra->m_ownerID == pTargetAvatar->GetPlayer()->GetUserID() || pPlayer->GetRole() >= ROLE_ADMINISTRATOR;
						if (bIsOwner)
						{
							std::string msg = "I already have access!";
							if (pPlayer->GetRole() >= ROLE_ADMINISTRATOR) 
							{
								msg.append(" (mod version)");
							}

							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), msg, 0, 1 });
							break;
						}

						if (bIsAdmin)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "This player already have access!", 0, 1 });
							break;
						}

						if (pExtra->m_admins.size() >= 25)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`w" + pTargetAvatar->GetPlayer()->GetName() + "`` can't be added, the lock is full!", 0, 1 });
							break;
						}

						pExtra->m_admins.emplace_back(pTargetAvatar->GetPlayer()->GetUserID());
						pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
						{
							pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`w" + pTargetAvatar->GetPlayer()->GetName() + "`` has been given access to a " + pTile->GetItem()->name + "." });
							pTargetPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pTargetAvatar->GetPlayer()->GetName() + "``", pTargetAvatar->GetPlayer()->GetTitleIcon() }, pTargetAvatar->GetNetID());
						});

						break;
					}
				}
			rest:;

				bool bPublic = (bool)t.GetParmInt("checkbox_public", 1);
				if (pTile->GetItem()->lockPower == 0)
				{
					bool bSilencedPleasants = (bool)t.GetParmInt("checkbox_silence", 1);
					bool bRoyalRainbows = (bool)t.GetParmInt("checkbox_rainbow", 1);
					bool bDisableMusic = (bool)t.GetParmInt("checkbox_disable_music", 1);
					bool bInvisibleNotes = (bool)t.GetParmInt("checkbox_disable_music_render", 1);
					bool bIsHome = (bool)t.GetParmInt("checkbox_home_world", 1);

					int tempo = t.GetParmInt("tempo", 1);
					int entryLevel = t.GetParmInt("entry_level", 1);

					std::string msg = "`w" + pPlayer->GetName() + "`` has set the `$World Lock`` to `4PRIVATE``";
					if (bPublic && !pTile->HasFlag(TILEFLAG_PUBLIC))
					{
						msg = "`w" + pPlayer->GetName() + "`` has set the `$World Lock`` to `$PUBLIC``";
					}

					if (bPublic != pTile->HasFlag(TILEFLAG_PUBLIC))
					{
						pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
						{
							pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", msg });
						});
					}

					if (pTile->GetItem()->lockPower == 0 && pTile->GetForeground() == ITEM_ID_ROYAL_LOCK)
					{
						std::string msg = "`9" + pPlayer->GetName() + "```w has silenced the peasants!``";
						if (!bSilencedPleasants && pExtra->HasFlag(LOCKFLAG_SILENCED_PLEASANTS))
						{
							msg = "`9" + pPlayer->GetName() + "```w has allowed the peasants to speak.``";
						}

						if (bSilencedPleasants != pExtra->HasFlag(LOCKFLAG_SILENCED_PLEASANTS))
						{
							pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
							{
								pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", msg });
								pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), msg, 0, 1 });
							});
						}
					}

					if (tempo < 20 || tempo > 200 && tempo != pExtra->m_tempo)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Tempo must be from 20-200 BPM.", 0, 1 });
					}

					if (entryLevel < 1)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Minimum entry level can't be lower than 1.", 0, 1 });
						return;
					}

					if (entryLevel > 124)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Minimum entry level can't be higher than 124.", 0, 1 });
						return;
					}

					if (entryLevel != pExtra->m_entryLevel)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Minimum entry level for this world is set to `2Level " + std::to_string(entryLevel) + "``.", 0, 1 });
					}

					bool bIsThisHome = pPlayer->GetHome() == pWorld->GetName();
					msg = pWorld->GetName() + " has been set as your home world!";
					if (!bIsHome && pPlayer->GetHome() == pWorld->GetName())
					{
						msg = pWorld->GetName() + " has been removed as your home world!";
					}

					pPlayer->SetHome(bIsHome ? pWorld->GetName() : "EXIT");
					if (bIsThisHome != bIsHome)
					{
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), msg, 0, 1 });
					}

					if (buttonClicked == "catchange")
					{
						std::string dialog = "set_default_color|`o\n"
							"add_label_with_icon|big|`wSet World Category``|left|3802|\n"
							"add_button|0|None|noflags|0|0|\n"
							"add_button|1|Adventure|noflags|0|0|\n"
							"add_button|2|Art|noflags|0|0|\n"
							"add_button|3|Farm|noflags|0|0|\n"
							"add_button|4|Game|noflags|0|0|\n"
							"add_button|5|Guild|noflags|0|0|\n"
							"add_button|6|Information|noflags|0|0|\n"
							"add_button|7|Music|noflags|0|0|\n"
							"add_button|8|Parkour|noflags|0|0|\n"
							"add_button|9|Puzzle|noflags|0|0|\n"
							"add_button|10|Roleplay|noflags|0|0|\n"
							"add_button|11|Shop|noflags|0|0|\n"
							"add_button|12|Social|noflags|0|0|\n"
							"add_button|13|Storage|noflags|0|0|\n"
							"add_button|14|Story|noflags|0|0|\n"
							"add_button|15|Trade|noflags|0|0|\n"
							"add_smalltext|Worlds in the category \"None\" can't be rated by players, and they are only listed on the normal World Select screen.|left|\n"
							"add_smalltext|`4Warning:`o Changing your category will delete all ratings on your world.|left|\n"
							"add_quick_exit|\n"
							"end_dialog|category_change|Nevermind|";

						pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
						return;
					}

					if (buttonClicked == "settimer")
					{
						std::string dialog = "set_default_color|`o\n";
						dialog += "add_label_with_icon|big|`wSet World Timer``|left|1482|\n";
						dialog += "add_textbox|Select a time limit for how long guests can stay in your world.|left\n";
						dialog += "set_max_checks|1\n";
						dialog += std::format("add_checkbox|0|None|{}\n", pWorld->GetTimer() == 0 ? "1" : "0");
						dialog += std::format("add_checkbox|5|5 Minutes|{}\n", pWorld->GetTimer() == 5 ? "1" : "0");
						dialog += std::format("add_checkbox|10|10 Minutes|{}\n", pWorld->GetTimer() == 10 ? "1" : "0");
						dialog += std::format("add_checkbox|20|20 Minutes|{}\n", pWorld->GetTimer() == 20 ? "1" : "0");
						dialog += std::format("add_checkbox|30|30 Minutes|{}\n", pWorld->GetTimer() == 30 ? "1" : "0");
						dialog += std::format("add_checkbox|40|40 Minutes|{}\n", pWorld->GetTimer() == 40 ? "1" : "0");
						dialog += std::format("add_checkbox|50|50 Minutes|{}\n", pWorld->GetTimer() == 50 ? "1" : "0");
						dialog += std::format("add_checkbox|60|60 Minutes|{}\n", pWorld->GetTimer() == 60 ? "1" : "0");
						dialog += "end_dialog|timer_change|Nevermind|Set Time Limit|";

						pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
						return;
					}

					if (buttonClicked == "getkey")
					{
						if (pPlayer->GetInventoryItemCount(ITEM_ID_WORLD_KEY) != 0)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You already have a World Key!", 0, 1});
							return;
						}

						int locks = 0;
						bool bLocked = false;
						bool bDroppedItems = false;
						bool bUntradeable = false;
						for (int i = 0; i < pWorld->GetTileMap()->GetTiles().size(); i++)
						{
							Tile* pTile = &pWorld->GetTileMap()->GetTiles().at(i);
							if (!pTile || !pTile->GetItem() || pTile->GetItem()->ID == ITEM_ID_BLANK)
							{
								continue;
							}

							if (pTile->GetItem()->type == TYPE_LOCK)
							{
								++locks;
								if (locks >= 2)
								{
									bLocked = true;
								}
								break;
							}

							if (pTile->GetItem()->editableTypes & UNTRADEABLE && pTile->GetItem()->type != TYPE_MAIN_DOOR && pTile->GetItem()->type != TYPE_BEDROCK)
							{
								bUntradeable = true;
								break;
							}
						}

						if (!pWorld->GetObjectMap()->GetObjects().empty())
						{
							for (int i = 0; i < pWorld->GetObjectMap()->GetObjects().size(); i++)
							{
								WorldObject* pObject = pWorld->GetObjectMap()->GetObjects().at(i);
								if (!pObject)
								{
									continue;
								}

								int tilePosX = (int)(pObject->x / 32.f);
								int tilePosY = (int)(pObject->y / 32.f);

								Tile* pTile = pWorld->GetTileMap()->GetTile(tilePosX, tilePosY);
								if (!pTile || !pTile->GetItem())
								{
									continue;
								}

								if (pTile->GetForeground() == ITEM_ID_BLANK || (pTile->GetForeground() != ITEM_ID_BLANK && pWorld->IsTileObstacle(pPlayer, pTile)))
								{
									bDroppedItems = true;
									break;
								}
							}
						}

						if (bUntradeable)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`4Oops!... Can't trade a world with Untradeable blocks in it!``", 0, 1 });
							return;
						}

						if (bLocked)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "To trade a world, you can only have one lock on it, the `5World Lock``.  Remove the rest!", 0, 1 });
							return;
						}

						if (bDroppedItems)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "`4Oops!... Can't trade a world with floating items that are unblocked or in treasure chests!``", 0, 1 });
							return;
						}

						Tile* pParent = pWorld->GetTile(pWorld->GetLockIndex());
						if (!pParent)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I was looking at a lock but now it's gone.  Magic is real!", 0, 1 });
							return;
						}

						TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pParent->GetTileExtra());
						if (!pExtra)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I was looking at a lock but now it's gone.  Magic is real!", 0, 1 });
							return;
						}

						if (!pExtra->m_admins.empty())
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You'll first need to `4remove all co-owners`` from your `5World Lock`` to get a `#World Key`` to trade this world.", 0, 1 });
							return;
						}

						
						if (!pPlayer->AddInventoryItem(ITEM_ID_WORLD_KEY, 1, true))
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You don't have enough space to get a World Key.", 0, 1 });
							return;
						}

						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You got a `#World Key``! You can now trade this world to other players.", 0, 1 });
						return;
					}

					pTile->ToggleFlag(TILEFLAG_PUBLIC, bPublic);
					pExtra->ToggleFlag(LOCKFLAG_SILENCED_PLEASANTS, bSilencedPleasants);
					pExtra->ToggleFlag(LOCKFLAG_ROYAL_RAINBOW, bRoyalRainbows);
					pExtra->ToggleFlag(LOCKFLAG_DISABLE_MUSIC_NOTE, bDisableMusic);
					pExtra->ToggleFlag(LOCKFLAG_INVISIBLE_MUSIC_NOTE, bInvisibleNotes);
					pExtra->m_tempo = tempo;
					pExtra->m_entryLevel = entryLevel;
				}
				else
				{
					bool bOnlyBuilding = (bool)t.GetParmInt("checkbox_buildonly", 1);
					bool bAdminsRestricted = (bool)t.GetParmInt("checkbox_admins", 1);
					bool bIgnoreAir = (bool)t.GetParmInt("checkbox_ignore", 1);

					pTile->ToggleFlag(TILEFLAG_PUBLIC, bPublic);
					pExtra->ToggleFlag(LOCKFLAG_ONLY_BUILDING, bOnlyBuilding);
					pExtra->ToggleFlag(LOCKFLAG_RESTRICT_ADMIN, bAdminsRestricted);
					pExtra->ToggleFlag(LOCKFLAG_IGNORE_EMPTY_AIR, bIgnoreAir);

					if (buttonClicked == "recalcLock")
					{
						if (pTile->GetItem()->lockPower == 0)
						{
							return;
						}

						pWorld->GetTileMap()->RemoveAllTilesFromThisLock(pTile);
						pWorld->GetTileMap()->AddTilesThisWouldLock(pTile, pTile->GetItem()->lockPower, bIgnoreAir);
						pWorld->SendLock(pPlayer, pTile);
						pWorld->SendTileUpdate(pTile);
						return;
					}
				}

				pWorld->SendTileUpdate(pTile);
			} break;

			case "remove_access"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int tileX = t.GetParmInt("tilex", 1);
				int tileY = t.GetParmInt("tiley", 1);
				Tile* pTile = pWorld->GetTileMap()->GetTile(tileX, tileY);
				if (!pTile)
				{
					return;
				}

				if (pTile->GetItem()->type != TYPE_LOCK)
				{
					return;
				}

				TileExtraLock* pExtra = dynamic_cast<TileExtraLock*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				if (!pExtra->IsAdmin(pPlayer))
				{
					return;
				}

				auto it = std::find(pExtra->m_admins.begin(), pExtra->m_admins.end(), pPlayer->GetUserID());
				pExtra->m_admins.erase(it);
				pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
				{
					pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`w" + pPlayer->GetName() + "`` removed their access from a " + pTile->GetItem()->name + "." });
					pTargetPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pPlayer->GetName() + "``", pPlayer->GetTitleIcon() }, pAvatar->GetNetID());
				});

				pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "I've removed my access!", 0, 1 });
			} break;

			case "category_change"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				int category = t.GetParmInt("buttonClicked", 1);
				if (category < WORLDCATEGORY_NONE || category > WORLDCATEGORY_TRADE)
				{
					return;
				}

				pWorld->SetCategory((eWorldCategory)category);
				pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
				{
					std::string msg = "This world's category has been set to 'None'.";
					if (category != WORLDCATEGORY_NONE)
					{
						msg = "This world has been moved to the '" + pWorld->GetCategoryName()  + "' category! Everyone, please type `2/rate`` to rate it from 1-5 stars.";
					}
					pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", msg });
				});
			} break;

			case "timer_change"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int minutes = 0;
				if (t.GetParmInt("0", 1) == 1)
				{
					minutes = 0;
				}

				if (t.GetParmInt("5", 1) == 1)
				{
					minutes = 5;
				}

				if (t.GetParmInt("10", 1) == 1)
				{
					minutes = 10;
				}

				if (t.GetParmInt("20", 1) == 1)
				{
					minutes = 20;
				}

				if (t.GetParmInt("30", 1) == 1)
				{
					minutes = 30;
				}

				if (t.GetParmInt("40", 1) == 1)
				{
					minutes = 40;
				}

				if (t.GetParmInt("50", 1) == 1)
				{
					minutes = 50;
				}

				if (t.GetParmInt("60", 1) == 1)
				{
					minutes = 60;
				}

				pWorld->SetTimer(minutes);
				pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
				{
					std::string msg = "World Timer limit removed!";
					if (minutes != 0)
					{
						msg = "World Timer limit set to `2 " + std::to_string(minutes) + " minutes``.";
					}

					pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), msg, 0, 1 });
				});
			} break;

			case "magnet_edit"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				if (pPlayer->GetInventoryItemCount(ITEM_ID_MAGIC_MAGNET) < 1)
				{
					return;
				}

				if (buttonClicked == "clear")
				{
					pPlayer->SetMagnetItem(ITEM_ID_BLANK);

					DialogBuilder d;
					std::string dialog = "set_default_color|`o\n";
					dialog += "add_label_with_icon|big|`wMagic Magnet``|left|8304\n";
					if (pPlayer->GetMagnetItem() != ITEM_ID_BLANK)
					{
						ItemInfo* pItem = GetItemInfoManager()->GetItem(pPlayer->GetMagnetItem());
						if (!pItem)
						{
							return;
						}

						dialog += std::format("add_label_with_icon|small|`w{}``|left|{}\n", pItem->name, pItem->ID);
					}

					dialog += "add_item_picker|magnetron|`wSelect Item``|Choose an item to pickup!|\n";
					dialog += "add_button|clear|`$Clear``|noflags|0|0\n";
					dialog += "end_dialog|magnet_edit|Close||";

					pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
					pPlayer->SendVariantPacket({ "OnMagicCompassTrackingItemIDChanged", pPlayer->GetMagnetItem() });
					return;
				}

				int magnetItem = t.GetParmInt("magnetron", 1);
				if (magnetItem == 0 || magnetItem == pPlayer->GetMagnetItem())
				{
					return;
				}

				if (pPlayer->GetInventoryItemCount(magnetItem) < 1)
				{
					return;
				}

				pPlayer->SetMagnetItem(magnetItem);
				pPlayer->SendVariantPacket({ "OnMagicCompassTrackingItemIDChanged", pPlayer->GetMagnetItem() });
			} break;

			case "terraformer_reply"_FNV:
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int item = t.GetParmInt("itemID", 1);
				ItemInfo* pItem = GetItemInfoManager()->GetItem(item);
				if (!pItem)
				{
					return;
				}

				if (pPlayer->GetInventoryItemCount(item) < 1)
				{
					return;
				}

				if (pItem->ID == ITEM_ID_BEACH_BLAST && pPlayer->GetInventoryItemCount(ITEM_ID_FIREWORKS) < 100) 
				{
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You need 100 Fireworks to launch!", 0, 1 });
					return;
				}

				std::string world = t.GetParmString("world_name", 1);
				if (world.length() > 24 || world.empty()) 
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "The world name is too short!" });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "The world name is too short!", 0, 1 });
					return;
				}

				std::string upperName = world;
				std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
				if (upperName.find_first_not_of("1234567890QWERTYUIOPASDFGHJKLZXCVBNM") != std::string::npos) 
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "Sorry, a world name can't contain special characters or spaces." });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Sorry, a world name can't contain special characters or spaces.", 0, 1 });
					return;
				}

				if (Utils::ContainsGTSwear(upperName)) 
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "Sorry, a world name can't be inappropriate." });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Sorry, a world name can't be inappropriate.", 0, 1 });
					return;
				}

				/*if (GetDatabase()->IsWorldCreated(upperName))
				{
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "That world name already exists. You'll have to be more original. Maybe add some numbers after it?" });
					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "That world name already exists. You'll have to be more original. Maybe add some numbers after it?", 0, 1 });
					return;
				}*/

				if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || (pItem->ID == ITEM_ID_BEACH_BLAST && !pPlayer->RemoveInventoryItem(ITEM_ID_FIREWORKS, 100, true)))
				{
					return;
				}

				World* pTerraformedWorld = new World(upperName, 100, 60);
				switch (pItem->ID) 
				{
				case ITEM_ID_CAVE_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_CAVE, 100, 60); } break;
				case ITEM_ID_BEACH_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_BEACH, 100, 60); } break;
				case ITEM_ID_MONOCHROME_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_MONOCHROME, 100, 60); } break;
				case ITEM_ID_BOUNTIFUL_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_BOUNTIFUL, 100, 60); } break;
				case ITEM_ID_JUNGLE_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_JUNGLE, 100, 60); } break;
				case ITEM_ID_DESERT_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_DESERT, 100, 60); } break;
				case ITEM_ID_MARS_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_MARS, 100, 60); } break;
				case ITEM_ID_UNDERSEA_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_UNDERSEA, 100, 60); } break;
				case ITEM_ID_TREASURE_BLAST: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_TREASURE, 100, 60); } break;
				default: { pTerraformedWorld->GetTileMap()->GenerateTerrain(TERRATYPE_SUNNY, 100, 60); } break;
				}

				GetWorldsManager()->AddWorld(std::move(pTerraformedWorld));
				pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
				{
					pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), std::format("** `5{} activates a {}!`` **", pPlayer->GetDisplayName(pWorld), pItem->name), 0, 1 });
					pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", std::format("** `5{} activates a {}!`` **", pPlayer->GetDisplayName(pWorld), pItem->name) });
				});

				GetWorldsManager()->ExitWorld(pPlayer, pWorld, false);
				GetWorldsManager()->EnterWorld(pPlayer, pTerraformedWorld, pTerraformedWorld->GetTileMap()->GetSpawnPoint());
			} break;

			case "itemsucker_block"_FNV: 
			{
				World* pWorld = pPlayer->GetWorld();
				if (!pWorld)
				{
					return;
				}

				NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
				if (!pAvatar)
				{
					return;
				}

				int x = t.GetParmInt("tilex", 1);
				int y = t.GetParmInt("tiley", 1);

				Tile* pTile = pWorld->GetTileMap()->GetTile(x, y);
				if (!pTile)
				{
					return;
				}

				if (pTile->GetItem()->type != TYPE_ITEM_SUCKER)
				{
					return;
				}

				TileExtraItemSucker* pExtra = dynamic_cast<TileExtraItemSucker*>(pTile->GetTileExtra());
				if (!pExtra)
				{
					return;
				}

				ItemInfo* pDisplayItem = GetItemInfoManager()->GetItem(pExtra->m_itemID);
				if (!pDisplayItem)
				{
					return;
				}

				uint8_t modType = (uint8_t)t.GetParmInt("changetype", 1);
				if (modType != 0)
				{
					if (pDisplayItem->ID == ITEM_ID_BLANK)
					{
						return;
					}

					int count = t.GetParmInt("count", 1);
					if (modType == 1) //adding items
					{
						if (count < 1 || count > pDisplayItem->maxCount)
						{
							return;
						}

						if (pPlayer->GetInventoryItemCount(pDisplayItem->ID) < 1)
						{
							return;
						}

						if (pExtra->m_count + count > pExtra->m_capacity)
						{
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "The machine is overfilled.", 0, 1 });
							return;
						}

						if (!pPlayer->RemoveInventoryItem(pDisplayItem->ID, count, true))
						{
							return;
						}

						pPlayer->SendVariantPacket({ "OnConsoleMessage", "Items added." });
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Items added.", 0, 1 });
						pExtra->m_count = pExtra->m_count + count;
						pWorld->SendTileUpdate(pTile);
					}

					if (modType == 2) //removing items
					{
						if (count < 1 || count > pDisplayItem->maxCount || count > pExtra->m_count)
						{
							return;
						}

						int countIHave = pPlayer->GetInventoryItemCount(pDisplayItem->ID);
						if (count > pExtra->m_capacity || countIHave + count > pDisplayItem->maxCount)
						{
							return;
						}

						if (!pPlayer->AddInventoryItem(pDisplayItem->ID, count, true)) 
						{
							pPlayer->SendVariantPacket({ "OnConsoleMessage", "You don't have enough space in your backpack to fit those items." });
							pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You don't have enough space in your backpack to fit those items.", 0, 1 });
							return;
						}

						pPlayer->SendVariantPacket({ "OnConsoleMessage", "Items removed." });
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "Items removed.", 0, 1 });
						pExtra->m_count = pExtra->m_count - count < 0 ? 0 : pExtra->m_count - count;
						pWorld->SendTileUpdate(pTile);
					}

					return;
				}

				int select = t.GetParmInt("selectitem", 1);
				ItemInfo* pSelectItem = GetItemInfoManager()->GetItem(select);
				if (!pSelectItem)
				{
					return;
				}

				auto IsCompatible = [&](uint16_t tileID, ItemInfo* pItem) -> bool 
				{
					switch (tileID) 
					{
					    case ITEM_ID_UNSTABLE_TESSERACT: 
						{
							if (pItem->rarity == 0 || pItem->rarity == 999 || pItem->type != TYPE_NORMAL || pItem->editableTypes & UNTRADEABLE)
							{
								return false;
							}

						} break;
						case ITEM_ID_GAIA_S_BEACON:
						{
							if (pItem->rarity == 0 || pItem->rarity == 999 || pItem->type != TYPE_SEED || pItem->editableTypes & UNTRADEABLE)
							{
								return false;
							}

						} break;
						default:
						{
							if (pItem->rarity == 0 || pItem->rarity == 999 || pItem->type == TYPE_LOCK || pItem->editableTypes & UNTRADEABLE || pItem->editableTypes & AUTOPICKUP || pItem->type == TYPE_ITEM_PLANTER)
							{
								return false;
							}

						} break;
					}

					return true;
				};

				if (pDisplayItem->ID == ITEM_ID_BLANK) 
				{
					if (!IsCompatible(pTile->GetForeground(), pSelectItem)) 
					{
						pPlayer->SendVariantPacket({ "OnConsoleMessage", "This item is not compatible." });
						pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "This item is not compatible.", 0, 1 });
						return;
					}

					pExtra->m_count = 0;
					pExtra->m_itemID = select;

					pWorld->Broadcast([&](int netID, Player* pTargetPlayer) 
					{
						pTargetPlayer->SendVariantPacket({ "OnPlanterActivated", select }, netID);
					});

					pWorld->SendTileUpdate(pTile);
					return;
				}

				if (buttonClicked == "clearitem") 
				{
					if (pDisplayItem->ID == ITEM_ID_BLANK || pExtra->m_count > 1)
					{
						return;
					}

					pExtra->m_itemID = ITEM_ID_BLANK;
					pExtra->m_bBuildingMode = false;
					pExtra->m_bMagnet = false;
					pExtra->m_count = 0;
					pWorld->SendTileUpdate(pTile);
					return;
				}

				if (buttonClicked == "additem") 
				{
					if (pDisplayItem->ID == ITEM_ID_BLANK || pExtra->m_count >= pExtra->m_capacity)
					{
						return;
					}

					int countIHave = pPlayer->GetInventoryItemCount(pExtra->m_itemID);
					if (countIHave < 1)
					{
						return;
					}

					std::string dialog = "set_default_color|`o\n";
					dialog += "add_label_with_icon|small|`2" + pDisplayItem->name + "``|left|" + std::to_string(pExtra->m_itemID) + "|\n";
					dialog += "embed_data|tilex|" + std::to_string(x) + "\n";
					dialog += "embed_data|tiley|" + std::to_string(y) + "\n";
					dialog += "embed_data|itemid|" + std::to_string(pExtra->m_itemID) + "\n";
					dialog += "embed_data|changetype|1\n";
					dialog += "add_smalltext|You have " + std::to_string(countIHave) + " `2" + pDisplayItem->name + "`` in your backpack.|left|\n";
					dialog += "add_textbox|`wHow many `2" + pDisplayItem->name + "`` would you like to add?``|\n";
					dialog += "add_text_input|count|Amount: |" + std::to_string(countIHave) + "|5|\n";
					dialog += "end_dialog|itemsucker_block|Close|Add|";

					pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
					return;
				}

				if (buttonClicked == "retrieveitem")
				{
					if (pDisplayItem->ID == ITEM_ID_BLANK || pExtra->m_count >= pExtra->m_capacity)
					{
						return;
					}

					int countIHave = pPlayer->GetInventoryItemCount(pExtra->m_itemID);

					std::string dialog = "set_default_color|`o\n";
					dialog += "add_label_with_icon|small|`2" + pDisplayItem->name + "``|left|" + std::to_string(pExtra->m_itemID) + "|\n";
					dialog += "embed_data|tilex|" + std::to_string(x) + "\n";
					dialog += "embed_data|tiley|" + std::to_string(y) + "\n";
					dialog += "embed_data|itemid|" + std::to_string(pExtra->m_itemID) + "\n";
					dialog += "embed_data|changetype|2\n";
					dialog += "add_textbox|`wHow many `2" + pDisplayItem->name + "`` would you like to add?``|\n";

					int count = pExtra->m_count;
					if (count > pDisplayItem->maxCount)
					{
						count = pDisplayItem->maxCount;
					}

					if (countIHave + count > pDisplayItem->maxCount)
					{
						count = pDisplayItem->maxCount - countIHave;
					}

					dialog += "add_text_input|count|Amount: |" + std::to_string(count) + "|5|\n";
					dialog += "end_dialog|itemsucker_block|Close|Remove|";

					pPlayer->SendVariantPacket({ "OnDialogRequest", dialog });
					return;
				}

				pExtra->m_bMagnet = (bool)t.GetParmInt("enablesucking", 1);
				pWorld->SendTileUpdate(pTile);
			} break;
		}
	}
	
} // namespace Listeners