// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldFactionGenerator.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"

void UUDWorldFactionGenerator::CreateAndDuplicateFactions(TObjectPtr<UUDWorldState> targetWorldState)
{
	if (IsNewFactionSpawnRequested(targetWorldState->Map))
	{
		UE_LOG(LogTemp, Log, TEXT("Generating new faction map for supplied parameters."));
		CreateFactionMap(targetWorldState);
	}
	UE_LOG(LogTemp, Log, TEXT("Applying faction map."));
	AssignFactionSpawns(targetWorldState);
}

bool UUDWorldFactionGenerator::IsNewFactionSpawnRequested(TObjectPtr<UUDMapState> mapState)
{
	return mapState->MapSeed != Seed ||
		mapState->RequiredFactionSpace != FactionCount;
}

void UUDWorldFactionGenerator::CreateFactionMap(TObjectPtr<UUDWorldState> worldState)
{
	// Remember core value.
	Seed = worldState->Map->MapSeed;
	// Store the count...
	FactionCount = worldState->Map->RequiredFactionSpace;
	// Store collections...
	ConstructFactionSpawnsSet(worldState);
	ConstructFactionSpawnList(worldState);
}

void UUDWorldFactionGenerator::ConstructFactionSpawnsSet(TObjectPtr<UUDWorldState> worldState)
{
	// We can safely assume that tileCount > factionCount (otherwise someone removed hack without making proper update to lobby setup)
	const int32 factionCount = worldState->Factions.Num();
	const int32 tileCount = worldState->Map->Tiles.Num();

	const FRandomStream& current = GetRandom();
	TileIndices.Empty(factionCount);
	const int32 firstTileIndex = 0;
	const int32 lastTileIndex = tileCount - 1;
	// Naive way of doing random selection of tiles.
	while (TileIndices.Num() < factionCount)
	{
		TileIndices.Add(current.RandRange(firstTileIndex, lastTileIndex));
	}
}

void UUDWorldFactionGenerator::ConstructFactionSpawnList(TObjectPtr<UUDWorldState> worldState)
{
	// Create list of all factions that need tile. (we should have more as the generate faction spawns uses also nonplayable factions)
	FactionSpawnList.Empty(0);
	for (const auto& faction : worldState->Factions)
	{
		if (faction.Value->Controller == EUDFactionController::Player || faction.Value->Controller == EUDFactionController::AI)
		{
			FactionSpawnList.Add(faction.Key);
		}
	}
}

void AssignTileToFaction(TObjectPtr<UUDTileState> tile, const FUDResourcePresentation& resource, int32 factionId)
{
	tile->OwnerUniqueId = factionId;
	// TODO this is hack that is also done in WorldGenerator as type is same as resource type in this version.
	tile->Type = resource.ResourceId;
	tile->ResourceType = resource.ResourceId;
	tile->ResourceStockpile = resource.TileStartingAmount;
}

void UUDWorldFactionGenerator::AssignFactionSpawns(TObjectPtr<UUDWorldState> worldState)
{
	FUDResourcePresentation reputationResource = ResourceManager->GetSpecified(UUDGameResourceReputation::ResourceId);
	int32 arrayIndex = 0;
	for (const auto& tileIndex : TileIndices)
	{
		if (arrayIndex >= FactionSpawnList.Num())
		{
			// Ends this part.
			break;
		}
		AssignTileToFaction(worldState->Map->Tiles[tileIndex], reputationResource, FactionSpawnList[arrayIndex]);
		arrayIndex++;
	}
}

const FRandomStream& UUDWorldFactionGenerator::GetRandom()
{
	if (Random.GetInitialSeed() != Seed)
	{
		Random = FRandomStream(Seed);
	}
	return Random;
}

void UUDWorldFactionGenerator::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}