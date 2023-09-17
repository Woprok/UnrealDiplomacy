// Copyright Miroslav Valach
// TODO DELETE

#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

//bool UUDGameActionGiftIrrevocable::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
//{
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
//	bool isPositiveAmount = data.Amount > 0;
//	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isPositiveAmount;
//}
//
//void UUDGameActionGiftIrrevocable::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Execute(action, world);
//	// Transfer resource to target.
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	world->Factions[action.InvokerFactionId]->Resources[data.Resource] -= data.Amount;
//	world->Factions[data.TargetId]->Resources[data.Resource] += data.Amount;
//}
//
//void UUDGameActionGiftIrrevocable::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Revert(action, world);
//	// Transfer resource back from target.
//	FUDGameDataTargetResourceAmount data(action.ValueParameters);
//	world->Factions[action.InvokerFactionId]->Resources[data.Resource] += data.Amount;
//	world->Factions[data.TargetId]->Resources[data.Resource] -= data.Amount;
//}
//
//#define LOCTEXT_NAMESPACE "GiftIrrevocable"
//FUDActionPresentation UUDGameActionGiftIrrevocable::GetPresentation() const
//{
//	if (GetId() != UUDGameActionGiftIrrevocable::ActionTypeId)
//		return Super::GetPresentation();
//	FUDActionPresentation presentation = FUDActionPresentation();
//	presentation.ActionId = GetId();
//	presentation.Name = FText(LOCTEXT("GiftIrrevocable", "Irrevocable Gift")).ToString();
//	presentation.Tags.Append(
//		{
//			UD_ACTION_TAG_VALID,
//			UD_ACTION_TAG_STRATAGEM,
//			UD_ACTION_TAG_STRATAGEM_COST_1,
//			UD_ACTION_TAG_FACTION_INTERACTION,
//			UD_ACTION_TAG_PARAMETER_FACTION,
//			UD_ACTION_TAG_PARAMETER_RESOURCE,
//			UD_ACTION_TAG_PARAMETER_VALUE,
//			UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT,
//		}
//	);
//	presentation.DealContentFormat = FText(LOCTEXT("GiftIrrevocable",
//		"Faction [{INVOKER}] will send [{VALUE}] of [{RESOURCE}] to [{TARGET}]."
//	)).ToString();
//
//	return presentation;
//}
//#undef LOCTEXT_NAMESPACE