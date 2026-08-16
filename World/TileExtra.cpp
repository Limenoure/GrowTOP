#include <BaseApp.hpp> //precomp

#include <World/TileExtra.hpp>
#include <Player/Player.hpp>

//Door
size_t TileExtraDoor::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t) + m_label.length();
	estimated += sizeof(uint8_t); //door flags

	if (!bPacket)
	{
		estimated += sizeof(uint16_t) + m_id.length();
		estimated += sizeof(uint16_t) + m_destination.length();
		estimated += sizeof(uint16_t) + m_password.length();
	}

	return estimated;
}

void TileExtraDoor::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(m_label, pMem, offset, true);
	MemorySerialize(&m_flags, pMem, offset, true);

	if (!bPacket)
	{
		MemorySerialize(m_id, pMem, offset, true);
		MemorySerialize(m_destination, pMem, offset, true);
		MemorySerialize(m_password, pMem, offset, true);
	}
}

void TileExtraDoor::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(m_label, pMem, offset, false);
	MemorySerialize(&m_flags, pMem, offset, false);

	if (!bPacket)
	{
		MemorySerialize(m_id, pMem, offset, false);
		MemorySerialize(m_destination, pMem, offset, false);
		MemorySerialize(m_password, pMem, offset, false);
	}
}



//Sign
size_t TileExtraSign::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t) + m_label.length();
	estimated += sizeof(int); //sign flags(used only for security camera/CCTV)

	if (!bPacket)
	{
		estimated += sizeof(uint16_t) + m_id.length();
	}

	return estimated;
}

void TileExtraSign::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(m_label, pMem, offset, true);
	MemorySerialize(&m_flags, pMem, offset, true);

	if (!bPacket)
	{
		MemorySerialize(m_id, pMem, offset, true);
	}
}

void TileExtraSign::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(m_label, pMem, offset, false);
	MemorySerialize(&m_flags, pMem, offset, false);

	if (!bPacket)
	{
		MemorySerialize(m_id, pMem, offset, false);
	}
}

//Donation Box
size_t TileExtraDonationBox::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t);
	estimated += sizeof(int);

	if (!bPacket)
	{
		estimated += sizeof(uint8_t); //donations size
		for (const auto& [ID, info] : m_donations)
		{
			estimated += sizeof(int); //DonationInfo.userID
			estimated += sizeof(uint16_t) + info.lastName.size();
			estimated += sizeof(uint16_t); //DonationItem::itemID
			estimated += sizeof(uint8_t); //DonationItem::count
			estimated += sizeof(uint16_t) + info.message.size();
		}
	}

	return estimated;
}

void TileExtraDonationBox::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	std::string reserved = "";
	int reserved2 = -1;
	MemorySerialize(reserved, pMem, offset, true);
	MemorySerialize(&reserved2, pMem, offset, true);

	if (!bPacket)
	{
		uint8_t size = (int)m_donations.size();
		MemorySerialize(&size, pMem, offset, true);
		for (auto& [ID, info] : m_donations)
		{
			MemorySerialize(&info.userID, pMem, offset, true);
			MemorySerialize(info.lastName, pMem, offset, true);
			MemorySerialize(&info.itemID, pMem, offset, true);
			MemorySerialize(&info.count, pMem, offset, true);
			MemorySerialize(info.message, pMem, offset, true);
		}
	}
}

void TileExtraDonationBox::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	std::string reserved = "";
	int reserved2 = -1;
	MemorySerialize(reserved, pMem, offset, false);
	MemorySerialize(&reserved2, pMem, offset, false);

	if (!bPacket)
	{
		uint8_t size = 0;
		MemorySerialize(&size, pMem, offset, false);
		for (uint8_t i = 0; i < size; i++)
		{
			DonationInfo info = m_donations[i];
			MemorySerialize(&info.userID, pMem, offset, false);
			MemorySerialize(info.lastName, pMem, offset, false);
			MemorySerialize(&info.itemID, pMem, offset, false);
			MemorySerialize(&info.count, pMem, offset, false);
			MemorySerialize(info.message, pMem, offset, false);

			m_donations.insert_or_assign(i, info);
		}
	}
}

int TileExtraDonationBox::GetGifts(const int& userID) const
{
	int count = 0;
	for (const auto& [ID, info] : m_donations)
	{
		if (info.userID != userID)
		{
			continue;
		}

		++count;
	}

	return count;
}

//Lock
size_t TileExtraLock::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(uint8_t); //lock flags
	estimated += sizeof(int); //owner's userID

	estimated += sizeof(int); //admins array length
	estimated += m_admins.size() * sizeof(int); //admins

	//estimated += sizeof(int); //world tempo

	return estimated;
}

