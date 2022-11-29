// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldGenerator.h"

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
	fillableMapState->Tiles.SetNumZeroed(fillableMapState->MapSizeOfX);

	for (int32 x = 0; x < fillableMapState->MapSizeOfX; x++)
	{
		fillableMapState->Tiles[x].SetNumZeroed(fillableMapState->MapSizeOfY);
	}

	for (int32 x = 0; x < fillableMapState->MapSizeOfX; x++)
	{
		for (int32 y = 0; y < fillableMapState->MapSizeOfY; y++)
		{
			fillableMapState->Tiles[x][y] = UUDTileState::Duplicate(Map[x][y]);
		}
	}
}

void UUDWorldGenerator::GenerateArray(int32 xSize, int32 ySize)
{
	// Remember core values.
	LastSizeOfX = xSize;
	LastSizeOfY = ySize;

	// Generate empty array for future use.
	Map.SetNumZeroed(xSize);

	for (int32 x = 0; x < xSize; x++)
	{
		Map[x].SetNumZeroed(ySize);
	}

	for (int32 x = 0; x < xSize; x++)
	{
		for (int32 y = 0; y < ySize; y++)
		{
			Map[x][y] = UUDTileState::CreateState(x, y);
		}
	}
}
