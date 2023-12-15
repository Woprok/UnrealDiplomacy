// Copyright Miroslav Valach
#include "Core/Simulation/Actions/UDDecisionActionCreate.h"
#include "Core/Simulation/Actions/UDDecisionActionConfirm.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDFactionModifierDecisionSend.h"

// Note:
// Invoker is the one that created the decision for other player.
// Target is the one that has to decide.
// In case of direct there is no decision.
// In case of Demand and Request, action passed in parameters has reversed invoker and target.
// To prevent repeated creations and spam:
// Modifier must be created on the invoker, to prevent him from using repeatedly decision create
// Target is innocenet, thus there is no need to do anything to him

bool UUDDecisionActionCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// Parse decision
	FUDDecisionDataTargetTypeActionParameters data(action.ValueParameters);
	EUDDecisionType decisionType = UUDDecisionAction::IntegerToDecisionType(data.Type);
	FUDActionData confirm = FUDActionData::FromValues(data.Parameters);

	const TObjectPtr<UUDFactionState>& creatorFaction = world->Factions[action.InvokerFactionId];

	bool isNotTarget = action.InvokerFactionId != data.TargetId;
	const auto& mods = ModifierManager->GetAllFactionModifiers(creatorFaction, UUDFactionModifierDecisionSend::ModifierTypeId);
		
	const auto& foundMod = mods.FindByPredicate(
		[&confirm](const FUDModifierData& data) {	return data.ValueParameters[0] == confirm.ActionTypeId; }
	);
	bool hasNotSameActionTypeDecisionPending = foundMod == nullptr;

	return IUDActionInterface::CanExecute(action, world) && isNotTarget && hasNotSameActionTypeDecisionPending;
}

void UUDDecisionActionCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDDecisionDataTargetTypeActionParameters data(action.ValueParameters);
	EUDDecisionType decisionType = UUDDecisionAction::IntegerToDecisionType(data.Type);
	FUDActionData confirm = FUDActionData::FromValues(data.Parameters);
	FUDDecision newDecision;

	// This handles the demand consequence case 
	if (UUDDecisionAction::IntegerToDecisionType(data.Type) == EUDDecisionType::Demand)
	{
		FUDActionData decline(world->Factions[action.InvokerFactionId]->DecisionDemandPolicy, action.InvokerFactionId, { data.TargetId });
		newDecision = FUDDecision(decisionType, confirm, decline, action.InvokerFactionId);
	}
	// HACK: inserts consequence for the actions that have improper decline replacement defined.
	// e.g. this exists only for deals invite handling...
	// basically old way of doing decisions...
	else if (data.DeclineReplacementActionId != UUDGlobalData::InvalidActionId)
	{
		FUDActionData decline = FUDActionData::FromValues(data.Parameters);
		decline.ActionTypeId = data.DeclineReplacementActionId;
		newDecision = FUDDecision(decisionType, confirm, decline, action.InvokerFactionId);
	}
	else
	{
		newDecision = FUDDecision(decisionType, confirm, action.InvokerFactionId);
	}

	// DecisionId is ActionUniqueId, thus any action that confirms or declines can just remove modifier with DecisionId
	const TObjectPtr<UUDFactionState>& creatorFaction = world->Factions[action.InvokerFactionId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierDecisionSend::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId, { confirm.ActionTypeId }
	);
	ModifierManager->CreateFactionModifier(creatorFaction, modifierData);

	AddPendingTargetDecision(data.TargetId, action.UniqueId, newDecision, world);
}

void UUDDecisionActionCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDecisionDataTargetTypeActionParameters data(action.ValueParameters);
	FUDActionData decisionAction = FUDActionData::FromValues(data.Parameters);

	const TObjectPtr<UUDFactionState>& creatorFaction = world->Factions[action.InvokerFactionId];
	ModifierManager->RemoveFactionModifier(creatorFaction, UUDFactionModifierDecisionSend::ModifierTypeId, action.UniqueId);

	RemovePendingTargetDecision(data.TargetId, action.UniqueId, world);
}

FUDActionPresentation UUDDecisionActionCreate::GetPresentation() const
{
	FUDActionPresentation presentation = Super::GetPresentation();
	presentation.ActionId = GetId();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VERIFY_PARAMETER_MINIMUM,
		}
	);

	return presentation;
}

TArray<FUDActionData> UUDDecisionActionCreate::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	TArray<FUDActionData> directExecution = { };
	FUDDecisionDataTargetTypeActionParameters data(parentAction.ValueParameters);
	// If enabled follows execution of the direct decisions. Thus this will call confirm on these decisions immediately as consequence.
	if (UUDGlobalGameConfig::IsDirectExecutionEnabled && 
		UUDDecisionAction::IntegerToDecisionType(data.Type) == EUDDecisionType::Gift)
	{
		FUDActionData decisionAction = FUDActionData(UUDDecisionActionConfirm::ActionTypeId, data.TargetId, { parentAction.UniqueId });
		directExecution.Add(decisionAction);
	}

	return directExecution;
}

void UUDDecisionActionCreate::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}