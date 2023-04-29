// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"

bool UUDDealActionPointChildAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointChildAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Creates new sub-point for a specified Point with current SourceUniqueId.
	FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(data);
	world->Deals[action.DealId]->Points.Add(action.SourceUniqueId, UUDDiscussionItem::CreateState(action.InvokerPlayerId));
	world->Deals[action.DealId]->Points[action.Point]->Consequencies.Add(action.SourceUniqueId);
}
void UUDDealActionPointChildAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Deletes created sub-point that is saved with this action SourceUniqueId.
	FUDDealPointData data = UUDDealActionPointChildAdd::ConvertData(data);
	world->Deals[action.DealId]->Points.Remove(action.SourceUniqueId);
	world->Deals[action.DealId]->Points[action.Point]->Consequencies.Remove(action.SourceUniqueId);
}