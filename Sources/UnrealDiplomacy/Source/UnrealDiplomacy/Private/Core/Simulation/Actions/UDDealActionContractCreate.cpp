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
		// TODO params
		// We require Invoker for all actions.
		for (auto invoker : point.Value->Invokers)
		{
			// for each invoker and each target create a pair for actions
			// at the moment there is no way for actions to target multiple players
			// and for very sensible reason it will probably remain like that ?
			// also executing one action for each pair is expected to be same as one action for 1 : N group
			if (point.Value->Targets.Num() > 0)
			{
				for (auto target : point.Value->Targets)
				{
					TArray<int> valueParameters = { target };
					valueParameters.Append(point.Value->ValueParameters);
					actions.Add(FUDActionData(actionId, invoker, valueParameters, point.Value->TextParameter));
				}
			}
			else
			{
				TArray<int> valueParameters = point.Value->ValueParameters;
				actions.Add(FUDActionData(actionId, invoker, valueParameters, point.Value->TextParameter));
			}
		}
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
			UUDGlobalData::GaiaId)
		);
	}
	return dealActions;
}