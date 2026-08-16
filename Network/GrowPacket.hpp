#ifndef NETWORK__GROWPACKET__HPP
#define NETWORK__GROWPACKET__HPP
#include <string>



enum eGrowMessageType : uint8_t
{
	GROWMESSAGE_TYPE_AUTHENTICATE

};

enum eGrowPacketType : uint8_t
{
	GROWPACKET_TYPE_AUTHENTICATE, //auth to master server
	GROWPACKET_TYPE_SHUT_DOWN, //server stops
	GROWPACKET_TYPE_SWITCH, //player switch servers

};

struct NetPacket
{
	uint8_t messageType;
	union
	{
		std::string data = "";
		std::string authKey;
	};

	char* pData = nullptr;

};

struct GrowPacket
{
	uint8_t type;
	uint16_t port;

	uint32_t flags;
	uint32_t intData;

	uint32_t authError = -1;

	uint32_t dataLength = 0;
	uint8_t extendedData[];

};

#endif // NETWORK__GROWPACKET__HPP