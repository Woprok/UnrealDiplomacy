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
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierStratagemShare::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);

	bool isNotSelfTargeting = action.InvokerFactionId != data.TargetId;
	bool isNotSupporting = !ModifierManager->HasFactionModifier(faction, modifierData);
	return IUDActionInterface::CanExecute(action, world) && isNotSupporting && isNotSelfTargeting;
}

void UUDGameActionStratagemShare::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Grant access to stratagems.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDFactionModifierStratagemShare::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);
	ModifierManager->CreateFactionModifier(faction, modifierData);
}

void UUDGameActionStratagemShare::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revokes access to stratagems.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	ModifierManager->RemoveFactionModifier(faction, action.UniqueId);
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
			UD_ACTION_TAG_DECISION_OFFER,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);
	presentation.MessageContentFormat = FText(LOCTEXT("ThroneSupport",
		"Faction [{INVOKER}] share stratagem with [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ThroneSupport",
		"Faction [{INVOKER}] will share stratagem with [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE