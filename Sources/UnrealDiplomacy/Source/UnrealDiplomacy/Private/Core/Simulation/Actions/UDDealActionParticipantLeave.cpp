// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"

bool UUDDealActionParticipantLeave::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionParticipantLeave::ConvertData(data);
		bool isStateOpen = world->Deals[action.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = world->Deals[action.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isLeaver = world->Deals[action.DealId]->Participants.Contains(action.InvokerPlayerId);
		result = result && isStateOpen && isResultOpen && isLeaver;
	}
	return result;
}

void UUDDealActionParticipantLeave::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	// Remove invoker from participants
	FUDDealData data = UUDDealActionParticipantLeave::ConvertData(data);
	world->Deals[action.DealId]->Participants.Remove(action.InvokerPlayerId);
	world->Deals[action.DealId]->BlockedParticipants.Add(action.InvokerPlayerId);
}

void UUDDealActionParticipantLeave::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	// Add invoker back to participants
	FUDDealData data = UUDDealActionParticipantLeave::ConvertData(data);
	world->Deals[action.DealId]->BlockedParticipants.Remove(action.InvokerPlayerId);
	world->Deals[action.DealId]->Participants.Add(action.InvokerPlayerId);
}
