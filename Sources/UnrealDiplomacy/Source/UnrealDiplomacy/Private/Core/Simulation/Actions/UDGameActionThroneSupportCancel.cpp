// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneSupportCancel.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"

bool UUDGameActionThroneSupportCancel::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);

	bool isNotSelfTargeting = action.InvokerFactionId != data.TargetId;
	bool isSupporting = ModifierManager->HasValueEqualFactionModifier(faction, modifierData);
	return IUDActionInterface::CanExecute(action, world) && isSupporting && isNotSelfTargeting;
}

void UUDGameActionThroneSupportCancel::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Cancel support target.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);
	int32 oldModifierActionId = ModifierManager->GetFactionModifierActionId(faction, modifierData);
	ModifierManager->RemoveFactionModifier(faction, oldModifierActionId);
}

void UUDGameActionThroneSupportCancel::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Support target.
	// TODO this must restore action id from the removed one, otherwise UUDGameActionThroneSupport will crash on revert...
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);
	ModifierManager->CreateFactionModifier(faction, modifierData);
}

void UUDGameActionThroneSupportCancel::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "ThroneSupportCancel"
FUDActionPresentation UUDGameActionThroneSupportCancel::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneSupportCancel::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneSupportCancel", "End Throne Support")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_DECISION_DIRECT,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);	
	presentation.MessageContentFormat = FText(LOCTEXT("ThroneSupportCancel",
		"Faction [{INVOKER}] stops support of [{TARGET}] claim to the throne."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ThroneSupportCancel",
		"Faction [{INVOKER}] will stop support of [{TARGET}] claim to the throne."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE