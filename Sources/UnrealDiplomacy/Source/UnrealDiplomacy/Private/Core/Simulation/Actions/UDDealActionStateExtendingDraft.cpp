// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionStateExtendingDraft.h"

bool UUDDealActionStateExtendingDraft::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationState == EUDDealSimulationState::Assembling;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionStateExtendingDraft::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}
void UUDDealActionStateExtendingDraft::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionStateExtendingDraft::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}