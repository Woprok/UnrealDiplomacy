// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionGameEnd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionGameEnd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isSimulating = world->WorldSimulationState == EUDWorldSimulationState::Simulating;
	return IUDActionInterface::CanExecute(action, world) && isSimulating;
}

void UUDSystemActionGameEnd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Disables simulation.
	world->WorldSimulationState = EUDWorldSimulationState::Finished;
}

void UUDSystemActionGameEnd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Enables simulation.
	world->WorldSimulationState = EUDWorldSimulationState::Simulating;
}