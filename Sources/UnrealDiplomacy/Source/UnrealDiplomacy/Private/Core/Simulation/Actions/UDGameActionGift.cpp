// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/Simulation/Actions/UDGameActionGiftReject.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionGift::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
	bool isPositiveAmount = data.Amount > 0;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isPositiveAmount;
}

void UUDGameActionGift::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
	AddPendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionGift::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);
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
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_PARAMETER_RESOURCE,
			UD_ACTION_TAG_PARAMETER_VALUE,
			UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT,
		}
	);

	presentation.AcceptActionId = UUDGameActionGiftAccept::ActionTypeId;
	presentation.RejectActionId = UUDGameActionGiftReject::ActionTypeId;
	presentation.MessageContentFormat = FText(LOCTEXT("Gift",
		"Faction [{INVOKER}] offered your faction [{TARGET}] [{VALUE}] in [{RESOURCE}].\nDo you accept ?"
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE