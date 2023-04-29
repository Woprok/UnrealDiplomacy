// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantInvite::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	return IUDActionInterface::CanExecute(action, world) && isNotMember;
}

void UUDDealActionParticipantInvite::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDDealTargetData data(action.ValueParameters);
	AddPendingTargetRequest(action, data.TargetId, world);
}

void UUDDealActionParticipantInvite::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDealTargetData data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
}