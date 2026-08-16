#include <BaseApp.hpp> //precomp

#include <Player/NetAvatar.hpp>
#include <Player/Player.hpp>
#include <World/World.hpp>

NetAvatar::NetAvatar(Player* pPlayer)
{
	m_pPlayer = pPlayer;

	if (m_pPlayer)
	{
		m_pPlayer->SetNetID(m_netID);
	}
}

NetAvatar::~NetAvatar()
{
	if (!m_pPlayer)
	{
		return;
	}
	
	m_pPlayer->SetNetID(-1);
	m_pPlayer->SetWorld(nullptr);
}

void NetAvatar::SetWorld(World* pWorld)
{
	if (!pWorld || m_pWorld || !m_pPlayer)
	{
		return;
	}

	m_pWorld = pWorld;
	m_pPlayer->SetWorld(pWorld);
	m_pPlayer->SetNetID(m_netID);
}

std::string NetAvatar::GetSpawnData(const bool& bLocal /* = false */)
{
	if (!m_pPlayer)
	{
		return "";
	}

	int modState = 0;
	int smodState = 0;
	if (m_pPlayer->GetRole() > ROLE_VIP)
	{
		modState = m_pPlayer->GetRole();
	}

	if (m_pPlayer->GetRole() > ROLE_CUSTOM)
	{
		smodState = m_pPlayer->GetRole();
	}

	std::string data = "spawn|avatar\n";
	data.append("netID|" + std::to_string(m_netID) + "\n");
	data.append("userID|" + std::to_string(m_pPlayer->GetUserID()) + "\n");
	data.append("colrect|0|0|20|30\n");
	data.append("posXY|" + std::to_string((int)m_vec.X) + "|" + std::to_string((int)m_vec.Y) + "\n");
	data.append("name|`w" + m_pPlayer->GetName() + "``\n");
	data.append("titleIcon|" + m_pPlayer->GetTitleIcon() + "\n");
	data.append("country|" + m_pPlayer->GetTankInfo()->country + "\n");
	data.append("invis|" + std::to_string((int)m_pPlayer->HasBit(PLAYERTEMPFLAG_INVIS)) + "\n");
	data.append("mstate|" + std::to_string(modState) + "\nsmstate|" + std::to_string(modState) + "\n");
	data.append("onlineID|" + std::to_string(m_pPlayer->GetOnlineID()) + "");
	
	if (bLocal)
	{
		data.append("\ntype|local");
	}

	return data;
}

std::string NetAvatar::GetMusicNoteStandingOn()
{
	std::string noteStandingOn = "A";

	switch (static_cast<int>(m_vec.Y / 32.f) % 14) 
	{
	    case 0: { noteStandingOn = "B"; } break;
		case 1: { noteStandingOn = "A"; } break;
		case 2: { noteStandingOn = "G"; } break;
		case 3: { noteStandingOn = "F"; } break;
		case 4: { noteStandingOn = "E"; } break;
		case 5: { noteStandingOn = "D"; } break;
		case 6: { noteStandingOn = "C"; } break;
		case 7: { noteStandingOn = "B"; } break;
		case 8: { noteStandingOn = "A"; } break;
		case 9: { noteStandingOn = "G"; } break;
		case 10: { noteStandingOn = "F"; } break;
		case 11: { noteStandingOn = "E"; } break;
		case 12: { noteStandingOn = "D"; } break;
		case 13: { noteStandingOn = "C"; } break;
		default: { noteStandingOn = "A"; } break;
	}

	return noteStandingOn;
}

