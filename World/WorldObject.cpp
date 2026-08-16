#include <BaseApp.hpp> //precomp

#include <World/WorldObject.hpp>

#include <ProtonSDK/MiscUtils.h>


size_t WorldObject::GetMemoryEstimated() 
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t); //WorldObject::ID
	estimated += sizeof(float) * 2; //WorldObject::x, y
	estimated += sizeof(uint8_t); //WorldObject::count
	estimated += sizeof(uint8_t); //WorldObject::flags
	estimated += sizeof(int); //WorldObject::objectID

	return estimated;
}

void WorldObject::SerializeToMem(uint8_t* pMem, int& offset) 
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&ID, pMem, offset, true);
	MemorySerialize(&x, pMem, offset, true);
	MemorySerialize(&y, pMem, offset, true);
	MemorySerialize(&count, pMem, offset, true);
	MemorySerialize(&flags, pMem, offset, true);
	MemorySerialize(&objectID, pMem, offset, true);
}

void WorldObject::SerializeFromMem(uint8_t* pMem, int& offset)
{
	if (!pMem)
	{
		return;
	}

	MemorySerialize(&ID, pMem, offset, false);
	MemorySerialize(&x, pMem, offset, false);
	MemorySerialize(&y, pMem, offset, false);
	MemorySerialize(&count, pMem, offset, false);
	MemorySerialize(&flags, pMem, offset, false);
	MemorySerialize(&objectID, pMem, offset, false);
}