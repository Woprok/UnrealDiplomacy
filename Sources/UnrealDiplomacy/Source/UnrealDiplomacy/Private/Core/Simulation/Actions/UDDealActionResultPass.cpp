// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultPass.h"

bool UUDDealActionResultPass::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionResultPass::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultPass::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionResultPass::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}
void UUDDealActionResultPass::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionResultPass::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}