void TileExtraLock::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_flags, pMem, offset, true);
	MemorySerialize(&m_ownerID, pMem, offset, true);

	int admins = (int)m_admins.size();
	MemorySerialize(&admins, pMem, offset, true);
	for (int i = 0; i < admins; i++)
	{
		int admin = m_admins[i];
		MemorySerialize(&admin, pMem, offset, true);
	}

	//int tempo = bPacket ? m_tempo * -1 : m_tempo;
	//MemorySerialize(&tempo, pMem, offset, true);

	//TODO: figure when did other stuff came and handle world map versions
}

void TileExtraLock::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_flags, pMem, offset, false);
	MemorySerialize(&m_ownerID, pMem, offset, false);


	int admins = 0;
	MemorySerialize(&admins, pMem, offset, false);
	for (int i = 0; i < admins; i++)
	{
		int admin = -1;
		MemorySerialize(&admin, pMem, offset, false);
		m_admins.push_back(admin);
	}

	//int tempo = 0;
	//MemorySerialize(&tempo, pMem, offset, true);
	//m_tempo = bPacket ? tempo * -1 : tempo;

	//TODO: figure when did other stuff came and handle world map versions
}

bool TileExtraLock::IsAdmin(Player* pPlayer)
{
	if (!pPlayer)
	{
		return false;
	}

	return std::find(m_admins.begin(), m_admins.end(), pPlayer->GetUserID()) != m_admins.end();
}

//Seeds
size_t TileExtraSeed::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += bPacket ? 4 : 8; //TileExtraSeed::m_growingSince
	estimated += sizeof(uint8_t); //TileExtraSeed::m_fruits
	
	return estimated;
}

void TileExtraSeed::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	if (!bPacket) 
	{
		uint64_t time = std::chrono::duration_cast<std::chrono::seconds>(m_growingSince.time_since_epoch()).count();
		MemorySerialize(&time, pMem, offset, true);
	}
	else 
	{
		int time = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_growingSince).count());
		MemorySerialize(&time, pMem, offset, true);
	}

	MemorySerialize(&m_fruits, pMem, offset, true);
}

void TileExtraSeed::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	if (!bPacket)
	{
		uint64_t time = std::chrono::duration_cast<std::chrono::seconds>(m_growingSince.time_since_epoch()).count();
		MemorySerialize(&time, pMem, offset, false);
		m_growingSince = std::chrono::steady_clock::time_point(std::chrono::seconds(time));
	}
	else
	{
		int time = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_growingSince).count());
		MemorySerialize(&time, pMem, offset, false);
		auto timeSinceGrowing = std::chrono::seconds(time);
		m_growingSince = std::chrono::time_point_cast<std::chrono::steady_clock::duration>(m_growingSince + timeSinceGrowing);
	}

	MemorySerialize(&m_fruits, pMem, offset, false);
}

//Dice Block & Roshambo Block
size_t TileExtraRoll::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;
	estimated += sizeof(uint8_t); //TileExtraSeed::m_roll

	return estimated;
}

void TileExtraRoll::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_roll, pMem, offset, true);
}

void TileExtraRoll::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_roll, pMem, offset, false);
}

//Display Block
size_t TileExtraDisplayBlock::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(int); //TileExtraDisplayBlock::m_itemID
	return estimated;
}

void TileExtraDisplayBlock::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_itemID, pMem, offset, true);
}

void TileExtraDisplayBlock::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_itemID, pMem, offset, false);
}

//Unstable Tesseract | Gaia's Beacon | MAGPLANT 5000
size_t TileExtraItemSucker::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	size_t estimated = 0;

	estimated += sizeof(int); //TileExtraItemSucker::m_itemID
	estimated += sizeof(int); //TileExtraItemSucker::m_count

	estimated += sizeof(bool); //TileExtraItemSucker::m_bMagnet
	estimated += sizeof(bool); //TileExtraItemSucker::m_bBuildingMode
	estimated += sizeof(int); //TileExtraItemSucker::m_capacity
	return estimated;
}

void TileExtraItemSucker::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_itemID, pMem, offset, true);
	MemorySerialize(&m_count, pMem, offset, true);
	MemorySerialize(&m_bMagnet, pMem, offset, true);
	MemorySerialize(&m_bBuildingMode, pMem, offset, true);
	MemorySerialize(&m_capacity, pMem, offset, true);
}

void TileExtraItemSucker::SerializeFromMem(uint8_t* pMem, int& offset, const bool& bPacket /* = false */, const uint16_t& mapVersion /* = 23 */)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&m_itemID, pMem, offset, false);
	MemorySerialize(&m_count, pMem, offset, false);
	MemorySerialize(&m_bMagnet, pMem, offset, false);
	MemorySerialize(&m_bBuildingMode, pMem, offset, false);
	MemorySerialize(&m_capacity, pMem, offset, false);
}