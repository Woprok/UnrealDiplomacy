// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantLeave::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	bool isLeaver = world->Deals[data.DealId]->Participants.Contains(action.InvokerPlayerId);
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen && isLeaver;
}

void UUDDealActionParticipantLeave::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Remove invoker from participants.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->Participants.Remove(action.InvokerPlayerId);
	world->Deals[data.DealId]->BlockedParticipants.Add(action.InvokerPlayerId);
}

void UUDDealActionParticipantLeave::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Add invoker back to participants.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Remove(action.InvokerPlayerId);
	world->Deals[data.DealId]->Participants.Add(action.InvokerPlayerId);
}
