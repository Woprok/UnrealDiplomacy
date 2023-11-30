// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"

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
	// Execute change based on data. Invoker transfers to target.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);

	ResourceManager->Substract(world->Factions[action.InvokerFactionId], data.Resource, data.Amount);
	ResourceManager->Add(world->Factions[data.TargetId], data.Resource, data.Amount);
}

void UUDGameActionGift::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert change based on data. Invoker was one that gave away the resources.
	FUDGameDataTargetResourceAmount data(action.ValueParameters);

	ResourceManager->Add(world->Factions[action.InvokerFactionId], data.Resource, data.Amount);
	ResourceManager->Substract(world->Factions[data.TargetId], data.Resource, data.Amount);
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
			UD_ACTION_TAG_DECISION_DIRECT,
			UD_ACTION_TAG_DECISION_REQUEST,
			UD_ACTION_TAG_DECISION_DEMAND,
		}
	);

	presentation.MessageContentFormat = FText(LOCTEXT("Gift",
		"Faction [{INVOKER}] sends to faction [{TARGET}] following: [{VALUE}] in [{RESOURCE}]]."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("Gift",
		"Faction [{INVOKER}] will send [{VALUE}] [{RESOURCE}] to [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE

void UUDGameActionGift::SetResourceManager(TObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}