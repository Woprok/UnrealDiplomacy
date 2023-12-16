// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionThroneAbdicate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isRuler = world->ImperialThrone.Ruler == action.InvokerFactionId;
	return IUDActionInterface::CanExecute(action, world) && isRuler;
}

void UUDGameActionThroneAbdicate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the throne..
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaFactionId;
}

void UUDGameActionThroneAbdicate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is returned to throne.
	world->ImperialThrone.Ruler = action.InvokerFactionId;
}

#define LOCTEXT_NAMESPACE "ThroneAbdicate"
FUDActionPresentation UUDGameActionThroneAbdicate::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneAbdicate::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneAbdicate", "Abdicate Throne")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_FREE_DEAL_USE,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("ThroneAbdicate",
		"Faction [{INVOKER}] abdicates."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ThroneAbdicate",
		"Faction [{INVOKER}] will abdicate."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE