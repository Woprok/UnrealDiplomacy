// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultVeto.h"

bool UUDDealActionResultVeto::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionResultVeto::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultVeto::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDDealActionResultVeto::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
}
void UUDDealActionResultVeto::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDDealActionResultVeto::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}