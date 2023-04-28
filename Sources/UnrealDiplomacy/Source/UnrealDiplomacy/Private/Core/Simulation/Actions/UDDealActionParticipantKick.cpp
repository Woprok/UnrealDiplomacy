// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"

bool UUDDealActionParticipantKick::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isKickedPresent = world->Deals[action.DealId]->Participants.Contains(action.TargetId);
		result = result && isStateOpen && isResultOpen && isModerator && isKickedPresent;
	}
	return result;
}

void UUDDealActionParticipantKick::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Remove invoker from participants
	FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(data);
	world->Deals[action.DealId]->Participants.Remove(action.TargetId);
	world->Deals[action.DealId]->BlockedParticipants.Add(action.TargetId);
}

void UUDDealActionParticipantKick::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Add invoker back to participants
	FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(data);
	world->Deals[action.DealId]->BlockedParticipants.Remove(action.TargetId);
	world->Deals[action.DealId]->Participants.Add(action.TargetId);
}