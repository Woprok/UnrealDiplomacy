// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"

void UUDDealActionParticipantInvite::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDDealTargetData data = UUDDealActionParticipantInvite::ConvertData(action);
	AddPendingTargetRequest(data, data.TargetId, world);
}

void UUDDealActionParticipantInvite::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDealTargetData data = UUDDealActionParticipantInvite::ConvertData(action);
	RemovePendingTargetRequest(data, action.TargetId, world);
}