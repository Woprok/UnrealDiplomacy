// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"

bool UUDDealActionPointChildAdd::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointChildAdd::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new sub-point for a specified Point with current SourceUniqueId.
	FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Add(actionData.SourceUniqueId);
}
void UUDDealActionPointChildAdd::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created sub-point that is saved with this action SourceUniqueId.
	FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Remove(actionData.SourceUniqueId);
}