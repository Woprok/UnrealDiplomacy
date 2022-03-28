// Copyright Miroslav Valach


#include "Core/Map/AUDHexTileGridManager.h"
#include "Core/Map/AUDHexTile.h"

// Sets default values
AUDHexTileGridManager::AUDHexTileGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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


// Called every frame
void AUDHexTileGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

