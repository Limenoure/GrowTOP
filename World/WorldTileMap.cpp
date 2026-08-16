#include <BaseApp.hpp> //precomp

#include <ProtonSDK/MiscUtils.h>

#include <World/WorldTileMap.hpp>
#include <Items/ItemInfoManager.hpp>

#include <BaseApp.hpp>

Tile* WorldTileMap::GetTile(const int& x, const int& y) 
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return nullptr;
	}

	return &m_tiles[x + (y * m_width)];
}

Tile* WorldTileMap::GetTile(const uint16_t& index) 
{
	if (index < 0 || index >= m_width * m_height)
	{
		return nullptr;
	}

	return &m_tiles[index];
}

Tile* WorldTileMap::GetTile(const float& x, const float& y) 
{
	if ((int)x < 0 || (int)x >= m_width || (int)y < 0 || (int)y >= m_height)
	{
		return nullptr;
	}

	return &m_tiles[(int)x + ((int)y * m_width)];
}

Tile* WorldTileMap::GetTile(const CL_Vec2f& vec) 
{
	if (vec.X < 0 || vec.X >= (float)m_width || vec.Y < 0 || vec.Y >= (float)m_height)
	{
		return nullptr;
	}

	return &m_tiles[(int)vec.X + ((int)vec.Y * m_width)];
}

Tile* WorldTileMap::GetTile(const CL_Vec2i& vec) 
{
	if (vec.X < 0 || vec.X >= (int)m_width || vec.Y < 0 || vec.Y >= (int)m_height)
	{
		return nullptr;
	}

	return &m_tiles[vec.X + (vec.Y * (int)m_width)];
}

