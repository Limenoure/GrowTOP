#ifndef WORLD__TILEEXTRAMANAGER__HPP
#define WORLD__TILEEXTRAMANAGER__HPP

#include <string>

class ItemInfo;
class TileExtraManager
{
public:
	TileExtraManager() = default;
	~TileExtraManager() = default;

	bool Supported(const uint8_t& type, const uint16_t& mapVersion);

};

TileExtraManager* GetTileExtraManager();

#endif // WORLD__TILEEXTRAMANAGER__HPP