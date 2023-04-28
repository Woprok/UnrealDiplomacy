// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultPass.h"

bool UUDDealActionResultPass::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionResultPass::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultPass::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionResultPass::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}
void UUDDealActionResultPass::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionResultPass::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}