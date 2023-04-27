// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"

void UUDDealActionParticipantInviteAccept::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Execute change based on data contained in confirm.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Add(data.TargetId);
	// Remove request from queue.
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDDealActionParticipantInviteAccept::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(actionData);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(actionData, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, data.TargetId, targetWorldState);
	// Revert change based on data that were used for confirmation.
	targetWorldState->Deals[data.DealId]->Participants.Remove(data.TargetId);
}