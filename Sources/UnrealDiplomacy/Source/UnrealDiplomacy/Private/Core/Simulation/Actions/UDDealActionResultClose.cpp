// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultClose.h"

bool UUDDealActionResultClose::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionResultClose::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultClose::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDDealActionResultClose::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Closed;
}
void UUDDealActionResultClose::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDDealActionResultClose::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}