// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultVeto.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionResultVeto::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isStateBefore = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isStateBefore;
}
void UUDDealActionResultVeto::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move to vetoed.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
}
void UUDDealActionResultVeto::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move back to opened.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}