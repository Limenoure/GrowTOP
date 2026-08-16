#ifndef WORLD__WORLDOBJECTMAP__HPP
#define WORLD__WORLDOBJECTMAP__HPP

#include <string>
#include <vector>
#include <ProtonSDK/Math.h>
#include <World/WorldObject.hpp>

class WorldObjectMap 
{
public:
	WorldObjectMap() = default;
	~WorldObjectMap() 
	{
		m_objectID = 0;
		m_objects.clear();
	}


	int GetObjectID(const bool& bIncrement = false) { return bIncrement ? m_objectID++ : m_objectID; }
	void SetObjectID(const int& id) { m_objectID = id; }

	void AddObject(const WorldObject& object);
	void RemoveObject(const int& id);

	WorldObject* GetObjectByID(const int& id);

	std::vector<WorldObject*>& GetObjects() { return m_objects; }

	size_t GetMemoryEstimated();
	void SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket = true);
	void SerializeFromMem(uint8_t* pMem, int& offset);

private:
	int m_objectID = 0;

	std::vector<WorldObject*> m_objects;

};

#endif // WORLD__WORLDOBJECTMAP__HPP