// Copyright Miroslav Valach


#include "Core/Map/AUDHexTileGridManager.h"
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

void AUDHexTileGridManager::Generate()
{	
	SetupPositionConstantValues();
	SetupMapSize();
	GenerateMap();
}

int32 AUDHexTileGridManager::GetTilesPerPlayer()
{
	// 3 n^2 - 3 n + 1 so we get proper sequence.
	const int32 eRad = PlayerExclusionRadius + 1;
	return 3 * eRad * eRad - 3 * eRad + 1;
}

void AUDHexTileGridManager::AdjustGridSize()
{
	
	
	// Extend by BorderTileThickness, so there is enough space for border.
	GridWidth += BorderTileThickness;
	GridHeight += BorderTileThickness;
	
}

void AUDHexTileGridManager::GenerateBorder()
{
	if (BorderTileThickness >= 1)
	{
		// first i rows
		for (int i = 0; i < BorderTileThickness; ++i)
		{
			for (int32 x = 0; x < GridWidth; ++x)
			{
				// Place Border at [x][i]
			}
		}
		// last i rows
		for (int i = GridHeight - BorderTileThickness; i < GridHeight; ++i)
		{
			for (int32 x = 0; x < GridWidth; ++x)
			{
				// Place Border at [x][i]
			}
		}

		// first j columns
		for (int j = 0; j < BorderTileThickness; ++j)
		{
			for (int32 y = 0; y < GridHeight; ++y)
			{
				// Place Border at [j][y]
			}
		}
		// last j columns
		for (int j = GridWidth - BorderTileThickness; j < GridWidth; ++j)
		{
			for (int32 x = 0; x < GridWidth; ++x)
			{
				// Place Border at [j][y]
			}
		}
	}
}

void AUDHexTileGridManager::GenerateMapInSteps()
{
	// Initialize
	
	SetupPositionConstantValues();
	// Adjust grid size before generation.
	AdjustGridSize();
	// Adjust internal array to proper dimensions.
	SetupMapSize();
	// Create border part.
	GenerateBorder();
	//

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

void AUDHexTileGridManager::SetupPositionConstantValues()
{
	auto extendedRadius = TileRadius + 4;
	TileHorizontalOffset = sqrt(3) * extendedRadius;
	OddRowTileHorizontalOffset = TileHorizontalOffset / 2;
	TileVerticalOffset = (6 * extendedRadius) / 4;
}
void AUDHexTileGridManager::SetupMapSize()
{
	HexGrid2DArray.SetNumZeroed(GridWidth);
	for (int32 i = 0; i < HexGrid2DArray.Num(); ++i)
	{
		HexGrid2DArray[i].SetNumZeroed(GridHeight);
	}
}
void AUDHexTileGridManager::GenerateMap()
{
	FRandomStream Stream(MapSeed);
	
	for (int32 y = 0; y < GridHeight; ++y)
	{
		for (int32 x = 0; x < GridWidth; ++x)
		{
			const bool isOddRow = y % 2 == 1;
			const float xPosition = isOddRow ? x * TileHorizontalOffset + OddRowTileHorizontalOffset : x * TileHorizontalOffset;
			const float yPosition = y * TileVerticalOffset;
			TSubclassOf<AUDHexTile> tileToSpawn;

			if (Stream.FRandRange(0.f, 1.0f) < 0.5f) {
				tileToSpawn = WaterHexTile;
			}
			else
			{
				tileToSpawn = GrassHexTile;
			}

			TObjectPtr<AUDHexTile> newTile = GetWorld()->SpawnActor<AUDHexTile>(tileToSpawn, FVector(FIntPoint(xPosition, yPosition)), FRotator::ZeroRotator);
			newTile->TileIndex = FIntPoint(x, y);
			newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d-%d"), x, y));
			HexGrid2DArray[x][y] = newTile;
		}
	}
}

