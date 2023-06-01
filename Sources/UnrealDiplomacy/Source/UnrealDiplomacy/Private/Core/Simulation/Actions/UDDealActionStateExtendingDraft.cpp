// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionStateExtendingDraft.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionStateExtendingDraft::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isParticipant = world->Deals[data.DealId]->Participants.Contains(action.InvokerFactionId);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isStateBefore = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Assembling;
	return IUDActionInterface::CanExecute(action, world) && isParticipant && isModerator && isStateBefore;
}
void UUDDealActionStateExtendingDraft::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Advances the state to assembling.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}
void UUDDealActionStateExtendingDraft::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts back to previous state.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}