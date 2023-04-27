// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointAdd.h"

bool UUDDealActionPointAdd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionPointAdd::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointAdd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new point.
	FUDDealData data = UUDDealActionPointAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Add(actionData.SourceUniqueId);
}
void UUDDealActionPointAdd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created point that is currently located at the end.
	FUDDealData data = UUDDealActionPointAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Remove(actionData.SourceUniqueId);
}