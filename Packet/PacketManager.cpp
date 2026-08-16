#include <BaseApp.hpp> //precomp
#include <Packet/PacketManager.hpp>

PacketManager g_packetManager;
PacketManager* GetPacketManager() { return &g_packetManager; }

int PacketManager::GetMessageType(ENetPacket* pPacket) 
{
	//gets eNetMessageType, every packet sent by the growtopia client contains it as a header, it's type int.
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	if (!pPacket || pPacket->dataLength < 4)
	{
		return -1;
	}

	return *(int*)pPacket->data;
}

const char* PacketManager::GetTextPointer(ENetPacket* pPacket) 
{
	//gets the text data for packets sent with net message type NET_MESSAGE_GENETIC_TEXT(2) or NET_MESSAGE_GAME_MESSAGE(3), they contain text data after the net message type.
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	if (!pPacket || pPacket->dataLength < 4 || pPacket->dataLength > 1024)
	{
		//too short or long
		return "";
	}

	memset(pPacket->data + pPacket->dataLength - 1, 0, 1);
	return (const char*)pPacket->data + 4;
}

GameUpdatePacket* PacketManager::GetGameUpdatePacket(ENetPacket* pPacket) 
{
	//gets GameUpdatePacket pointer for packets sent with net message type NET_MESSAGE_GAME_PACKET(4)
#ifdef _MSC_VER
	g_lastCall = __FUNCSIG__;
#else
	g_lastCall = __PRETTY_FUNCTION__;
#endif
	if (!pPacket || pPacket->dataLength < 60 || pPacket->dataLength > 61)
	{
		return nullptr;
	}

	return reinterpret_cast<GameUpdatePacket*>(pPacket->data + 4);
}