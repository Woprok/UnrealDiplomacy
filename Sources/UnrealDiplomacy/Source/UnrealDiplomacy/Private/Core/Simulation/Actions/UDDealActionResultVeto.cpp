// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultVeto.h"

bool UUDDealActionResultVeto::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionResultVeto::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultVeto::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionResultVeto::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
}
void UUDDealActionResultVeto::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionResultVeto::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}