void WorldTileMap::GenerateTerrain(const uint8_t& terraType /* = 0 */, uint8_t width /* = 100 */, uint8_t height /* = 60 */) 
{
	if (width < 30)
	{
		width = 30;
	}

	if (width > 255)
	{
		width = 255;
	}

	if (height < 30)
	{
		height = 30;
	}

	if (height > 255)
	{
		height = 255;
	}

	m_width = width;
	m_height = height;

	switch (terraType) 
	{
	    case TERRATYPE_CAVE: 
		{
			//i think it's a no secret what gt uses for caves, algorithm is called 'Cellular Automata' (https://code.tutsplus.com/generate-random-cave-levels-using-cellular-automata--gamedev-9664t) ~ Hexago
			//needs little rework cause it's too messy but it works 1:1 basically so i wont bother fucking around with it
			//since i don't wanna be the bad guy and i respect axl... axl is the person behind the cave terrain generating so credits to him :) ~ Hexago
			
		    uint8_t mainDoorX = (rand() % width - 3) + 1;

			int generatorXOffset = 0;
			int generatorYOffset = 1;
			int generatorWidthOffset = 0;
			int generatorHeightOffset = -6;

			int chanceToStartAlive = 35; // 35
			int birthLimit = 3; // 3
			int deathLimit = 3; // 3
			int numberOfSteps = 2; // 2

			int treasureHiddenLimit = 4;
			int treasureChance = 50;

			std::vector<bool> arr(width * height);

			m_tiles.resize(width * height);
			auto Rand = [&](int min, int max) 
			{
				static std::mt19937 generator(std::random_device{}());
				std::uniform_int_distribution<int> distribution(min, max);
				return distribution(generator);
			};

			auto InitializeMap = ([&](std::vector<bool>& bMap) 
			{
				for (int x = generatorXOffset; x < width + generatorWidthOffset; x++) 
				{
					for (int y = generatorYOffset; y < height + generatorHeightOffset; y++) 
					{
						if (Rand(0, 100) < chanceToStartAlive)
						{
							bMap[x + y * width] = true;
						}
					}
				}
			});

			auto CountAliveNeighbors = ([&](std::vector<bool>& bMap, int x, int y) 
			{
				int count = 0;

				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++) 
					{
						int nX = x + i;
						int nY = y + j;

						if (nX < generatorXOffset || nY < generatorYOffset || nX >= width + generatorWidthOffset || nY >= height + generatorHeightOffset)
						{
							count++;
						}

						else if (bMap[nX + nY * width] == true)
						{
							count++;
						}
					}
				}

				return count;
			});

			auto SimulateStep = ([&](std::vector<bool>& bMap) 
				{
				std::vector<bool> tempArr(width * height);

				for (int x = generatorXOffset; x < width + generatorWidthOffset; x++) 
				{
					for (int y = generatorYOffset; y < height + generatorHeightOffset; y++) 
					{
						int nbs = CountAliveNeighbors(bMap, x, y);

						if (bMap[x + y * width]) 
						{
							if (nbs < deathLimit)
							{
								tempArr[x + y * width] = false;
							}
							else
							{
								tempArr[x + y * width] = true;
							}
						}

						else 
						{
							if (nbs > birthLimit)
							{
								tempArr[x + y * width] = true;
							}
							else
							{
								tempArr[x + y * width] = false;
							}
						}

					}
				}

				return tempArr;
			});

			InitializeMap(arr);

			for (int i = 0; i < numberOfSteps; i++)
			{
				arr = SimulateStep(arr);
			}

			for (int i = 0; i < width * height; ++i) 
			{
				CL_Vec2i pos = { i % width, i / width };

				Tile tile(0, 0, 0, 0);
				tile.SetIndex(i);

				if (arr[i] == true) 
				{
					tile.SetForeground(ITEM_ID_CAVE_DIRT);
					if (Rand(0, 35) < 1)
					{ //giving it the blue dots
						tile.ToggleFlag(TILEFLAG_ENABLED, true);
					}
				}

				if ((pos.X == 0 && pos.Y >= 1) || (pos.X == width - 1 && pos.Y >= 1) || pos.Y == 1 || pos.Y == height - 7) 
				{
					tile.SetForeground(ITEM_ID_CAVE_DIRT);

					if (Rand(0, 35) < 1)
					{
						tile.ToggleFlag(TILEFLAG_ENABLED, true);
					}
				}

				if (pos.Y == 0 && pos.X == mainDoorX) 
				{
					tile.SetForeground(ITEM_ID_MAIN_DOOR);
					m_spawnPoint = { (float)(i % m_width) * 32.f + 5.f, (float)(i / m_width) * 32.f };
				}
				else if (pos.Y > 0) 
				{
					tile.SetBackground(ITEM_ID_DARK_CAVE_BACKGROUND);
				}

				if (pos.Y == 1 && pos.X == mainDoorX)
				{
					tile.SetForeground(ITEM_ID_BEDROCK);
				}

				if (pos.Y >= height - 6)
				{
					tile.SetForeground(ITEM_ID_BEDROCK);
				}

				m_tiles[i] = tile;
			}

			bool bTreasure = false;
			for (int i = 0; i < width * height; ++i) 
			{
				CL_Vec2i pos = { i % width, i / width };
				Tile& tile = m_tiles[i];

				if (pos.X >= 1 && pos.X < width + -1 && pos.Y > 2 && pos.Y < height + -7) 
				{
					if (arr[i] == false && !(rand() % 5)) 
					{
						int nbs = CountAliveNeighbors(arr, pos.X, pos.Y);
						if (nbs >= 5)
						{
							tile.SetForeground(ITEM_ID_AQUA_CAVE_CRYSTAL);
						}
					}

					if (arr[i] == true && !(rand() % 90)) 
					{
						int nbs = CountAliveNeighbors(arr, pos.X, pos.Y);
						if (nbs >= 6)
						{
							if (i + width <= width * height && m_tiles[i + width].GetForeground() == ITEM_ID_CAVE_DIRT)
							{
								tile.SetForeground(ITEM_ID_DEEP_IRON);
							}
						}
					}

					if (arr[i] == false && !(rand() % 5)) 
					{
						int nbs = CountAliveNeighbors(arr, pos.X, pos.Y);
						if (nbs >= 4) 
						{
							Tile* pTop = GetTile(pos.X, pos.Y - 1);
							Tile* pBottom = GetTile(pos.X, pos.Y + 1);

							if (pBottom && pBottom->GetForeground() == ITEM_ID_CAVE_DIRT)
							{
								tile.SetForeground(ITEM_ID_STALAGMITE);
							}
							else if (pTop && pTop->GetForeground() == ITEM_ID_CAVE_DIRT)
							{
								tile.SetForeground(ITEM_ID_STALACTITE);
							}
						}
					}

					if (!(rand() % 100) && !bTreasure) 
					{

						int nbs = CountAliveNeighbors(arr, pos.X, pos.Y);
						if (nbs >= 6 && pos.Y >= m_height / 2) 
						{
							if (i + width <= width * height && m_tiles[i + width].GetForeground() == ITEM_ID_CAVE_DIRT)
							{
								bTreasure = true;
								tile.SetForeground(ITEM_ID_GOLDEN_TREASURE_HOARD);
							}
						}
					}
				}
			}

			for (int x = generatorXOffset; x < width + generatorWidthOffset; x++) 
			{
				if (!(rand() % 45))
				{
					continue;
				}

				for (int y = generatorYOffset; y < height + generatorHeightOffset; y++) 
				{
					Tile& t = m_tiles[x + y * width];

					if (t.GetForeground() == ITEM_ID_CAVE_DIRT)
					{
						Tile* pTile2 = GetTile(x, y + 1);
						Tile* pTile3 = GetTile(x, y + 2);
						if (!pTile2 || pTile3)
						{
							continue;
						}

						if (pTile2->GetForeground() != 0 || pTile3->GetForeground() != 0)
						{
							continue;
						}

						if (!(rand() % 68)) 
						{
							int curX = x;
							int curY = y + 1;
							int length = 0;

							while (true) 
							{
								Tile* pTile = GetTile(curX, curY);
								if (!pTile)
								{
									continue;
								}

								if (pTile->GetForeground() == 0)
								{
									length++;
								}
								else
								{
									break;
								}

								curY++;
							}

							if (length > 16) 
							{
								y = y + length + 1;
								continue;
							}

							curY = y + 1;
							while (true) 
							{
								Tile* pTile = GetTile(curX, curY);
								if (!pTile)
								{
									continue;
								}

								if (pTile->GetForeground() == 0 || pTile->GetForeground() == ITEM_ID_CAVE_PLATFORM)
								{
									pTile->SetForeground(ITEM_ID_CAVE_COLUMN);
								}
								else if (pTile->GetForeground() == ITEM_ID_CAVE_DIRT)
								{
									break;
								}

								curY++;
							}

						}
					}
				}
			}

			for (int y = generatorYOffset; y < height + generatorHeightOffset; y++) 
			{
				if ((rand() % 6))
				{
					continue;
				}

				for (int x = generatorXOffset; x < width + generatorWidthOffset; x++) 
				{
					if ((rand() % 6))
					{
					    continue;
					}

					Tile& t = m_tiles[x + y * width];

					if (t.GetForeground() == ITEM_ID_CAVE_DIRT) 
					{
						Tile* pRight = GetTile(x + 1, y);
						Tile* pLeft = GetTile(x - 1, y);

						if (!(rand() % 65))
						{
							continue;
						}

						if (pRight && (pRight->GetForeground() == 0 || pRight->GetForeground() == ITEM_ID_CAVE_COLUMN)) 
						{
							int curX = x + 1;
							int curY = y;
							int length = 0;
							int randMaxLen = Rand(1, 8);

							while (true) 
							{
								if (length >= randMaxLen)
								{
									break;
								}

								Tile* pTile = GetTile(curX, curY);
								if (!pTile)
								{
									continue;
								}

								if (pTile->GetForeground() == 0 || pTile->GetForeground() == ITEM_ID_CAVE_COLUMN) 
								{
									pTile->SetForeground(ITEM_ID_CAVE_PLATFORM);
									length++;
								}

								else if (pTile->GetForeground() == ITEM_ID_CAVE_DIRT)
								{
									break;
								}

								curX++;
							}

						}
						else if (pLeft && (pLeft->GetForeground() == 0 || pLeft->GetForeground() == ITEM_ID_CAVE_COLUMN)) 
						{
							int curX = x - 1;
							int curY = y;
							int length = 0;
							int randMaxLen = Rand(1, 8);

							while (true) 
							{
								if (length >= randMaxLen)
								{
									break;
								}

								if (curX < 0)
								{
									continue;
								}

								Tile* pTile = GetTile(curX, curY);
								if (!pTile)
								{
									continue;
								}

								if (pTile->GetForeground() == 0) 
								{
									pTile->SetForeground(ITEM_ID_CAVE_PLATFORM);
									length++;
								}
								else
								{
									break;
								}

								curX--;
							}

						}
					}
				}
			}

			for (int x = 0; x < width; x++) 
			{
				for (int y = 1; y < height; y++) 
				{
					Tile* pTile = GetTile(x + y);
					Tile* pTileLeft = GetTile(x - 1, y);
					Tile* pTileRight = GetTile(x + 1, y);
					Tile* pTileTop = GetTile(x, y - 1);
					Tile* pTileBottom = GetTile(x, y + 1);
					if (!pTile || !pTileLeft || !pTileRight || !pTileTop || !pTileBottom)
					{
						continue;
					}

					if (pTile->GetForeground() != ITEM_ID_CAVE_PLATFORM && pTile->GetForeground() != ITEM_ID_CAVE_COLUMN)
					{
						continue;
					}

					if (pTile->GetForeground() == ITEM_ID_CAVE_PLATFORM) 
					{
						if (pTileLeft && pTileLeft->GetForeground() == ITEM_ID_BLANK)
						{
							pTileLeft->SetForeground(ITEM_ID_CAVE_DIRT);
						}
						if (pTileRight && pTileRight->GetForeground() == ITEM_ID_BLANK)
						{
							pTileRight->SetForeground(ITEM_ID_CAVE_DIRT);
						}
					}

					if (pTile->GetForeground() == ITEM_ID_CAVE_COLUMN) 
					{
						if (pTileTop->GetForeground() != ITEM_ID_CAVE_DIRT)
						{
							pTileTop->SetForeground(ITEM_ID_CAVE_DIRT);
						}
						if (pTileBottom->GetForeground() != ITEM_ID_CAVE_DIRT)
						{
							pTileBottom->SetForeground(ITEM_ID_CAVE_DIRT);
						}
					}

				}
			}
	    } break;
	    case TERRATYPE_SUNNY: default: 
		{
			uint16_t bedrockLayer = (width * height) - (width * 6);
			uint16_t bedrockHeight = bedrockLayer / width;

			uint16_t lavaLayer = bedrockLayer - (width * 4);
			uint16_t lavaHeight = lavaLayer / width;

			uint16_t dirtLayer = ((height / 2) * width) - ((height / 10) * width);
			uint16_t dirtHeight = dirtLayer / width;

			uint16_t doorIndex = dirtLayer - (std::rand() % width - 1) + 1;
			uint16_t doorX = Randomizer::Get(1, 99);
			
			if (doorIndex >= dirtLayer)
			{
				doorIndex = dirtLayer - 1;
			}

			if (doorIndex < dirtLayer - width)
			{
				doorIndex = dirtLayer - width;
			}

			int currentHeight = dirtHeight;
			int originHeight = dirtHeight;
			m_tiles.resize(width * height);

			for (uint8_t x = 0; x < width; ++x)
			{
				for (uint8_t y = 0; y < height; ++y)
				{
					Tile t(0, 0, 0, 0);
					t.SetIndex(x + (y * width));

					if (y == currentHeight - 1)
					{
						if (GetBaseApp()->GetConfig().bWorldGrass)
						{
							t.SetForeground(ITEM_ID_GRASS);
						}

						if (GetBaseApp()->GetConfig().bWorldFlowers)
						{
							if (std::rand() % 50 < 15)
							{
								static uint16_t flowers[7] = { ITEM_ID_ROSE, ITEM_ID_DAISY, ITEM_ID_HAPPY_FLOWER_GRASS, ITEM_ID_GRASS, ITEM_ID_POPPY, ITEM_ID_FLOWER_CHECKPOINT, ITEM_ID_SUNFLOWER_CHECKPOINT };
								t.SetForeground(flowers[std::rand() % 7]);
							}
						}

						if (GetBaseApp()->GetConfig().bWorldSeeds)
						{
							if (std::rand() % 50 < 4)
							{
								static uint16_t seeds[8] = { ITEM_ID_DIRT_SEED, ITEM_ID_DOOR_SEED, ITEM_ID_CAVE_BACKGROUND_SEED, ITEM_ID_SIGN_SEED, ITEM_ID_ROCK_SEED, ITEM_ID_LAVA_SEED, ITEM_ID_WOOD_BLOCK_SEED, ITEM_ID_GLASS_PANE_SEED };
								t.SetForeground(seeds[std::rand() % 8]);
							}
						}

						if (x == doorX)
						{
							int i = x + (y * width);
							t.SetForeground(ITEM_ID_MAIN_DOOR);
							t.SetBackground(ITEM_ID_CAVE_BACKGROUND);
							m_spawnPoint = { (float)(i % m_width) * 32.f + 5.f, (float)(i / m_width) * 32.f };
						}
					}

					if (y >= currentHeight)
					{
						t.SetForeground(ITEM_ID_DIRT);
						t.SetBackground(ITEM_ID_CAVE_BACKGROUND);

						if (y >= currentHeight + 1 && y < bedrockHeight && std::rand() % 80 <= 1 && x > 0 && x < width)
						{
							t.SetForeground(ITEM_ID_ROCK);
						}

						if (y >= lavaHeight && y < bedrockHeight && std::rand() % 7 <= 1 && x > 0 && x < width)
						{
							t.SetForeground(ITEM_ID_LAVA);
						}

						if (y >= bedrockHeight)
						{
							t.SetForeground(ITEM_ID_BEDROCK);
						}

						if (x == doorX && y == currentHeight)
						{
							t.SetForeground(ITEM_ID_BEDROCK);
							t.SetBackground(ITEM_ID_CAVE_BACKGROUND);
						}
					}

					m_tiles[x + (y * width)] = t;
				}

				if (GetBaseApp()->GetConfig().bWorldMountain && std::rand() % 10 >= 4)
				{
					currentHeight += Randomizer::Get(-1, 1);
					if (currentHeight < originHeight - 3)
					{
						currentHeight = originHeight - 3;
					}

					if (currentHeight > originHeight + 3)
					{
						currentHeight = originHeight + 3;
					}
				}
			}
	    } break;
	}
}

