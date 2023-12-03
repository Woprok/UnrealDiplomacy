// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"

void UUDSystemActionWorldCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Create new state and call generator to fill it with new world action.
	world->Map = UUDMapState::CreateState(world->Settings.MapSeed, world->Settings.MapWidth, world->Settings.MapHeight);
	// Generated data are dumped into the provided state by the world generator automatically.
	WorldGenerator->CreateAndDuplicate(world->Map);
}

void UUDSystemActionWorldCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Destroy map using GC.
	world->Map = nullptr;
}

void UUDSystemActionWorldCreate::SetWorldGenerator(TWeakObjectPtr<UUDWorldGenerator> worldGenerator)
{
	WorldGenerator = worldGenerator;
}