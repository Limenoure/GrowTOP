#include <BaseApp.hpp> //precomp

#include <World/WorldObjectMap.hpp>
#include <ProtonSDK/MiscUtils.h>




WorldObject* WorldObjectMap::GetObjectByID(const int& id) 
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* pObject = m_objects[i];
		if (!pObject || pObject->objectID != id)
		{
			continue;
		}

		return pObject;
	}

	return nullptr;
}

void WorldObjectMap::AddObject(const WorldObject& object) 
{
	WorldObject* pObject = new WorldObject();
	pObject->ID = object.ID;
	pObject->x = object.x;
	pObject->y = object.y;
	pObject->flags = object.flags;
	pObject->count = object.count;
	pObject->objectID = m_objectID++;

	m_objects.push_back(std::move(pObject));
}

void WorldObjectMap::RemoveObject(const int& id) 
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&id](WorldObject* pObject)
	{
		return pObject && pObject->objectID == id;
	});

	if (it != m_objects.end())
	{
		delete *it;
		m_objects.erase(it);
	}
}

size_t WorldObjectMap::GetMemoryEstimated() 
{
	size_t estimated = sizeof(int) * 2; //object ID and offset

	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* pObject = m_objects[i];
		if (!pObject)
		{
			continue;
		}

		estimated += pObject->GetMemoryEstimated();
	}

	return estimated;
}


void WorldObjectMap::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */) 
{
	if (!pMem)
	{
		return;
	}

	int objectOffset = (int)m_objects.size();
	int objectID = bPacket ? m_objectID - 1 : m_objectID;
	MemorySerialize(&objectOffset, pMem, offset, true);
	MemorySerialize(&objectID, pMem, offset, true);

	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* pObject = m_objects[i];
		if (!pObject)
		{
			continue;
		}

		pObject->SerializeToMem(pMem, offset);
	}
}

void WorldObjectMap::SerializeFromMem(uint8_t* pMem, int& offset) 
{
	if (!pMem)
	{
		return;
	}

	int objectOffset = 0;
	MemorySerialize(&objectOffset, pMem, offset, false);
	MemorySerialize(&m_objectID, pMem, offset, false);

	for (int i = 0; i < objectOffset; i++) 
	{
		WorldObject* pObject = new WorldObject();
		pObject->SerializeFromMem(pMem, offset);

		m_objects.push_back(std::move(pObject));
	}
}