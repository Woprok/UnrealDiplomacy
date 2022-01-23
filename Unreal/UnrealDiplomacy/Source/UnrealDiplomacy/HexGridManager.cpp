#include "HexGridManager.h"
#include "HexTile.h"

// Sets default values
AHexGridManager::AHexGridManager()
{
}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();
	
	HexGrid2DArray.SetNumZeroed(GridWidth);
	for (int32 i = 0; i < HexGrid2DArray.Num(); ++i)
	{
		HexGrid2DArray[i].SetNumZeroed(GridHeight);
	}

	for (int32 y = 0; y < GridHeight; ++y)
	{
		for (int32 x = 0; x < GridWidth; ++x)
		{
			const bool isOddRow = y % 2 == 1;
			const float xPosition = isOddRow ? x * TileHorizontalOffset + OddRowTileHorizontalOffset : x * TileHorizontalOffset;
			const float yPosition = y * TileVerticalOffset;
			TSubclassOf<AHexTile> tileToSpawn;

			if (FMath::RandRange(0.f, 1.0f) < 0.5) {
				tileToSpawn = WaterHexTile;
			}
			else
			{
				tileToSpawn = GrassHexTile;
			}

			AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(tileToSpawn, FVector(FIntPoint(xPosition, yPosition)), FRotator::ZeroRotator);
			newTile->TileIndex = FIntPoint(x, y);
			newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d-%d"), x, y));
			HexGrid2DArray[x][y] = newTile;
		}
	}
}