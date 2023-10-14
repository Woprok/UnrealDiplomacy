// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionBurglary.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionBurglary::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	return IUDActionInterface::CanExecute(action, world);
}

void UUDGameActionBurglary::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Reduce target reputation and invoker finances.
	FUDGameDataTarget data(action.ValueParameters);
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_LUXURIES_ID] -= 50;
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_GOLD_ID] -= 100;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_LUXURIES_ID] += 50;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_GOLD_ID] += 100;
}

void UUDGameActionBurglary::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Increase target reputation and invoker finances.
	FUDGameDataTarget data(action.ValueParameters);
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_LUXURIES_ID] += 50;
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_GOLD_ID] += 100;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_LUXURIES_ID] -= 50;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_GOLD_ID] -= 100;
}

#define LOCTEXT_NAMESPACE "Burglary"
FUDActionPresentation UUDGameActionBurglary::GetPresentation() const
{
	if (GetId() != UUDGameActionBurglary::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("Burglary", "Burglary")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_DECISION_DIRECT,
			UD_ACTION_TAG_DECISION_CONSEQUENCE,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("Burglary",
		"Faction [{INVOKER}] perpetrates burglary against [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("Burglary",
		"Faction [{INVOKER}] will perpetrate burglary against [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE