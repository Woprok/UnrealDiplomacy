// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointChildAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPoint data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen;
}

void UUDDealActionPointChildAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Creates new sub-point for a specified Point with current SourceUniqueId.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points.Add(action.SourceUniqueId, UUDDiscussionItem::CreateState(action.InvokerPlayerId));
	world->Deals[data.DealId]->Points[data.PointId]->Consequencies.Add(action.SourceUniqueId);
}

void UUDDealActionPointChildAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Deletes created sub-point that is saved with this action SourceUniqueId.
	FUDDealDataPoint data(action.ValueParameters);
	world->Deals[data.DealId]->Points.Remove(action.SourceUniqueId);
	world->Deals[data.DealId]->Points[data.PointId]->Consequencies.Remove(action.SourceUniqueId);
}