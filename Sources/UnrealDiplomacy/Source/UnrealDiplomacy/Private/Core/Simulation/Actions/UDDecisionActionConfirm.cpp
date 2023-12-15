// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDecisionActionConfirm.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDFactionModifierDecisionSend.h"

bool UUDDecisionActionConfirm::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDecisionDataDecision data(action.ValueParameters);
	bool isQueued = IsPendingTargetDecision(action.InvokerFactionId, data.DecisionId, world);
	// TODO add test for confirm action ?
	// bool canExecuteAction = world->Factions[data.TargetId]->PendingDecisions[data.DecisionId].ConfirmAction;
	return UUDDecisionAction::CanExecute(action, world) && isQueued;
}

void UUDDecisionActionConfirm::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move from pending to resolved. Add confirm.
	FUDDecisionDataDecision data(action.ValueParameters);
	MoveToResolved(action.InvokerFactionId, data.DecisionId, world);
	int32 originalCreator = world->Factions[action.InvokerFactionId]->ResolvedDecisions[data.DecisionId].CreatorId;
	world->Factions[action.InvokerFactionId]->ResolvedDecisions[data.DecisionId].Result = EUDDecisionResult::Confirmed;

	// Clear modifier from original invoker player
	const TObjectPtr<UUDFactionState>& creatorFaction = world->Factions[originalCreator];
	ModifierManager->RemoveFactionModifier(creatorFaction, UUDFactionModifierDecisionSend::ModifierTypeId, data.DecisionId);
}

void UUDDecisionActionConfirm::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Move from resolved to pending. Remove confirm.
	FUDDecisionDataDecision data(action.ValueParameters);
	MoveToPending(action.InvokerFactionId, data.DecisionId, world);
	world->Factions[action.InvokerFactionId]->PendingDecisions[data.DecisionId].Result = EUDDecisionResult::Pending;

	// TODO revert back modifier to existance
}

TArray<FUDActionData> UUDDecisionActionConfirm::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	FUDDecisionDataDecision data(parentAction.ValueParameters);
	TArray<FUDActionData> finalActionList = { };
	finalActionList.Add(world->Factions[parentAction.InvokerFactionId]->ResolvedDecisions[data.DecisionId].ConfirmAction);
	return finalActionList;
}

void UUDDecisionActionConfirm::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}