// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointAdd.h"

bool UUDDealActionPointAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionPointAdd::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDealActionPointAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Creates new point.
	FUDDealData data = UUDDealActionPointAdd::ConvertData(data);
	world->Deals[action.DealId]->Points.Add(action.SourceUniqueId, UUDDiscussionItem::CreateState(action.InvokerPlayerId));
	world->Deals[action.DealId]->PrimaryPoints.Add(action.SourceUniqueId);
}
void UUDDealActionPointAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Deletes created point that is currently located at the end.
	FUDDealData data = UUDDealActionPointAdd::ConvertData(data);
	world->Deals[action.DealId]->Points.Remove(action.SourceUniqueId);
	world->Deals[action.DealId]->PrimaryPoints.Remove(action.SourceUniqueId);
}