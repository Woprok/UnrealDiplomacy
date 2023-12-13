// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileBuildQuarry.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingQuarry.h"

bool UUDGameActionTileBuildQuarry::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingQuarry::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);

	bool isNotSelfTargeting = action.InvokerFactionId != data.TargetId;
	bool isNotSupporting = !ModifierManager->HasFactionModifier(faction, modifierData);
	return IUDActionInterface::CanExecute(action, world) && isNotSupporting && isNotSelfTargeting;
}

void UUDGameActionTileBuildQuarry::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	FUDModifierData modifierData = FUDModifierData(
		UUDTileModifierBuildingQuarry::ModifierTypeId, action.UniqueId,
		action.InvokerFactionId, data.TargetId
	);
	ModifierManager->CreateFactionModifier(faction, modifierData);
}

void UUDGameActionTileBuildQuarry::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback to the empty throne.
	FUDGameDataTarget data(action.ValueParameters);
	const TObjectPtr<UUDFactionState>& faction = world->Factions[data.TargetId];
	ModifierManager->RemoveFactionModifier(faction, action.UniqueId);
}

void UUDGameActionTileBuildQuarry::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

#define LOCTEXT_NAMESPACE "TileBuildQuarry"
FUDActionPresentation UUDGameActionTileBuildQuarry::GetPresentation() const
{
	if (GetId() != UUDGameActionTileBuildQuarry::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileBuildQuarry", "Tile Build Quarry")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
			UD_ACTION_TAG_TILE_INTERACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
			UD_ACTION_TAG_DECISION_DIRECT,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("TileBuildQuarry",
		"Faction [{INVOKER}] builds Quarry on province [{TILE}] owned by [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("TileBuildQuarry",
		"Faction [{INVOKER}] will build Quarry on province [{TILE}] owned by [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE