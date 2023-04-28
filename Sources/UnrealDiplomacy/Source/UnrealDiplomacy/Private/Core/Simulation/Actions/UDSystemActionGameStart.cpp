// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionGameStart.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionGameStart::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isPreparing = world->WorldSimulationState == EUDWorldSimulationState::Preparing;
	return IUDActionInterface::CanExecute(action, world) && isPreparing;
}

void UUDSystemActionGameStart::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Enables simulation.
	world->WorldSimulationState = EUDWorldSimulationState::Simulating;
}

void UUDSystemActionGameStart::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Disables simulation.
	world->WorldSimulationState = EUDWorldSimulationState::Preparing;
}