size_t WorldTileMap::GetMemoryEstimated(const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */) 
{
	size_t estimated = 0;

	estimated += sizeof(int) * 3; //width, height, tiles length(width * height)

	if (bPacket && fGameVersion >= 4.31f)
	{
		estimated += sizeof(int) + sizeof(uint8_t); //new client version requires it
	}

	for (uint16_t i = 0; i < m_tiles.size(); i++)
	{
		Tile tile = m_tiles[i];
		estimated += tile.GetMemoryEstimated(bPacket, fGameVersion, mapVersion);
	}

	return estimated;
}

void WorldTileMap::SerializeToMem(uint8_t* pMem, int& offset, const bool& bPacket /* = true */, const float& fGameVersion /* = 4.59f */, const uint16_t& mapVersion /* = 23 */) 
{
	if (!pMem)
	{
		return;
	}

	int tilesLength = m_width * m_height;
	MemorySerialize(&m_width, pMem, offset, true);
	MemorySerialize(&m_height, pMem, offset, true);
	MemorySerialize(&tilesLength, pMem, offset, true);
	 
	if (bPacket && fGameVersion >= 4.31f) 
	{
		int zero = 0;
		uint8_t zero2 = 0;

		MemorySerialize(&zero, pMem, offset, true);
		MemorySerialize(&zero2, pMem, offset, true);
	}

	for (uint16_t i = 0; i < m_tiles.size(); i++)
	{
		Tile tile = m_tiles[i];
		tile.SerializeToMem(pMem, offset, bPacket, fGameVersion, mapVersion);
	}
}

void WorldTileMap::SerializeFromMem(uint8_t* pMem, int& offset, const uint16_t& mapVersion /* = 23 */) 
{
	if (!pMem)
	{
		return;
	}

	int tilesLength = 0;
	MemorySerialize(&m_width, pMem, offset, false);
	MemorySerialize(&m_height, pMem, offset, false);
	MemorySerialize(&tilesLength, pMem, offset, false);

	m_tiles.resize(tilesLength);
	for (int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].SerializeFromMem(pMem, offset, mapVersion);

		if (m_tiles[i].GetForeground() == ITEM_ID_MAIN_DOOR)
		{
			m_spawnPoint = { (float)(i % m_width) * 32.f + 5.f, (float)(i / m_width) * 32.f };
		}
	}
}

void WorldTileMap::RemoveAllTilesFromThisLock(Tile* pLock)
{
	if (!pLock)
	{
		return;
	}

	ItemInfo* pItem = pLock->GetItem();
	if (!pItem)
	{
		return;
	}

	uint16_t index = pLock->GetIndex();
	for (int i = 0; i < m_tiles.size(); i++)
	{
		Tile* pTile = &m_tiles[i];
		if (!pTile)
		{
			continue;
		}

		if (pItem->lockPower != 0)
		{
			if (pTile->GetParent() == index)
			{
				pTile->SetParent(0);
				pTile->ToggleFlag(TILEFLAG_LOCKED, false);
			}
		}
		else
		{
			if (pTile->GetLockIndex() == index)
			{
				pTile->SetLockIndex(0);
			}
		}
	}
}

