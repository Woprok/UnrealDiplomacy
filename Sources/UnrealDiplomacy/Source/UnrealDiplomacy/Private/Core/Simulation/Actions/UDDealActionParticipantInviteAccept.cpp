// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"

void UUDDealActionParticipantInviteAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Execute change based on data contained in confirm.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(data);
	world->Deals[action.DealId]->Participants.Add(action.TargetId);
	// Remove request from queue.
	RemovePendingTargetRequest(data, action.TargetId, world);
}

void UUDDealActionParticipantInviteAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Confirmed request is returned to queue, but it has to be changed first.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(data);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
	// Revert change based on data that were used for confirmation.
	world->Deals[action.DealId]->Participants.Remove(action.TargetId);
}