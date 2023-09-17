// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteConsequence.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantInviteConsequence::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	return IUDActionInterface::CanExecute(action, world) && isNotMember;
}

void UUDDealActionParticipantInviteConsequence::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Block player future participation.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantInviteConsequence::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Enable blocked player participation.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
}