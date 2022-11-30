// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldState.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UDGameInstance.h"
#include "UDWorldGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldGenerator : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Create or retrieve existing map from the map generator.
	 * Clients will generate once and use it, while server will reuse it multiple times.
	 * Provided parameter is modified!
	 */
	void CreateAndDuplicate(TObjectPtr<UUDMapState> targetMapState);
protected:
	/**
	 * Creates map with specified parameters as defined in UUDMapState object.
	 */
	void CreateMap(TObjectPtr<UUDMapState> emptyMapState);
	/**
	 * Replicates the map to the empty state for future use.
	 * This will not remove copy carried by this generator.
	 * TODO evaluate if keeping it has any purpose ?
	 */
	void DuplicateToState(TObjectPtr<UUDMapState> fillableMapState);
	/**
	 * Determines initial property values for all tiles created by GenerateArray.
	 */
	void GenerateProperties(int32 mapSeed);
	/**
	 * Initializes new array, without any specific information about tiles,
	 * other then their location.
	 */
	void GenerateArray(int32 xSize, int32 ySize);
	/**
	 * Holds generated information about the last generated map.
	 * Access in order of [X][Y]
	 */
	TArray<TArray<TObjectPtr<UUDTileState>>> Map;
	/**
	 * Checks if last map is same as the new request.
	 */
	bool IsNewMapRequested(TObjectPtr<UUDMapState> mapState);
private:
	int32 LastSeed = 0;
	int32 LastSizeOfX = 0;
	int32 LastSizeOfY = 0;
};
