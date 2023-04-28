// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"

void UUDDealActionParticipantInviteReject::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Request is removed from queue, without any effect being applied.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(data);
	RemovePendingTargetRequest(data, action.TargetId, world);
	// Block player future participation.
	world->Deals[action.DealId]->BlockedParticipants.Add(action.TargetId);
}

void UUDDealActionParticipantInviteReject::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Request is returned to queue.
	FUDDealTargetData data = UUDInviteParticipantDealAction::ConvertData(data);
	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(data, UUDInviteParticipantDealAction::ActionTypeId);
	AddPendingTargetRequest(originalActionData, action.TargetId, world);
	// Enable blocked player participation.
	world->Deals[action.DealId]->BlockedParticipants.Remove(action.TargetId);
}