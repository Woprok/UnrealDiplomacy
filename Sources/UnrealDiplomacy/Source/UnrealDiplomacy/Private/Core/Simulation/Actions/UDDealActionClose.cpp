// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionClose.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionClose::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isOpen;
}
void UUDDealActionClose::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move to closed.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Closed;
}
void UUDDealActionClose::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move back to opened.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
	// TODO REVERT
	// world->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
}