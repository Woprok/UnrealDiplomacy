// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionNone.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionNone::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// Always useable by default.
	return IUDActionInterface::CanExecute(action, world);
}

void UUDGameActionNone::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Finished by default.
}

void UUDGameActionNone::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Finished by default.
}

#define LOCTEXT_NAMESPACE "None"
FUDActionPresentation UUDGameActionNone::GetPresentation() const
{
	if (GetId() != UUDGameActionNone::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("None", "None")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_DECISION_CONSEQUENCE,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("None",
		"Faction [{INVOKER}] has no plan towards [{TARGET}]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("None",
		"Faction [{INVOKER}] has no plan towards [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE