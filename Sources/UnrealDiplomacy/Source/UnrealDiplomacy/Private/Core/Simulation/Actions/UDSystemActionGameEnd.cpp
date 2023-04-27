// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionGameEnd.h"

bool UUDSystemActionGameEnd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// State is checked.
		targetWorldState->WorldSimulationState == EUDWorldSimulationState::PLAYING;
}

void UUDSystemActionGameEnd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// State is updated.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::FINISHING;
}

void UUDSystemActionGameEnd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// State is returned to previous value.
	targetWorldState->WorldSimulationState = EUDWorldSimulationState::PLAYING;
}