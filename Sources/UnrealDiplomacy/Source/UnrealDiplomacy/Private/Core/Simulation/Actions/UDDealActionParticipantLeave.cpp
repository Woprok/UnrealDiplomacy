// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"

bool UUDDealActionParticipantLeave::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionParticipantLeave::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isLeaver = targetWorldState->Deals[data.DealId]->Participants.Contains(actionData.InvokerPlayerId);
		result = result && isStateOpen && isResultOpen && isLeaver;
	}
	return result;
}

void UUDDealActionParticipantLeave::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Remove invoker from participants
	FUDDealData data = UUDDealActionParticipantLeave::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Participants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Add(actionData.InvokerPlayerId);
}

void UUDDealActionParticipantLeave::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Add invoker back to participants
	FUDDealData data = UUDDealActionParticipantLeave::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->BlockedParticipants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals[data.DealId]->Participants.Add(actionData.InvokerPlayerId);
}
