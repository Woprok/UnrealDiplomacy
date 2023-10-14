// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionRumour.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionRumour::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	return IUDActionInterface::CanExecute(action, world);
}

void UUDGameActionRumour::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Reduce target reputation and invoker finances.
	FUDGameDataTarget data(action.ValueParameters);
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_REPUTATION_ID] -= 25;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_GOLD_ID] -= 75;
}

void UUDGameActionRumour::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Increase target reputation and invoker finances.
	FUDGameDataTarget data(action.ValueParameters);
	world->Factions[data.TargetId]->Resources[UD_RESOURCE_REPUTATION_ID] += 25;
	world->Factions[action.InvokerFactionId]->Resources[UD_RESOURCE_GOLD_ID] += 75;
}

#define LOCTEXT_NAMESPACE "Rumour"
FUDActionPresentation UUDGameActionRumour::GetPresentation() const
{
	if (GetId() != UUDGameActionRumour::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("Rumour", "Illicit Rumour")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_DECISION_DIRECT,
			UD_ACTION_TAG_DECISION_CONSEQUENCE,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("Rumour",
		"Faction [{INVOKER}] spreads illicit rumours about [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("Rumour",
		"Faction [{INVOKER}] will spread illicit rumours about [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE