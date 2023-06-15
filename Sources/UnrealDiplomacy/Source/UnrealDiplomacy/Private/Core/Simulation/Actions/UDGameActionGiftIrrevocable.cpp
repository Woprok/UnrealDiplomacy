// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionGiftIrrevocable::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetAmount data(action.ValueParameters);
	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
	bool isPositiveAmount = data.Amount > 0;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isPositiveAmount;
}

void UUDGameActionGiftIrrevocable::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Transfer resource to target.
	FUDGameDataTargetAmount data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->ResourceGold -= data.Amount;
	world->Factions[data.TargetId]->ResourceGold += data.Amount;
}

void UUDGameActionGiftIrrevocable::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Transfer resource back from target.
	FUDGameDataTargetAmount data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->ResourceGold += data.Amount;
	world->Factions[data.TargetId]->ResourceGold -= data.Amount;
}

#define LOCTEXT_NAMESPACE "GiftIrrevocable"
FUDActionPresentation UUDGameActionGiftIrrevocable::GetPresentation() const
{
	if (GetId() != UUDGameActionGiftIrrevocable::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("GiftIrrevocable", "Irrevocable Gift")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
			UD_ACTION_TAG_FACTION_INTERACTION,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE