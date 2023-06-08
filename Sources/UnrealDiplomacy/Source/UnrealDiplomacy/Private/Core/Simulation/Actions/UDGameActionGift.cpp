// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionGift::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetAmount data(action.ValueParameters);
	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
	bool isPositiveAmount = data.Amount > 0;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isPositiveAmount;
}

void UUDGameActionGift::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDGameDataTargetAmount data(action.ValueParameters);
	AddPendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionGift::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDGameDataTargetAmount data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
}

#define LOCTEXT_NAMESPACE "Gift"
FUDActionPresentation UUDGameActionGift::GetPresentation() const
{
	if (GetId() != UUDGameActionGift::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("Gift", "Gift")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE