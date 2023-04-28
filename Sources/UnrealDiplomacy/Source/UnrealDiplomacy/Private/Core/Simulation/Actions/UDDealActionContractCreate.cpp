// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractCreate.h"

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

TArray<FUDDiscsussionAction> UUDDealActionContractCreate::WrapActions(TArray<FUDActionData> data)
{
	TArray<FUDDiscsussionAction> dealActions;
	for (auto action : data)
	{
		dealActions.Add(FUDDiscsussionAction(
			action,
			EUDDealActionResult::Unresolved,
			false,
			UUDWorldState::GaiaWorldStateId)
		);
	}
	return dealActions;
}

bool UUDDealActionContractCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionContractCreate::ConvertData(data);
		bool isEmpty = world->Deals[action.DealId]->DealActionList.IsEmpty();
		result = result && isEmpty;
	}
	return result;
}
void UUDDealActionContractCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(data, world);
	FUDDealData data = UUDDealActionContractCreate::ConvertData(data);
	TArray<FUDActionData> actions = UUDDealActionContractCreate::FinalizeActions(world, action.DealId);
	world->Deals[action.DealId]->DealActionList = UUDDealActionContractCreate::WrapActions(actions);
}
void UUDDealActionContractCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(data, world);
	FUDDealData data = UUDDealActionContractCreate::ConvertData(data);
	world->Deals[action.DealId]->DealActionList.Empty();
}