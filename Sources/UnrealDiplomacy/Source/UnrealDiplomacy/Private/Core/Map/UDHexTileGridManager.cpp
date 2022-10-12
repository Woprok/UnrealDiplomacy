// Copyright Miroslav Valach


#include "Core/Map/UDHexTileGridManager.h"
#include "Core/Map/UDHexTile.h"

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
	FullGridWidth = UseableGridWidth + 2 * BorderTileThickness;
	FullGridHeight = UseableGridHeight + 2 * BorderTileThickness;
}

void AUDHexTileGridManager::InitializeGridArray()
{
	// ROWS
	HexGrid2DArray.SetNumZeroed(FullGridHeight);
	for (int32 i = 0; i < HexGrid2DArray.Num(); ++i)
	{
		// COLUMNS
		HexGrid2DArray[i].SetNumZeroed(FullGridWidth);
	}
}

FIntPoint AUDHexTileGridManager::DetermineTilePositionInWorld(const int32 xRow, const int32 yCol)
{
	const float xPosition = xRow * TileVerticalOffset;
	const bool isOddRow = xRow % 2 == 1;
	const float yPosition = isOddRow ? yCol * TileHorizontalOffsetEven + TileHorizontalOffsetOdd : yCol * TileHorizontalOffsetEven;
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
		//********
		//|      |
		//|      |
		//|      |
		//--------
		// First i rows
		for (int i = 0; i < BorderTileThickness; ++i)
		{
			for (int32 y = 0; y < FullGridWidth; ++y)
			{
				// Place Border at [i_row][y_column]
				SpawnBorderTile(i, y);
			}
		}
		//--------
		//|      |
		//|      |
		//|      |
		//********
		// Last i rows
		for (int i = FullGridHeight - BorderTileThickness; i < FullGridHeight; ++i)
		{
			for (int32 y = 0; y < FullGridWidth; ++y)
			{
				// Place Border at [i_row][y_column]
				SpawnBorderTile(i, y);
			}
		}
		//--------
		//*      |
		//*      |
		//*      |
		//--------
		// First j columns
		for (int j = 0; j < BorderTileThickness; ++j)
		{
			for (int32 x = BorderTileThickness; x < FullGridHeight - BorderTileThickness; ++x)
			{
				// Place Border at [x][j_columns]
				SpawnBorderTile(x, j);
			}
		}
		//--------
		//|      *
		//|      *
		//|      *
		//--------
		// Last j columns
		for (int j = FullGridWidth - BorderTileThickness; j < FullGridWidth; ++j)
		{
			for (int32 x = BorderTileThickness; x < FullGridHeight - BorderTileThickness; ++x)
			{
				// Place Border at [x][j_columns]
				SpawnBorderTile(x, j);
			}
		}
	}
}

void AUDHexTileGridManager::GenerateFromTileData(TArray<FUDTileRow>& tilemap)
{
	check(tilemap.Num() == UseableGridHeight)
	// ROWS
	for (int i = 0; i < tilemap.Num(); ++i)
	{
		check(tilemap[i].Tiles.Num() == UseableGridWidth)
		// COLUMNS
		for (int j = 0; j < tilemap[i].Tiles.Num(); ++j)
		{
			// World position must include border.
			int32 xWorld = i + BorderTileThickness;
			int32 yWorld = j + BorderTileThickness;
			tilemap[i].Tiles[j].WorldPosition = FIntPoint(xWorld, yWorld);
			// Data postion is same as the receiving map position.
			SpawnTile(xWorld, yWorld, MapWorldHexTile, i, j);
			HexGrid2DArray[xWorld][yWorld]->OnVisualUpdate(tilemap[i].Tiles[j]);
		}
	}
}