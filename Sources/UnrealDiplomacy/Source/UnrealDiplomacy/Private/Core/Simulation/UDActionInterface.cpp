// Copyright Miroslav Valach


#include "Core/Simulation/UDActionInterface.h"

bool IUDActionInterface::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return actionData.ActionTypeId == GetActionTypeId();
}

void IUDActionInterface::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
}

void IUDActionInterface::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
}

int32 IUDActionInterface::GetActionTypeId()
{
	return -1;
}

#pragma region UUDLogAction

void UUDLogAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("UUDLogAction was invoked by FUDActionData with id(%d), so it was logged due to UUDLogAction is id(%d)."),
		actionData.ActionTypeId, ActionTypeId);
}

void UUDLogAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// LogAction is not revertable as there is no state change.
}

int32 UUDLogAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion

#pragma region UUDAddPlayerAction

void UUDAddPlayerAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("AddPlayer was invoked by new playerd with id(%d)."),
		actionData.InvokerPlayerId);
	targetWorldState->PlayerOrder.Add(actionData.InvokerPlayerId);
}

void UUDAddPlayerAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log,
		TEXT("AddPlayer was reverted by removing playerd with id(%d)."),
		actionData.InvokerPlayerId);
	targetWorldState->PlayerOrder.Remove(actionData.InvokerPlayerId);
}

int32 UUDEndTurnAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion

#pragma region UUDEndTurnAction

void UUDEndTurnAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(LogTemp, Log, 
		TEXT("EndTurn was invoked by playerd with id(%d)."),
		actionData.InvokerPlayerId);
	// TODO proper pass once players are saved
	targetWorldState->CurrentTurnPlayerId = actionData.InvokerPlayerId + 1;
	targetWorldState->CurrentTurn += 1;
}

void UUDEndTurnAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// TODO proper pass once players are saved
	targetWorldState->CurrentTurnPlayerId = actionData.InvokerPlayerId - 1;
	targetWorldState->CurrentTurn -= 1;
}

int32 UUDEndTurnAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion