#pragma once
#include <string>

#include <ProtonSDK/MiscUtils.h>
#include <ProtonSDK/TextScanner.h>

#include <Network/CommandsHandler.hpp>

namespace Listeners
{
	void OnInputListener(Player* pPlayer, const std::string& textData)
	{
		g_lastCall = "Listeners::OnInputListener";
		if (!pPlayer || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INGAME) || !pPlayer->HasTempBit(PLAYERTEMPFLAG_INWORLD))
		{
			return;
		}

		World* pWorld = GetWorldsManager()->GetWorldByName(pPlayer->GetWorld()->GetName());
		if (!pWorld)
		{
			return;
		}

		NetAvatar* pAvatar = pWorld->GetNetObjectManager()->GetNetObject(pPlayer);
		if (!pAvatar)
		{
			return;
		}

		std::string input = textData.substr(textData.find("|text|") + 6);

		Utils::RemoveBadCharacters(input);
		Utils::TrimSpaceBothSidesOfString(input);
		Utils::RemoveExtraSpaces(input);

		if (input.empty() || input == " ")
		{
			return;
		}

		if (input.starts_with("/")) 
		{
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`6" + input + "``");
			
			CommandHeader head;
			head.commandUsed = input;
			head.pPlayer = pPlayer;
			head.pWorld = pWorld;

			if (!GetCommandsHandler()->ProccessSlashCommand(head))
			{
				pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|`4Unknown command.``  Enter `$/?`` for a list of valid commands.");
			}

			return;
		}

		bool bEmote = false;
		if (input == ":)" || input == ">:(" || input == ":D" || input == ";)" || input == ":(" || input == ":o" || input == ":O" || input == ":~(" || input == ":'(" || input == ":,(" || input == ":|" || input == ":/" || input == ":," || input == ";-)" || input == ":*")
		{
			bEmote = true;
		}

		std::string c = "w";
		std::string cs = "$";

		std::string bubble = bEmote ? "CP:_PL:0_OID:_" + input : "CP:_PL:0_OID:_player_chat=`" + c + input;
		if (!bEmote)
		{
			bubble.append("``");
		}

		RoleInfo role = GetRoleInfo((ePlayerRole)pPlayer->GetRole());
		c = role.chatColor;
		cs = role.chatColor == 'w' ? '$' : role.chatColor;

		pWorld->Broadcast([&](int netID, Player* pTargetPlayer) 
		{
			std::string start = "";
			if (pTargetPlayer->GetTankInfo()->gameVersion >= 2.998)
			{
				start = "CP:_OID:_CT:[W]_ ";
			}

			pTargetPlayer->SendVariantPacket({ "OnTalkBubble", pAvatar->GetNetID(), bubble });
			
			if (!bEmote)
			{
				pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", start + "`6<`w" + pPlayer->GetName() + "``>`` `" + cs + input + "``" });
			}

		});
	}
	
} // namespace Listeners