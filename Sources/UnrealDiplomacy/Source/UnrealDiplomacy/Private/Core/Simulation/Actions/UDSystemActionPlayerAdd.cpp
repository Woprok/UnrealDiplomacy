// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"

void UUDSystemActionPlayerAdd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Player is added to the state.
	targetWorldState->PlayerOrder.Add(actionData.InvokerPlayerId);
	targetWorldState->Players.Add(actionData.InvokerPlayerId, UUDNationState::CreateState(actionData.InvokerPlayerId));
}

void UUDSystemActionPlayerAdd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Player is removed from the state.
	targetWorldState->PlayerOrder.Remove(actionData.InvokerPlayerId);
	targetWorldState->Players.Remove(actionData.InvokerPlayerId);
}