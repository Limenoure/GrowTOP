#pragma once
#include <string>

#include <Network/CommandsHandler.hpp>
#include <Items/ItemInfoManager.hpp>

void CheatCommand(const CommandHeader& head)
{
	Player* pPlayer = head.pPlayer;
	World* pWorld = head.pWorld;

	if (!pWorld || !pPlayer)
	{
		return;
	}

	if (head.args.empty())
	{
		pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
		return;
	}

	int cheatCode = std::atoi(head.args[0].c_str());
	switch (cheatCode)
	{
	    case 2:
	    {
			pPlayer->GetItems()->SetBackpackSlots(197);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 2; i < 396; i++) 
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Cheat activated, gave you all items past 1.");
	    } break;

		case 5:
		{
			pPlayer->GetItems()->SetBackpackSlots(198);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 1150; i < 2100; i++)
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Cheat activated, gave you all items past 1150.");
		} break;

		case 10:
		{
			pPlayer->GetItems()->SetBackpackSlots(198);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 2700; i < 3800; i++)
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Cheat activated, gave you all items past 2700.");
		} break;

		case 12:
		{
			pPlayer->GetItems()->SetBackpackSlots(198);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 3300; i < 4250; i++)
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Cheat activated, gave you all items past 3300.");
		} break;

		case 20:
		{
			pPlayer->GetItems()->SetBackpackSlots(198);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 5522; i < 6000; i++)
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Cheat activated, gave you all items past 5522.");
		} break;

		case 40:
		{
			pPlayer->GetItems()->SetBackpackSlots(198);
			pPlayer->GetItems()->ClearItems();

			pPlayer->AddInventoryItem(ITEM_ID_FIST, 1, false);
			pPlayer->AddInventoryItem(ITEM_ID_WRENCH, 1, false);

			for (uint16_t i = 9018; i < 9800; i++)
			{
				ItemInfo* pItem = GetItemInfoManager()->GetItem(i);
				if (!pItem)
				{
					continue;
				}

				if (pItem->type == TYPE_SEED || pItem->ID == ITEM_ID_MAIN_DOOR)
				{
					continue;
				}

				uint8_t count = pItem->type == TYPE_CLOTHES ? Randomizer::Get(0, 3) : Randomizer::Get(20, 70);
				pPlayer->AddInventoryItem(i, count, false);
			}

			pPlayer->SendInventoryState();
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|>> Gave you all items past 9018 (October 2019 Update - Part II)");
		} break;

		default:
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
			return;
		}
	}
}