// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"

bool UUDGameActionGiftIrrevocable::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState);
}

void UUDGameActionGiftIrrevocable::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Transfer resource to target.
	FUDTargetValueData data = UUDGameActionGiftIrrevocable::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold -= data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold += data.Value;
}

void UUDGameActionGiftIrrevocable::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Transfer resource back from target.
	FUDTargetValueData data = UUDGameActionGiftIrrevocable::ConvertData(actionData);
	targetWorldState->Players[actionData.InvokerPlayerId]->ResourceGold += data.Value;
	targetWorldState->Players[data.TargetId]->ResourceGold -= data.Value;
}