bool WorldTileMap::NeighboursThisLock(Tile* pLock, Tile* pTile, const bool& bIgnoreAir)
{
	if (!pLock || !pTile || pTile->GetParent() != 0)
	{
		return false;
	}

	ItemInfo* pItem = pTile->GetItem();
	if (!pItem)
	{
		return false;
	}

	if (pItem->type == TYPE_LOCK || pItem->type == TYPE_MAIN_DOOR || pItem->type == TYPE_BEDROCK)
	{
		return false;
	}

	if (pItem->ID == ITEM_ID_BLANK && bIgnoreAir)
	{
		return false;
	}


	uint16_t lockX = pLock->GetIndex() % m_width;
	uint16_t lockY = pLock->GetIndex() / m_width;
	uint16_t tileX = pTile->GetIndex() % m_width;
	uint16_t tileY = pTile->GetIndex() / m_width;

	if (tileX == lockX && tileY == lockY)
	{
		return false;
	}

	Tile* pNeighbourRight = GetTile(tileX + 1, tileY);
	if (pNeighbourRight)
	{
		if (pNeighbourRight->GetParent() == pLock->GetIndex() || (tileX + 1 == lockX && tileY == lockY))
		{
			return true;
		}
	}

	Tile* pNeighbourBottom = GetTile(tileX, tileY + 1);
	if (pNeighbourBottom)
	{
		if (pNeighbourBottom->GetParent() == pLock->GetIndex() || (tileX == lockX && tileY + 1 == lockY))
		{
			return true;
		}
	}

	Tile* pNeighbourLeft = GetTile(tileX - 1, tileY);
	if (pNeighbourLeft)
	{
		if (pNeighbourLeft->GetParent() == pLock->GetIndex() || (tileX - 1 == lockX && tileY == lockY))
		{
			return true;
		}
	}

	Tile* pNeighbourTop = GetTile(tileX, tileY - 1);
	if (pNeighbourTop)
	{
		if (pNeighbourTop->GetParent() == pLock->GetIndex() || (tileX == lockX && tileY - 1 == lockY))
		{
			return true;
		}
	}

	return false;
}

