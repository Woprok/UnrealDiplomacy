// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionEndStateAssemble.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionEndStateAssemble::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isCorrectState = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::CreateAndAssemble;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isCorrectState && isOpen;
}
void UUDDealActionEndStateAssemble::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Advances the state to assembling.
	FUDDealData data(action.ValueParameters);

	int32 partCount = world->Deals[data.DealId]->Participants.Num();
	// Deal Passed
	if (partCount >= MinimumParticipantCount)
	{
		world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DefinePoints;
	}
	// Deal Failed
	else
	{
		world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
		world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
	}

}
void UUDDealActionEndStateAssemble::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts back to previous state.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::CreateAndAssemble;
}
