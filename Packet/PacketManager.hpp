#ifndef PACKET__PACKETMANAGER__HPP
#define PACKET__PACKETMANAGER__HPP
#include <enet/enet.h>

#include <Packet/GameUpdatePacket.hpp>


class PacketManager 
{
public:
	PacketManager() = default;
	~PacketManager() = default;

	int GetMessageType(ENetPacket* pPacket);
	const char* GetTextPointer(ENetPacket* pPacket);
	GameUpdatePacket* GetGameUpdatePacket(ENetPacket* pPacket);
};

PacketManager* GetPacketManager();

#endif // PACKET__PACKETMANAGER__HPP