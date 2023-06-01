// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointAdd::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen;
}

void UUDDealActionPointAdd::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Creates new point with current SourceUniqueId.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->Points.Add(action.SourceUniqueId, UUDDiscussionItem::CreateState(action.InvokerFactionId));
	world->Deals[data.DealId]->PrimaryPoints.Add(action.SourceUniqueId);
}

void UUDDealActionPointAdd::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Deletes created point that is saved with this action SourceUniqueId.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->Points.Remove(action.SourceUniqueId);
	world->Deals[data.DealId]->PrimaryPoints.Remove(action.SourceUniqueId);
}