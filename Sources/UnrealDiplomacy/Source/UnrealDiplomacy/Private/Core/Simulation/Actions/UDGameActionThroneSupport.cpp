// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneSupport.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"

bool UUDGameActionThroneSupport::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);

	bool isNotSelfTargeting = action.InvokerFactionId != data.TargetId;
	bool isNotSupporting = !ModifierManager->HasValueEqualFactionModifier(faction, modifierData);
	return IUDActionInterface::CanExecute(action, world) && isNotSupporting && isNotSelfTargeting;
}

void UUDGameActionThroneSupport::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);
	ModifierManager->CreateFactionModifier(faction, modifierData);
}

void UUDGameActionThroneSupport::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback to the empty throne.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	ModifierManager->RemoveFactionModifier(faction, UUDFactionModifierThroneSupport::ModifierTypeId, action.UniqueId);
}

void UUDGameActionThroneSupport::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "ThroneSupport"
FUDActionPresentation UUDGameActionThroneSupport::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneSupport::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneSupport", "Throne Support")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_DECISION_OFFER,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);	
	presentation.MessageContentFormat = FText(LOCTEXT("ThroneSupport",
		"Faction [{INVOKER}] supports [{TARGET}] claim to the throne."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ThroneSupport",
		"Faction [{INVOKER}] will support [{TARGET}] claim to the throne."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE