// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"

void UUDDealActionParticipantInvite::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Queue new confirmable request.
	FUDDealTargetData data = UUDDealActionParticipantInvite::ConvertData(actionData);
	AddPendingTargetRequest(actionData, data.TargetId, targetWorldState);
}

void UUDDealActionParticipantInvite::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove request from queue.
	FUDDealTargetData data = UUDDealActionParticipantInvite::ConvertData(actionData);
	RemovePendingTargetRequest(actionData, data.TargetId, targetWorldState);
}