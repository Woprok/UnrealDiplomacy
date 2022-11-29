// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldState.h"
#include "UDWorldGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldGenerator : public UObject
{
	GENERATED_BODY()
public:
	void CreateMap(TObjectPtr<UUDMapState> emptyMapState);
protected:
	/**
	 * Replicates the map to the empty state for future use.
	 * This will not remove copy carried by this generator.
	 * TODO evaluate if keeping it has any purpose ?
	 */
	void DuplicateToState(TObjectPtr<UUDMapState> fillableMapState);
	/**
	 * Determines initial property values for all tiles created by GenerateArray.
	 */
	void UUDWorldGenerator::GenerateProperties(int32 mapSeed);
	/**
	 * Initializes new array, without any specific information about tiles,
	 * other then their location.
	 */
	void GenerateArray(int32 xSize, int32 ySize);
	/**
	 * Holds generated information about the map.
	 * Access in order of [X][Y]
	 */
	TArray<TArray<TObjectPtr<UUDTileState>>> Map;
private:
	int32 LastSeed = 0;
	int32 LastSizeOfX = 0;
	int32 LastSizeOfY = 0;
};
