#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

namespace Listeners
{
	void OnItemActivateListener(Player* pPlayer, GameUpdatePacket* pPacket)
	{
		g_lastCall = "Listeners::OnItemActivateListener";
		if (!pPlayer || !pPacket || pPlayer->GetInventoryItemCount(pPacket->intData) < 1 || !pPlayer->GetWorld())
		{
			return;
		}

		World* pWorld = pPlayer->GetWorld();
		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		ItemInfo* pItem = GetItemInfoManager()->GetItem(pPacket->intData);
		if (!pItem)
		{
			return;
		}

		if (pItem->bodyPart < 0 || pItem->bodyPart > NUM_CLOTHES)
		{
			return;
		}

		if (pItem->type != TYPE_ANCES && pItem->type != TYPE_CLOTHES)
		{
			switch (pItem->ID)
			{
			    case ITEM_ID_WORLD_LOCK: 
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 100)
					{
						return;
					}

					if (!pPlayer->AddInventoryItem(ITEM_ID_DIAMOND_LOCK, 1, true) || !pPlayer->RemoveInventoryItem(pItem->ID, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You compressed 100 `2" + pItem->name + "`` into a `2Diamond Lock``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You compressed 100 `2" + pItem->name + "`` into a `2Diamond Lock``!" });
			    } return;
				case ITEM_ID_DIAMOND_LOCK:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || !pPlayer->AddInventoryItem(ITEM_ID_WORLD_LOCK, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You shattered a `2" + pItem->name + "`` into 100 `2World Lock``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You shattered a `2" + pItem->name + "`` into 100 `2World Lock``!" });
				} return;
				case ITEM_ID_BLUE_GEM_LOCK:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || !pPlayer->AddInventoryItem(ITEM_ID_DIAMOND_LOCK, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You shattered a `2" + pItem->name + "`` into 100 `2Diamond Lock``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You shattered a `2" + pItem->name + "`` into 100 `2Diamond Lock``!" });
				} return;
				case ITEM_ID_CADUCEUS:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 100)
					{
						return;
					}

					if (!pPlayer->AddInventoryItem(ITEM_ID_GOLDEN_CADUCEUS, 1, true) || !pPlayer->RemoveInventoryItem(pItem->ID, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You compressed 100 `2" + pItem->name + "`` into a `2Golden Caduceus``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You compressed 100 `2" + pItem->name + "`` into a `2Golden Caduceus``!" });
				} return;
				case ITEM_ID_GOLDEN_CADUCEUS:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || !pPlayer->AddInventoryItem(ITEM_ID_CADUCEUS, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You shattered a `2" + pItem->name + "`` into 100 `2Caduceus``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You shattered a `2" + pItem->name + "`` into 100 `2Caduceus``!" });
				} return;
				case ITEM_ID_BUCKAZOID:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 100)
					{
						return;
					}

					if (!pPlayer->AddInventoryItem(ITEM_ID_MEGA_BUCKAZOID, 1, true) || !pPlayer->RemoveInventoryItem(pItem->ID, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You compressed 100 `2" + pItem->name + "`` into a `2Mega Buckazoid``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You compressed 100 `2" + pItem->name + "`` into a `2Mega Buckazoid``!" });
				} return;
				case ITEM_ID_MEGA_BUCKAZOID:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || !pPlayer->AddInventoryItem(ITEM_ID_BUCKAZOID, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You shattered a `2" + pItem->name + "`` into 100 `2Buckazoid``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You shattered a `2" + pItem->name + "`` into 100 `2Buckazoid``!" });
				} return;
				case ITEM_ID_GROWTOKEN:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 100)
					{
						return;
					}

					if (!pPlayer->AddInventoryItem(ITEM_ID_MEGA_GROWTOKEN, 1, true) || !pPlayer->RemoveInventoryItem(pItem->ID, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You compressed 100 `2" + pItem->name + "`` into a `2Mega Growtoken``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You compressed 100 `2" + pItem->name + "`` into a `2Mega Growtoken``!" });
				} return;
				case ITEM_ID_MEGA_GROWTOKEN:
				{
					if (pPlayer->GetInventoryItemCount(pItem->ID) < 1)
					{
						return;
					}

					if (!pPlayer->RemoveInventoryItem(pItem->ID, 1, true) || !pPlayer->AddInventoryItem(ITEM_ID_GROWTOKEN, 100, true))
					{
						return;
					}

					pPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), "You shattered a `2" + pItem->name + "`` into 100 `2Growtoken``!", 0, 1 });
					pPlayer->SendVariantPacket({ "OnConsoleMessage", "You shattered a `2" + pItem->name + "`` into 100 `2Growtoken``!" });
				} return;
			}
		}

		int bodyPart = pItem->bodyPart;
		if (pItem->type == TYPE_ANCES)
		{
			bodyPart = ARTIFACT;
		}

		if (pItem->ID != ITEM_ID_EXQUISITE_GAUNTLET_OF_ELEMENTS && pPlayer->GetItems()->GetCloth(HAND) == ITEM_ID_EXQUISITE_GAUNTLET_OF_ELEMENTS)
		{
			pPlayer->SendTileSelect();
		}

		if (pPlayer->GetItems()->GetCloth((eClothingType)bodyPart) == pItem->ID && (pItem->ID == ITEM_ID_DIAMOND_HORN || pItem->ID == ITEM_ID_DIAMOND_HORNS || pItem->ID == ITEM_ID_DIAMOND_DEVIL_HORNS)) 
		{
			if (pItem->ID == ITEM_ID_DIAMOND_HORN) 
			{
				if (pPlayer->AddInventoryItem(ITEM_ID_DIAMOND_HORNS, 1, true)) 
				{
					pPlayer->RemoveInventoryItem(ITEM_ID_DIAMOND_HORN, 1, true);
					pPlayer->GetItems()->SetCloth((eClothingType)bodyPart, 0);
				}
			}

			if (pItem->ID == ITEM_ID_DIAMOND_HORNS) 
			{
				if (pPlayer->AddInventoryItem(ITEM_ID_DIAMOND_DEVIL_HORNS, 1, true)) 
				{
					pPlayer->RemoveInventoryItem(ITEM_ID_DIAMOND_HORNS, 1, true);
					pPlayer->GetItems()->SetCloth((eClothingType)bodyPart, 0);
				}
			}

			if (pItem->ID == ITEM_ID_DIAMOND_DEVIL_HORNS) 
			{
				if (pPlayer->AddInventoryItem(ITEM_ID_DIAMOND_HORN, 1, true)) 
				{
					pPlayer->RemoveInventoryItem(ITEM_ID_DIAMOND_DEVIL_HORNS, 1, true);
					pPlayer->GetItems()->SetCloth((eClothingType)bodyPart, 0);
				}
			}

			pAvatar->Update();
			return;
		}

		pPlayer->GetItems()->SetCloth((eClothingType)bodyPart, pPlayer->GetItems()->GetCloth((eClothingType)bodyPart) == pItem->ID ? 0 : pItem->ID, true);
		if (pItem->ID == ITEM_ID_NEWSPAPER && pPlayer->GetItems()->GetCloth(HAND) == ITEM_ID_NEWSPAPER)
		{
			pPlayer->SendVariantPacket({ "OnDialogRequest", GetBaseApp()->GetGazette() });
		}

		pAvatar->Update();
		if (pItem->ID == ITEM_ID_MAGIC_MAGNET && pPlayer->GetItems()->GetCloth((eClothingType)bodyPart) == ITEM_ID_MAGIC_MAGNET) 
		{
			DialogBuilder d;
			std::string dialog = "set_default_color|`o\n";
			dialog += std::format("add_label_with_icon|big|`wMagic Magnet``|left|{}\n", pItem->ID);
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
			return;
		}
	}
	
} // namespace Listeners