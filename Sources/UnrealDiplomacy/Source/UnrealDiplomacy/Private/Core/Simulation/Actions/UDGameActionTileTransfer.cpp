// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionTileTransfer::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDGameDataTargetTile data(action.ValueParameters);
	FIntPoint tile(data.X, data.Y);
	bool isDifferentPlayer = action.InvokerFactionId != data.TargetId;
	bool isOwner = action.InvokerFactionId == world->Map->GetTile(tile)->OwnerUniqueId;
	return IUDActionInterface::CanExecute(action, world) && isDifferentPlayer && isOwner;
}

void UUDGameActionTileTransfer::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDGameDataTargetTile data(action.ValueParameters);
	AddPendingTargetRequest(action, data.TargetId, world);
}

void UUDGameActionTileTransfer::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDGameDataTargetTile data(action.ValueParameters);
	RemovePendingTargetRequest(action, data.TargetId, world);
}

#define LOCTEXT_NAMESPACE "TileTransfer"
FUDActionPresentation UUDGameActionTileTransfer::GetPresentation() const
{
	if (GetId() != UUDGameActionTileTransfer::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("TileTransfer", "Transfer Tile")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
			UD_ACTION_TAG_FACTION_INTERACTION,
			UD_ACTION_TAG_PARAMETER_FACTION,
			UD_ACTION_TAG_PARAMETER_TILE,
		}
	);

	presentation.AcceptActionId = UUDGameActionTileTransferAccept::ActionTypeId;
	presentation.RejectActionId = UUDGameActionTileTransferReject::ActionTypeId;
	presentation.MessageContentFormat = FText(LOCTEXT("TileTransfer", 
		"Faction [{INVOKER}] offered your faction [{TARGET}] province [{TILE}].\nDo you accept ?"
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE