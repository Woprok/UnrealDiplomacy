// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantInviteReject::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
	return IUDActionInterface::CanExecute(action, world) && isNotMember && isQueued;
}

void UUDDealActionParticipantInviteReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Request is removed from queue, without any effect being applied.
	FUDDealDataTarget data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
	// Block player future participation.
	world->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantInviteReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Request is returned to queue.
	FUDDealDataTarget data(action.ValueParameters);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(action, UUDDealActionParticipantInvite::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, world);
	// Enable blocked player participation.
	world->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
}