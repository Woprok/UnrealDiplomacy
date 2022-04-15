// Copyright Miroslav Valach


#include "Core/Map/AUDHexTileGridManager.h"
#include "Core/Data/FUDTypes.h"
#include "Core/Map/AUDHexTile.h"

// Sets default values
AUDHexTileGridManager::AUDHexTileGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called every frame
void AUDHexTileGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AUDHexTileGridManager::BeginPlay()
{
	Super::BeginPlay();
}

// ----------------------------------------------------------------------------------
// Preparation methods for map gen.
// ----------------------------------------------------------------------------------

void AUDHexTileGridManager::PrepareMapParameters()
{
	DefineTileOffsets();

	DefinePlayerTileCounts();
	
	DefineGridSize();

	DefineFullGridSize();

	InitializeGridArray();
}

void AUDHexTileGridManager::DefineTileOffsets()
{
	TileHorizontalOffsetEven = sqrt(3) * TileExtendedRadius;
	TileHorizontalOffsetOdd = TileHorizontalOffsetEven / 2;
	TileVerticalOffset = (6 * TileExtendedRadius) / 4;
}

void AUDHexTileGridManager::DefinePlayerTileCounts()
{
	// 3 n^2 - 3 n + 1 should be how we get proper sequence for hexes.
	auto CalculateTileCount = [](int32 n)
	{
		return static_cast<int32>(3 * n * n - 3 * n + 1);
	};
	
	PlayerExclusionTiles = CalculateTileCount(1 + PlayerExclusionRadius);
	PlayerBufferTiles = CalculateTileCount(1 + PlayerExclusionRadius + PlayerBufferRadius);
}

void AUDHexTileGridManager::DefineGridSize()
{
	int32 totalRequired = PlayerCount * PlayerBufferTiles;
	UseableGridHeight = MinimumGridHeight;
	UseableGridWidth = MinimumGridWidth;
	int32 baseProvided = UseableGridHeight * UseableGridWidth;
	while (totalRequired > baseProvided)
	{
		UseableGridHeight++;
		UseableGridWidth++;
		baseProvided = UseableGridHeight * UseableGridWidth;
	}
}

void AUDHexTileGridManager::DefineFullGridSize()
{
	UseableGridWidth += 2 * BorderTileThickness;
	UseableGridHeight += 2 * BorderTileThickness;
}

void AUDHexTileGridManager::InitializeGridArray()
{
	// ROWS
	HexGrid2DArray.SetNumZeroed(UseableGridHeight);
	for (int32 i = 0; i < HexGrid2DArray.Num(); ++i)
	{
		// COLUMNS
		HexGrid2DArray[i].SetNumZeroed(UseableGridWidth);
	}
}

FIntPoint AUDHexTileGridManager::DetermineTilePositionInWorld(const int32 x, const int32 y)
{
	const bool isOddRow = y % 2 == 1;
	const float xPosition = isOddRow ? x * TileHorizontalOffsetEven + TileHorizontalOffsetOdd : x * TileHorizontalOffsetEven;
	const float yPosition = y * TileVerticalOffset;
	return FIntPoint(xPosition, yPosition);
}

void AUDHexTileGridManager::SpawnTile(const int32 xWorld, const int32 yWorld, TSubclassOf<AUDHexTile> tileType, int32 xData, int32 yData)
{
	FVector worldPosition = FVector(DetermineTilePositionInWorld(xWorld, yWorld), 0);
	// Create at position
	TObjectPtr<AUDHexTile> newTile = GetWorld()->SpawnActor<AUDHexTile>(tileType, worldPosition, FRotator::ZeroRotator);
	newTile->GridIndices = FIntPoint(xWorld, yWorld);
	newTile->DataIndices = FIntPoint(xData, yData);
	HexGrid2DArray[xWorld][yWorld] = newTile;
}

void AUDHexTileGridManager::SpawnBorderTile(const int32 x, const int32 y)
{
	SpawnTile(x, y, MapBorderHexTile, -1, -1);
}

// ----------------------------------------------------------------------------------
// Generation methods for map gen.
// ----------------------------------------------------------------------------------

void AUDHexTileGridManager::GenerateMapBorder()
{
	if (BorderTileThickness >= 1)
	{
		// First i rows
		for (int i = 0; i < BorderTileThickness; ++i)
		{
			for (int32 x = 0; x < FullGridWidth; ++x)
			{
				// Place Border at [x][i]
				SpawnBorderTile(x, i);
			}
		}
		// Last i rows
		for (int i = FullGridHeight - BorderTileThickness; i < FullGridHeight; ++i)
		{
			for (int32 x = 0; x < FullGridWidth; ++x)
			{
				// Place Border at [x][i]
				SpawnBorderTile(x, i);
			}
		}

		// First j columns
		for (int j = 0; j < BorderTileThickness; ++j)
		{
			// Start after first few rows and End before last few rows
			for (int32 y = BorderTileThickness; y < FullGridHeight - BorderTileThickness; ++y)
			{
				// Place Border at [j][y]
				SpawnBorderTile(j, y);
			}
		}
		// Last j columns
		for (int j = FullGridWidth - BorderTileThickness; j < FullGridWidth; ++j)
		{
			// Start after first few rows and End before last few rows
			for (int32 y = BorderTileThickness; y < FullGridHeight - BorderTileThickness; ++y)
			{
				// Place Border at [j][y]
				SpawnBorderTile(j, y);
			}
		}
	}
}

//void AUDHexTileGridManager::GenerateFromTileData(TArray<FUDTileRow>& map)
//{
//	check(map.Num() == UseableGridHeight)
//	// ROWS
//	for (int i = 0; i < map.Num(); ++i)
//	{
//		check(map[i].Tiles.Num() == UseableGridWidth)
//		// COLUMNS
//		for (int j = 0; j < map[i].Tiles.Num(); ++j)
//		{
//			// World position must include border.
//			int32 xWorld = i + BorderTileThickness;
//			int32 yWorld = j + BorderTileThickness;
//			map[i].Tiles[j].WorldPosition = FIntPoint(xWorld, yWorld);
//			// Data postion is same as the receiving map position.
//			SpawnTile(xWorld, yWorld, MapWorldHexTile, i, j);
//			HexGrid2DArray[xWorld][yWorld]->VisualUpdate(map[i].Tiles[j]);
//		}
//	}
//}


void TestGen()
{
	//FRandomStream Stream(MapSeed);
	//if (Stream.FRandRange(0.f, 1.0f) < 0.5f) {
	//	tileToSpawn = WaterHexTile;
	//}

	// Obtain generated tilemap
	
	// precondition:: maintaned array of all tiles
	// random: any index in current array returns only non used tile
	// postcondition: buffer array of all unused tiles
	// for each player
		// Get random point in map
		// Select it as player spawn
		// Remove all adjacent tiles based on exclusion radius
		// Remove all adjacent tiles based on buffer radius
		// next random point is always valid
	// generate all of remaining buffer returned as postcondition
	// these are tiles that were not used by other players
	// merge results from adjecent tiles if possible	
}