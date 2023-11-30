// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDWorldState.h"
//#include "Kismet/GameplayStatics.h"
//#include "Core/UDGameInstance.h"

void UUDWorldGenerator::CreateAndDuplicate(TObjectPtr<UUDMapState> targetMapState)
{
	if (IsNewMapRequested(targetMapState))
	{
		UE_LOG(LogTemp, Log, TEXT("Generating new map for supplied parameters."));
		CreateMap(targetMapState);
	}
	UE_LOG(LogTemp, Log, TEXT("Duplicating map for supplied parameters."));
	DuplicateToState(targetMapState);
}

bool UUDWorldGenerator::IsNewMapRequested(TObjectPtr<UUDMapState> mapState)
{
	return mapState->MapSeed != Seed ||
		mapState->MapSizeOfX != SizeOfX ||
		mapState->MapSizeOfY != SizeOfY;
}

void UUDWorldGenerator::CreateMap(TObjectPtr<UUDMapState> emptyMapState)
{
	GenerateArray(emptyMapState->MapSizeOfX, emptyMapState->MapSizeOfY);

	GenerateProperties(emptyMapState->MapSeed);
}

int32 UUDWorldGenerator::CalculateIndex(int32 x, int32 y)
{
	return SizeOfY * x + y;
}

int32 UUDWorldGenerator::CalculateMaximumSize()
{
	return SizeOfY * SizeOfX;
}

bool InRange(int value, int32 min, int32 max)
{
	return value >= min && value <= max;
}

void UUDWorldGenerator::GenerateProperties(int32 mapSeed)
{
	// Remember core value.
	Seed = mapSeed;

	const FRandomStream& current = GetRandom();
	// Generate specific tiles for each available empty tile.
	for (int32 x = 0; x < SizeOfX; x++)
	{
		for (int32 y = 0; y < SizeOfY; y++)
		{
			int32 next = current.RandRange(1, 100);
			int32 xy = CalculateIndex(x, y);
			// TODO make this properly choose and generate all types of tiles.
			// TODO added generator that actually does something properly.
			if (InRange(next, 0, 40))
			{
				Map[xy]->Type = 69;
			}
			else if (InRange(next, 41, 70))
			{
				Map[xy]->Type = 69;
			}
			else if (InRange(next, 71, 90))
			{
				Map[xy]->Type = 42;
			}
			else if (InRange(next, 91, 100))
			{
				Map[xy]->Type = 42;
			}
			else
			{
				Map[xy]->Type = 42;
			}
		}
	}
}

void UUDWorldGenerator::DuplicateToState(TObjectPtr<UUDMapState> fillableMapState)
{
	// Recursive duplicate on tiles, that duplicates individual tiles from current Map.
	fillableMapState->Tiles.SetNumZeroed(CalculateMaximumSize());

	for (int32 xy = 0; xy < CalculateMaximumSize(); xy++)
	{
		fillableMapState->Tiles[xy] = UUDTileState::Duplicate(Map[xy]);
	}
}

void UUDWorldGenerator::GenerateArray(int32 xSize, int32 ySize)
{
	// Remember core values.
	SizeOfX = xSize;
	SizeOfY = ySize;

	// Generate empty array for future use.
	Map.SetNumZeroed(CalculateMaximumSize());

	for (int32 x = 0; x < SizeOfX; x++)
	{
		for (int32 y = 0; y < SizeOfY; y++)
		{
			int32 xy = CalculateIndex(x, y);
			Map[xy] = UUDTileState::CreateState(x, y);
		}
	}
}

const FRandomStream& UUDWorldGenerator::GetRandom()
{
	if (Random.GetInitialSeed() != Seed)
	{
		Random = FRandomStream(Seed);
	}
	return Random;
}