void WorldTileMap::AddTilesThisWouldLock(Tile* pLock, const int& power, const bool& bIgnoreAir)
{
	int range = 1;
	int locked = 0;
	std::vector<Tile*> lockedTiles;
	CL_Vec2i start = CL_Vec2i{ static_cast<int>(pLock->GetIndex() % m_width), static_cast<int>(pLock->GetIndex() / m_width) };
	RemoveAllTilesFromThisLock(pLock);

	while (locked < power) 
	{
		if (start.X - range > start.X + range || start.Y - range > start.Y + range)
		{
			break;
		}

		bool bAssigned = false;
		while (true) 
		{
			int32_t minDist = 99999, SX = -1, SY = -1;
			for (int tileX = start.X - range; tileX <= start.X + range; tileX++) 
			{
				for (int tileY = start.Y - range; tileY <= start.Y + range; tileY++) 
				{
					Tile* pTile = GetTile(tileX, tileY);
					if (!pTile)
					{
						continue;
					}

					auto it = std::find(lockedTiles.begin(), lockedTiles.end(), pTile);
					if (it != lockedTiles.end())
					{
						continue;
					}

					if (!NeighboursThisLock(pLock, pTile, bIgnoreAir))
					{
						continue;
					}

					int32_t distance = std::abs(static_cast<int>(tileY - start.Y)) + std::abs(static_cast<int>(tileX - start.X));
					if (distance >= minDist)
					{
						continue;
					}

					minDist = distance;
					SX = tileX;
					SY = tileY;
				}
			}

			if (SX == -1 && SY == -1)
			{
				break;
			}

			bAssigned = true;
			Tile* pAssignedTile = GetTile(SX, SY);
			if (!pAssignedTile) 
			{
				continue;
			}

			pAssignedTile->SetParent(pLock->GetIndex());
			pAssignedTile->ToggleFlag(TILEFLAG_LOCKED, true);
			lockedTiles.emplace_back(pAssignedTile);
			
			++locked;
			if (locked >= power)
			{
				break;
			}
		}

		if (!bAssigned)
		{
			return;
		}
		++range;
	}
}