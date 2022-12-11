// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldGenerator.h"

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
	return mapState->MapSeed != LastSeed ||
		mapState->MapSizeOfX != LastSizeOfX ||
		mapState->MapSizeOfY != LastSizeOfY;
}

void UUDWorldGenerator::CreateMap(TObjectPtr<UUDMapState> emptyMapState)
{
	GenerateArray(emptyMapState->MapSizeOfX, emptyMapState->MapSizeOfY);

	GenerateProperties(emptyMapState->MapSeed);
}

void UUDWorldGenerator::GenerateProperties(int32 mapSeed)
{
	// Remember core value.
	LastSeed = mapSeed;

	// Generate specific tiles for each available empty tile.
	for (int32 x = 0; x < LastSizeOfX; x++)
	{
		for (int32 y = 0; y < LastSizeOfY; y++)
		{
			// TODO add some code for this...
			//Map[x][y]->MyPropertyTest = PropertyValueTest;
		}
	}
}

void UUDWorldGenerator::DuplicateToState(TObjectPtr<UUDMapState> fillableMapState)
{
	// Recursive duplicate on tiles, that duplicates individual tiles from current Map.
	fillableMapState->Tiles.SetNumZeroed(fillableMapState->MapSizeOfX * fillableMapState->MapSizeOfY);

	for (int32 xy = 0; xy < fillableMapState->MapSizeOfX * fillableMapState->MapSizeOfY; xy++)
	{
		fillableMapState->Tiles[xy] = UUDTileState::Duplicate(Map[xy]);
	}
}

void UUDWorldGenerator::GenerateArray(int32 xSize, int32 ySize)
{
	// Remember core values.
	LastSizeOfX = xSize;
	LastSizeOfY = ySize;

	// Generate empty array for future use.
	Map.SetNumZeroed(LastSizeOfX * LastSizeOfY);

	for (int32 x = 0; x < LastSizeOfX; x++)
	{
		for (int32 y = 0; y < LastSizeOfY; y++)
		{
			int32 xy = LastSizeOfX * x + y;
			Map[xy] = UUDTileState::CreateState(x, y);
		}
	}
}