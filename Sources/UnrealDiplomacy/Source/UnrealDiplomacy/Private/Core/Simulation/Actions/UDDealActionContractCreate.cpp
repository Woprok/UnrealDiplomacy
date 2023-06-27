// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isEmpty = world->Deals[data.DealId]->DealActionList.IsEmpty();
	return IUDActionInterface::CanExecute(action, world) && isEmpty;
}

void UUDDealActionContractCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Defines new action list.
	FUDDealData data(action.ValueParameters);
	TArray<FUDActionData> actions = UUDDealActionContractCreate::FinalizeActions(world, data.DealId);
	world->Deals[data.DealId]->DealActionList = UUDDealActionContractCreate::WrapActions(actions);
}

void UUDDealActionContractCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes action list.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList.Empty();
}

TArray<FUDActionData> UUDDealActionContractCreate::FinalizeActions(TObjectPtr<UUDWorldState> world, int32 dealUniqueId)
{
	TArray<FUDActionData> actions;
	auto deal = world->Deals[dealUniqueId];
	for (auto point : deal->Points)
	{
		auto actionId = point.Value->ActionId;
		// TODO figure out what this field is supposed to do in actions.
		auto type = point.Value->Type;
		auto invoker = point.Value->Invoker;
		// Value and Text parameters should be same.		
		// Create action template by combining invoker and parameters
		actions.Add(FUDActionData(actionId, invoker, point.Value->ValueParameters, point.Value->TextParameter));
	}
	return actions;
}

TArray<FUDDiscussionAction> UUDDealActionContractCreate::WrapActions(TArray<FUDActionData> data)
{
	TArray<FUDDiscussionAction> dealActions;
	for (auto action : data)
	{
		dealActions.Add(FUDDiscussionAction(
			action,
			EUDDealActionResult::Unresolved,
			false,
			UUDGlobalData::InvalidActionId)
		);
	}
	return dealActions;
}