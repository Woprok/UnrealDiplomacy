// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneUsurper.h"

bool UUDGameActionThroneUsurp::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneEmpty = world->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId;

	const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneUsurper::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);

	bool isNotRecentUsurper = !ModifierManager->HasValueEqualFactionModifier(faction, modifierData);

	return IUDActionInterface::CanExecute(action, world) && isThroneEmpty && isNotRecentUsurper;
}

void UUDGameActionThroneUsurp::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Prevent future takeovers by this action.
	const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierThroneUsurper::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, action.InvokerFactionId
	);
	ModifierManager->CreateFactionModifier(faction, modifierData);
	// Takeover the empty throne.
	world->ImperialThrone.Ruler = action.InvokerFactionId;
}

void UUDGameActionThroneUsurp::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove modifiers
	const TObjectPtr<UUDFactionState>& faction = world->Factions[action.InvokerFactionId];
	ModifierManager->RemoveFactionModifier(faction, UUDFactionModifierThroneUsurper::ModifierTypeId, action.UniqueId);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaFactionId;
}

#define LOCTEXT_NAMESPACE "ThroneUsurp"
FUDActionPresentation UUDGameActionThroneUsurp::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneUsurp::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneUsurp", "Throne Usurp")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
		}
	);
	presentation.DealContentFormat = FText(LOCTEXT("ThroneUsurp",
		"Faction [{INVOKER}] will attempt to take throne."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE

void UUDGameActionThroneUsurp::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}