// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionFinalize.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDDealActionContractReputationBonus.h"

bool UUDDealActionFinalize::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	//bool areAllActionsResolved = UUDDealActionContractExecute::AreAllActionsPrepared(world, data.DealId);
	//bool dealIsNotExecuted = world->Deals[data.DealId]->
	//	return IUDActionInterface::CanExecute(action, world) && areAllActionsResolved && dealIsNotExecuted;
	return false;
}
void UUDDealActionFinalize::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);

	// Execution is empty as this action only invokes other.
}
void UUDDealActionFinalize::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);

	// Execution is always reverted if all subactions were revoked.
}