// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"

void UUDDealActionParticipantInviteReject::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Request is removed from queue, without any effect being applied.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
	// Block player future participation.
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantInviteReject::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Request is returned to queue.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Enable blocked player participation.
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
}