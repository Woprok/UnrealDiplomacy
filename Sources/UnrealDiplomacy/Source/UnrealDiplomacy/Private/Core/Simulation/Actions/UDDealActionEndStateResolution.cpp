// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionEndStateResolution.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionEndStateResolution::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isCorrectState = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ResolutionOfPoints;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	bool areAllActionsResolved = UUDDealActionEndStateResolution::AreAllActionsPrepared(world, data.DealId);
	return IUDActionInterface::CanExecute(action, world) && isModerator && isCorrectState && isOpen && areAllActionsResolved;
}

void UUDDealActionEndStateResolution::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Advances the state to assembling.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->CanBeExecuted = true;
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
}

void UUDDealActionEndStateResolution::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Reverts back to previous state.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->CanBeExecuted = false;
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ResolutionOfPoints;
}

bool UUDDealActionEndStateResolution::AreAllActionsPrepared(TObjectPtr<UUDWorldState> world, int32 dealUniqueId)
{
	for (auto actionWrapper : world->Deals[dealUniqueId]->DealActionList)
	{
		if (actionWrapper.SelectedResult < EUDDealActionResult::Accepted)
		{
			return false;
		}
	}
	return true;
}
