// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"

bool UUDGameActionGiftIrrevocable::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return IUDActionInterface::CanExecute(data, world);
}

void UUDGameActionGiftIrrevocable::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Transfer resource to target.
	FUDTargetValueData data = UUDGameActionGiftIrrevocable::ConvertData(data);
	world->Players[action.InvokerPlayerId]->ResourceGold -= action.Value;
	world->Players[action.TargetId]->ResourceGold += action.Value;
}

void UUDGameActionGiftIrrevocable::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Transfer resource back from target.
	FUDTargetValueData data = UUDGameActionGiftIrrevocable::ConvertData(data);
	world->Players[action.InvokerPlayerId]->ResourceGold += action.Value;
	world->Players[action.TargetId]->ResourceGold -= action.Value;
}