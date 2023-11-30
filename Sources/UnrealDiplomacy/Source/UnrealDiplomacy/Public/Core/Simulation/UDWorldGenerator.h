// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldGenerator.generated.h"

// Forward Declarations

class UUDMapState;
class UUDTileState;
class UUDResourceManager;

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
	virtual void SetResourceManager(TObjectPtr<UUDResourceManager> resourceManager);
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
	 * Checks if last map is same as the new request.
	 */
	bool IsNewMapRequested(TObjectPtr<UUDMapState> mapState);
	/**
	 * Holds generated information about the last generated map.
	 * Access in order of [X][Y]
	 */
	UPROPERTY()
	TArray<TObjectPtr<UUDTileState>> Map;
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
private:
	/**
	 * Retrieves generator for current seed, if seed changed or was not set creates new.
	 */
	const FRandomStream& GetRandom();
	/** 
	 * Helper function to calculate xy index based on x and y coordinates.
	 */
	int32 CalculateIndex(int32 x, int32 y);
	/**
	 * Helper function to calculate xy maximum based on x and y sizes.
	 */
	int32 CalculateMaximumSize();
	UPROPERTY()
	FRandomStream Random;
	UPROPERTY()
	int32 Seed = 0;
	UPROPERTY()
	int32 SizeOfX = 0;
	UPROPERTY()
	int32 SizeOfY = 0;
};