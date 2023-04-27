// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionStateAssemble.h"

bool UUDDealActionStateAssemble::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionStateAssemble::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::CreatingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionStateAssemble::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionStateAssemble::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}
void UUDDealActionStateAssemble::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionStateAssemble::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::CreatingDraft;
}
