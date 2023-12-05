// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionWorldSpawnFactions.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDWorldFactionGenerator.h"

void UUDSystemActionWorldSpawnFactions::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	WorldFactionGenerator->CreateAndDuplicateFactions(world);
}

void UUDSystemActionWorldSpawnFactions::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// TODO add revert spawning of the factions...
	// one possible option is to take it from the WorldGenerator
}

void UUDSystemActionWorldSpawnFactions::SetWorldGenerator(TWeakObjectPtr<UUDWorldGenerator> worldGenerator)
{
	WorldGenerator = worldGenerator;
}

void UUDSystemActionWorldSpawnFactions::SetWorldFactionGenerator(TWeakObjectPtr<UUDWorldFactionGenerator> worldFactionGenerator)
{
	WorldFactionGenerator = worldFactionGenerator;
}