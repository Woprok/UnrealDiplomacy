// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"

bool UUDDealActionParticipantKick::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isKickedPresent = targetWorldState->Deals[data.DealId]->Participants.Contains(data.TargetId);
		result = result && isStateOpen && isResultOpen && isModerator && isKickedPresent;
	}
	return result;
}

void UUDDealActionParticipantKick::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Remove invoker from participants
	FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Remove(data.TargetId);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantKick::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Add invoker back to participants
	FUDDealTargetData data = UUDDealActionParticipantKick::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
	targetWorldState->Deals[data.DealId]->Participants.Add(data.TargetId);
}