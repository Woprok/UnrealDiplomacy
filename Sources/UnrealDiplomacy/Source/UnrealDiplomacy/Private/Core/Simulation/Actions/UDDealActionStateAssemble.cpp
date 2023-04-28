// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionStateAssemble.h"

bool UUDDealActionStateAssemble::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionStateAssemble::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationState == EUDDealSimulationState::CreatingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionStateAssemble::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionStateAssemble::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}
void UUDDealActionStateAssemble::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionStateAssemble::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationState = EUDDealSimulationState::CreatingDraft;
}
