// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldFactionGenerator.generated.h"

// Forward Declarations

class UUDWorldState;
class UUDMapState;
class UUDTileState;
class UUDResourceManager;

/**
 * Generates factions over the initial world, if possible.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldFactionGenerator : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Create or retrieve existing map from the map generator.
	 * Clients will generate once and use it, while server will reuse it multiple times.
	 * Provided parameter is modified!
	 * Factions will be add on top of the map, overriding previous states.
	 */
	void CreateAndDuplicateFactions(TObjectPtr<UUDWorldState> targetWorldState);
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager);
protected:
	/** Generates set of available spawns. */
	void ConstructFactionSpawnsSet(TObjectPtr<UUDWorldState> worldState);
	/** Constructs faction spawn list, that will be used for spawning. */
	void ConstructFactionSpawnList(TObjectPtr<UUDWorldState> worldState);
	/** Assigns in order tile to each faction controlled by player. */
	void AssignFactionSpawns(TObjectPtr<UUDWorldState> worldState);
	/** Creates faction map data, that will be used for assigning. */
	void CreateFactionMap(TObjectPtr<UUDWorldState> worldState);
	/** Checks if faction count is same as well as if last map is same as the new request. */
	bool IsNewFactionSpawnRequested(TObjectPtr<UUDMapState> mapState);
	/** Holds unique Ids of tiles from the world map. */
	UPROPERTY()
	TSet<int32> TileIndices = { };
	/** Holds Ids of faction that will be assigned spawn. */
	UPROPERTY()
	TArray<int32> FactionSpawnList = { };
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
private:
	/** Retrieves generator for current seed, if seed changed or was not set creates new. */
	const FRandomStream& GetRandom();
	UPROPERTY()
	FRandomStream Random;
	UPROPERTY()
	int32 Seed = 0;
	UPROPERTY()
	int32 FactionCount = 0;
};