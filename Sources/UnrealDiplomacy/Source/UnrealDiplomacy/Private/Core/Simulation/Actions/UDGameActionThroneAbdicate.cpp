// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"

bool UDGameActionThroneAbdicate::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be current throne owner. No other conditions are requried for abdication.
		targetWorldState->ImperialThrone.Ruler == actionData.InvokerPlayerId;
}

void UDGameActionThroneAbdicate::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Player is removed from the throne..
	targetWorldState->ImperialThrone.Ruler = UUDWorldState::GaiaWorldStateId;
}

void UDGameActionThroneAbdicate::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Player is returned to throne.
	targetWorldState->ImperialThrone.Ruler = actionData.InvokerPlayerId;
}