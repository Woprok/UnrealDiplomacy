// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantKick::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerPlayerId;
	bool isKickedPresent = world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen && isModerator && isKickedPresent;
}

void UUDDealActionParticipantKick::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Remove target from participants.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Participants.Remove(data.TargetId);
	world->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantKick::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Add target back to participants.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
	world->Deals[data.DealId]->Participants.Add(data.TargetId);
}