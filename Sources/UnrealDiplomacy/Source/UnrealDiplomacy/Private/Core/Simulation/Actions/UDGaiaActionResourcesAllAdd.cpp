// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"

bool UUDGaiaActionResourcesAllAdd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	return IUDActionInterface::CanExecute(actionData, targetWorldState) &&
		// Invoker must be server
		actionData.InvokerPlayerId == UUDWorldState::GaiaWorldStateId;
}

void UUDGaiaActionResourcesAllAdd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Grant all players defined amount of gold.
	FUDValueData data = UUDGaiaActionResourcesAllAdd::ConvertData(actionData);
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold += data.Value;
	}
}

void UUDGaiaActionResourcesAllAdd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Take defined amount of gold from all players.
	FUDValueData data = UUDGaiaActionResourcesAllAdd::ConvertData(actionData);
	for (auto& state : targetWorldState->Players)
	{
		state.Value->ResourceGold -= data.Value;
	}
}
