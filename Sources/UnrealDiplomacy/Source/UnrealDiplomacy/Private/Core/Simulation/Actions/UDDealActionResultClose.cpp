// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultClose.h"

bool UUDDealActionResultClose::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionResultClose::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultClose::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	FUDDealData data = UUDDealActionResultClose::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Closed;
}
void UUDDealActionResultClose::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	FUDDealData data = UUDDealActionResultClose::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}