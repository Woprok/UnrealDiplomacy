// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultPass.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionResultPass::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isStateBefore = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isStateBefore;
}
void UUDDealActionResultPass::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move to passed.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}
void UUDDealActionResultPass::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Move back to opened.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}