// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"

void UUDSystemActionWorldCreate::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Create new state and call generator to fill it with new world data.
	FUDMapSettingsData data = UUDSystemActionWorldCreate::ConvertData(actionData);
	targetWorldState->Map = UUDMapState::CreateState(data.Seed, data.SizeX, data.SizeY);
	// Generated data are dumped into the provided state by the world generator automatically.
	WorldGenerator->CreateAndDuplicate(targetWorldState->Map);
}

void UUDSystemActionWorldCreate::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Destroy map using GC.
	targetWorldState->Map = nullptr;
}