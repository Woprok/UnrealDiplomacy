// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionMessageSend.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionMessageSend::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isParticipant = world->Deals[data.DealId]->Participants.Contains(action.InvokerPlayerId);
	bool isNotEmptyMessage = action.TextParameter.Len() > 0;
	return IUDActionInterface::CanExecute(action, world) && isParticipant && isNotEmptyMessage;
}
void UUDDealActionMessageSend::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Add message to history.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->ChatHistory.Add(action.TextParameter);
}
void UUDDealActionMessageSend::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove last added message from the history.
	FUDDealData data(action.ValueParameters);
	int32 lastItem = world->Deals[data.DealId]->ChatHistory.Num() - 1;
	world->Deals[data.DealId]->ChatHistory.RemoveAt(lastItem);
}