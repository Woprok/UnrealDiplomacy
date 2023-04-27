// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionStateExtendingDraft.h"

bool UUDDealActionStateExtendingDraft::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Assembling;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionStateExtendingDraft::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}
void UUDDealActionStateExtendingDraft::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}