void NetAvatar::SendCharacterState(Player* pPlayer)
{
	if (!m_pPlayer || !pPlayer)
	{
		return;
	}

	GameUpdatePacket statePacket;
	statePacket.type = NET_GAME_PACKET_SET_CHARACTER_STATE;
	statePacket.punchID = 0;
	statePacket.buildRange = 128;
	statePacket.punchRange = 128;
	statePacket.netID = m_netID;
	statePacket.flags = m_pPlayer->GetCharacterState().GetEffectFlags();
	statePacket.waterSpeed = m_pPlayer->GetCharacterState().GetWaterSpeed();
	statePacket.effectFlags = m_pPlayer->GetCharacterState().GetStateFlags();
	statePacket.accel = m_pPlayer->GetCharacterState().GetAcceleration();
	statePacket.punchStrength = m_pPlayer->GetCharacterState().GetKnockPower();
	statePacket.speedOut = m_pPlayer->GetCharacterState().GetSpeed();
	statePacket.gravityOut = m_pPlayer->GetCharacterState().GetGravity();
	statePacket.pupilColor = m_pPlayer->GetCharacterState().GetLenColor();
	statePacket.eyeColor = m_pPlayer->GetCharacterState().GetEyesColor();
	statePacket.hairColor = m_pPlayer->GetCharacterState().GetHairColor();

	pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &statePacket, sizeof(GameUpdatePacket));
}

void NetAvatar::SendClothes(Player* pPlayer, const bool& bEquipSound /* = false */)
{
	if (!m_pPlayer || !pPlayer)
	{
		return;
	}

	pPlayer->SendVariantPacket({ "OnSetClothing",
		CL_Vec3f{ (float)m_pPlayer->GetItems()->GetCloth(HAT), (float)m_pPlayer->GetItems()->GetCloth(SHIRT), (float)m_pPlayer->GetItems()->GetCloth(PANTS) },
		CL_Vec3f{ (float)m_pPlayer->GetItems()->GetCloth(SHOES), (float)m_pPlayer->GetItems()->GetCloth(FACEITEM), (float)m_pPlayer->GetItems()->GetCloth(HAND) },
		CL_Vec3f{ (float)m_pPlayer->GetItems()->GetCloth(BACK), (float)m_pPlayer->GetItems()->GetCloth(HAIR), (float)m_pPlayer->GetItems()->GetCloth(CHESTITEM) },
		m_pPlayer->GetSkinColor(),
		CL_Vec3f{ (float)m_pPlayer->GetItems()->GetCloth(ARTIFACT), (float)bEquipSound, 0.f }
	}, m_netID);
}

void NetAvatar::Update()
{
	if (!m_pPlayer || !m_pWorld)
	{
		return;
	}

	m_pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
	{
		SendCharacterState(pTargetPlayer);
		SendClothes(pTargetPlayer);
	});
}
void NetAvatar::SendExperience(const int& xp)
{
	if (!m_pWorld || xp < 1)
	{
		return;
	}

	int xpToAdd = xp;
	while (xpToAdd > 0)
	{
		if (m_pPlayer->GetXP() + xpToAdd >= 50 * ((m_pPlayer->GetLevel() * m_pPlayer->GetLevel()) + 2))
		{
			int tempXP = xpToAdd;
			m_pPlayer->SetLevel(m_pPlayer->GetLevel() + 1);
			m_pPlayer->SetXP(m_pPlayer->GetXP() + tempXP - 50 * ((m_pPlayer->GetLevel() * m_pPlayer->GetLevel()) + 2));
			xpToAdd -= 50 * ((m_pPlayer->GetLevel() * m_pPlayer->GetLevel()) + 2) - m_pPlayer->GetXP() + xpToAdd;

			m_pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
			{
				pTargetPlayer->SendVariantPacket({ "OnPlayerLeveledUp", m_pPlayer->GetLevel() }, m_netID);
				pTargetPlayer->SendVariantPacket({ "OnParticleEffect", 46, m_vec, 0.0f, 0.0f });
				pTargetPlayer->SendVariantPacket({ "OnTalkBubble", m_netID, "`w" + m_pPlayer->GetName() + "``" + " is now level " + std::to_string(m_pPlayer->GetLevel()) + "!", 0 });
				pTargetPlayer->SendVariantPacket({ "OnConsoleMessage", "`w" + m_pPlayer->GetName() + "``" + " is now level " + std::to_string(m_pPlayer->GetLevel()) + "!" });
				pTargetPlayer->SendVariantPacket({ "OnPlayPositioned", "audio/levelup.wav" });
			});

			if (m_pPlayer->GetLevel() == 5)
			{
				m_pPlayer->SendVariantPacket({ "OnAddNotification", "interface/large/friend_button.rttex", "You've unlocked some new `$Growmojis``!", "", 0 });
				m_pPlayer->m_growmojiFlags |= PLAYERGROWMOJI_FLAGS1_SIGH | PLAYERGROWMOJI_FLAGS1_MAD | PLAYERGROWMOJI_FLAGS1_SMILE | PLAYERGROWMOJI_FLAGS1_TONGUE | PLAYERGROWMOJI_FLAGS1_WOW;
				m_pPlayer->OnEmoticonDataChanged();
			}

			if (m_pPlayer->GetLevel() % 2 == 0 && m_pPlayer->GetLevel() <= 20)
			{
				int award = m_pPlayer->GetLevel() * 50;
				if (m_pPlayer->GetLevel() % 10 == 0)
				{
					award = m_pPlayer->GetLevel() * 150;
				}

				m_pPlayer->GetItems()->SetGems(m_pPlayer->GetItems()->GetGems() + award);
				m_pPlayer->SendVariantPacket({ "OnConsoleMessage", "`2Glory of Growtopians``: For achieving level `w" + std::to_string(m_pPlayer->GetLevel()) + "``, you have been awarded `w" + std::to_string(award) + "`` Gems!" });
				if (m_pPlayer->GetLevel() == 20)
				{
					m_pPlayer->SendVariantPacket({ "OnConsoleMessage", "`2Glory of Growtopians``: Congratulations on reaching your pinnacle of glory! However, from now on, you will no longer receive gems as rewards for leveling up." });
				}

				m_pPlayer->OnSetBux();
			}

			if (m_pPlayer->GetLevel() >= 125)
			{
				m_pPlayer->ToggleBit(PLAYERFLAG_BLUE_NAME_TITLE, true);
				m_pPlayer->ToggleBit(PLAYERFLAG_BLUE_NAME_TITLE_ENABLED, true);
				m_pWorld->Broadcast([&](int netID, Player* pTargetPlayer)
				{
					pTargetPlayer->SendVariantPacket({ "OnCountryState", m_pPlayer->GetCountryState() }, m_netID);
				});
			}
		}

		xpToAdd -= xp;
		m_pPlayer->SetXP(m_pPlayer->GetXP() + xp);
	}
}

