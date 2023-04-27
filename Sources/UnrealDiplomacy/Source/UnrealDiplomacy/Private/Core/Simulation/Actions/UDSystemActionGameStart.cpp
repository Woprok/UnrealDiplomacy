// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionGameStart.h"

bool UUDSystemActionGameStart::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// State is checked.
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::INITIALIZING;
}

void UUDSystemActionGameStart::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// State is updated.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
}

void UUDSystemActionGameStart::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// State is returned to previous value.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::INITIALIZING;
}