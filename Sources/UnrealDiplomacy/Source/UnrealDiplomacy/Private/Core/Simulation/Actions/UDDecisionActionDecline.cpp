// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDecisionActionDecline.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDecisionActionDecline::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDecisionDataDecision data(action.ValueParameters);
	bool isQueued = IsPendingTargetDecision(action.InvokerFactionId, data.DecisionId, world);
	// TODO add test for decline action ?
	// bool canExecuteAction = world->Factions[data.TargetId]->PendingDecisions[data.DecisionId].ConfirmAction;
	return UUDDecisionAction::CanExecute(action, world) && isQueued;
}

void UUDDecisionActionDecline::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move from pending to resolved. Add decline.
	FUDDecisionDataDecision data(action.ValueParameters);
	MoveToResolved(action.InvokerFactionId, data.DecisionId, world);
	world->Factions[action.InvokerFactionId]->ResolvedDecisions[data.DecisionId].Result = EUDDecisionResult::Declined;
}

void UUDDecisionActionDecline::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Move from resolved to pending. Remove decline.
	FUDDecisionDataDecision data(action.ValueParameters);
	MoveToPending(action.InvokerFactionId, data.DecisionId, world);
	world->Factions[action.InvokerFactionId]->PendingDecisions[data.DecisionId].Result = EUDDecisionResult::Pending;
}

TArray<FUDActionData> UUDDecisionActionDecline::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	FUDDecisionDataDecision data(parentAction.ValueParameters);
	TArray<FUDActionData> finalActionList = { };
	if (world->Factions[parentAction.InvokerFactionId]->ResolvedDecisions[data.DecisionId].HasDecline)
	{
		finalActionList.Add(world->Factions[parentAction.InvokerFactionId]->ResolvedDecisions[data.DecisionId].DeclineAction);
	}
	return finalActionList;
}