void NetAvatar::OnConsoleMessage(const std::string& msg, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnConsoleMessage", msg }, -1, delay);
}

void NetAvatar::OnTalkBubble(const int& playerNetID, const std::string& msg, const int& bubbleType /* = 0 */, const bool& bOverride /* = false */, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnConsoleMessage", (uint32_t)playerNetID, msg, (uint32_t)bubbleType, (uint32_t)bOverride }, -1, delay);
}

void NetAvatar::OnDialogRequest(const std::string& dialog, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnDialogRequest", dialog }, -1, delay);
}

void NetAvatar::OnTextOverlay(const std::string& msg, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnTextOverlay", msg }, -1, delay);
}

void NetAvatar::OnRequestWorldSelectManu(const std::string& msg, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnRequestWorldSelectManu", msg }, -1, delay);
}

void NetAvatar::OnInvis(const bool& bInvisible, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnInvis", (uint32_t)bInvisible }, -1, delay);
}

void NetAvatar::OnFailedToEnterWorld(const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnFailedToEnterWorld" }, -1, delay);
}

void NetAvatar::OnSpawn(const std::string& data, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnSpawn", data }, -1, delay);
}

void NetAvatar::OnRemove(const std::string& netID, const std::string& playerID, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnRemove", netID, playerID }, -1, delay);
}

void NetAvatar::OnNameChanged(const std::string& name, const std::string& titleIcon, const int& netID, const int& delay /* = 0 */)
{
	if (!m_pPlayer)
	{
		return;
	}

	m_pPlayer->SendVariantPacket({ "OnRemove", name, titleIcon }, netID, delay);
}

void NetAvatar::LogMsg(const char* traceStr, ...)
{
	if (!m_pPlayer)
	{
		return;
	}

	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset(buffer, 0, logSize);
	va_start(argsVA, traceStr);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, traceStr, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, traceStr, argsVA);
#else
	vsnprintf(buffer, logSize, traceStr, argsVA);
#endif

	va_end(argsVA);

	m_pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + std::string(buffer));
}