// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionThroneUsurp::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneEmpty = world->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId;
	bool hasStratagem = world->Factions[action.InvokerFactionId]->StratagemOptions.Contains(ActionTypeId);
	return IUDActionInterface::CanExecute(action, world) && isThroneEmpty && hasStratagem;
}

void UUDGameActionThroneUsurp::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	world->ImperialThrone.Ruler = action.InvokerFactionId;
}

void UUDGameActionThroneUsurp::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
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

	return presentation;
}
#undef LOCTEXT_NAMESPACE