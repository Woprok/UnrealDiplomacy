// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionStratagemShare.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemShare.h"

bool UUDGameActionStratagemShare::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetAction data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& targetFaction = world->Factions[data.TargetId];
	const TObjectPtr<UUDFactionState>& invokerFaction = world->Factions[action.InvokerFactionId];

	bool isNotSelfSharing = action.InvokerFactionId != data.TargetId;
	bool targetDoesNotHaveStratagem = !targetFaction->AccessibleStratagemOptions.Contains(data.ActionTypeId);
	bool invokerHasStratagem = invokerFaction->AccessibleStratagemOptions.Contains(data.ActionTypeId);

	return IUDActionInterface::CanExecute(action, world) && targetDoesNotHaveStratagem && invokerHasStratagem && isNotSelfSharing;
}

void UUDGameActionStratagemShare::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant access to stratagems.
	FUDGameDataTargetAction data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierStratagemShare::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId, { data.ActionTypeId }
	);
	// Create modifier and add action as accessible (might do nothing, if present, we don't care)
	faction->AccessibleStratagemOptions.Add(data.ActionTypeId);
	ModifierManager->CreateFactionModifier(faction, modifierData);
}

void UUDGameActionStratagemShare::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revokes access to stratagems.
	FUDGameDataTargetAction data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	// We do care if this is picked or not, so we never remove picked by accident.
	// Technically this should nover succeed as the execution should prevent this case.
	if (!faction->PickedStratagemOptions.Contains(data.ActionTypeId))
		faction->AccessibleStratagemOptions.Remove(data.ActionTypeId);
	ModifierManager->RemoveFactionModifier(faction, UUDFactionModifierStratagemShare::ModifierTypeId, action.UniqueId);
}

void UUDGameActionStratagemShare::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "StratagemShare"
FUDActionPresentation UUDGameActionStratagemShare::GetPresentation() const
{
	if (GetId() != UUDGameActionStratagemShare::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("StratagemShare", "Stratagem Share")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_PARAMETER_ACTION,
			UD_ACTION_TAG_DECISION_OFFER,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);
	presentation.MessageContentFormat = FText(LOCTEXT("StratagemShare",
		"Faction [{INVOKER}] share stratagem [{ACTION}] with [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("StratagemShare",
		"Faction [{INVOKER}] will share stratagem [{ACTION}] with [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE