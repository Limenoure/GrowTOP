#ifndef WORLD__WORLDOBJECT__HPP
#define WORLD__WORLDOBJECT__HPP

#include <cstdint>

enum eObjectChangeTypes 
{
	CHANGETYPE_EDIT = -3,
	CHANGETYPE_REMOVE, //-2
	CHANGETYPE_SPAWN, //-1
	CHANGETYPE_PICKUP //0
};

//eWorldObjectFlags
#define WORLDOBJECTFLAG_SHRINK 0x0001
#define WORLDOBJECTFLAG_UNKNOWN_2 0x0002
#define WORLDOBJECTFLAG_UNPICKED_FOSSIL 0x0004
#define WORLDOBJECTFLAG_RADIOACTIVE 0x0008
#define WORLDOBJECTFLAG_UNKNOWN_16 0x0010
#define WORLDOBJECTFLAG_UNKNOWN_32 0x0020
#define WORLDOBJECTFLAG_UNKNOWN_64 0x0040
#define WORLDOBJECTFLAG_UNKNOWN_128 0x0080

class WorldObject 
{
public:
	WorldObject() = default;
	~WorldObject() = default;

	size_t GetMemoryEstimated();
	void SerializeToMem(uint8_t* pMem, int& offset);
	void SerializeFromMem(uint8_t* pMem, int& offset);

public:
	short ID = 0;
	float x = 0.f;
	float y = 0.f;

	uint8_t count = 0;
	uint8_t flags = 0;

	int objectID = 0;
};

#endif // WORLD__WORLDOBJECT__HPP