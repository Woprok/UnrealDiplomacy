// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishAIController.h"

void AUDSkirmishAIController::InitializeAdministrator()
{
	InternalPersonalAdministrator = NewObject<UUDActionAdministrator>();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGaiaAIController(%d): Initialized personal administrator."), GetControllerUniqueId());
}

void AUDSkirmishAIController::OnActionExecuted(FUDActionData executedAction)
{
	// TODO PREPROCESSING FOR executedAction

	// AI is not allowed to act before the game is in progress.
	// AI is not allowed to act after the game is finished.
	if (!GetPersonalAdministrator()->IsGameInProgress())
	{
		return;
	}

	// AI act's only on their respective turn.
	if (!GetPersonalAdministrator()->CanEndTurn())
	{
		return;
	}
	// AI can make new decisions only once on their respective turn.
	if (IsPlaying)
	{
		// Prevents repeated nesting as it forces AI code to break.
		return;
	}
	IsPlaying = true;
	DoTurn();
	IsPlaying = false;
	return;
}

void AUDSkirmishAIController::DoTurn()
{
	if (GetPersonalAdministrator()->IsMapStateReady())
	{
		FIntPoint tile = GetPersonalAdministrator()->GetFirstNeutralTile();
		if (tile != FIntPoint(-1, -1))
		{
			OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetTakeTileAction(tile));
		}
	}

	// Naively give first player gold, TODO remove this or replace with some logic
	if (GetPersonalAdministrator()->GetCurrentResourceGold() > 100 && gifters.Num() > 0)
	{
		OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetConditionalGiftGoldAction(gifters.Pop(), 69));
	}

	// Resolve requests
	for (auto request : GetPersonalAdministrator()->GetPendingRequests())
	{
		if (request.ActionTypeId == UUDGiftAction::ActionTypeId)
		{
			gifters.Add(request.InvokerPlayerId);
			OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetConfirmConditionalGiftGoldAction(request));
		}
	}

	// Finish this by executing end turn action, thus giving up control.
	OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetEndTurnAction());

	// This should contain decisions, or alternatively if we reach a point where behaviour tree is implemented
	// this should be left empty :)
	// TODO behaviour tree

	//if (executedAction.ActionTypeId == UUDGiftAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	//{
	//	// Ai is able to accept gift without any worry about it future :)
	//	FUDActionData confirm(executedAction, UUDConfirmGiftAction::ActionTypeId);
	//	OnActionDecidedDelegate.ExecuteIfBound(confirm);
	//	return;
	//}
	//
	//if (executedAction.ActionTypeId == UUDTransferTileAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	//{
	//	// Ai is able to accept tile without any worry about it future :)
	//	FUDActionData confirm(executedAction, UUDConfirmTransferTileAction::ActionTypeId);
	//	OnActionDecidedDelegate.ExecuteIfBound(confirm);
	//	return;
	//}
	//
	//if (executedAction.ActionTypeId == UUDGrantExploitTilePermissionAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	//{
	//	// Exploit your new tile in use
	//	FUDActionData exploitNewTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, GetControllerUniqueId(), executedAction.TileParameter);
	//	OnActionDecidedDelegate.ExecuteIfBound(exploitNewTile);
	//	return;
	//}
	//
	//if (executedAction.ActionTypeId == UUDConfirmTransferTileAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	//{
	//	FUDActionData exploitNewTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, GetControllerUniqueId(), executedAction.TileParameter);
	//	OnActionDecidedDelegate.ExecuteIfBound(exploitNewTile);
	//	return;
	//}
	//
	//// End turn automatically as there is no other action to await for.
	//if (executedAction.ActionTypeId == UUDEndTurnAction::ActionTypeId && PersonalState->CurrentTurnPlayerId == GetControllerUniqueId())
	//{
	//	FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
	//	//OnActionDecidedDelegate.ExecuteIfBound(endTurn);
	//	// TODO THIS REQUIRES AT LEAST ONE PLAYER OR AI WILL NEVER END 
	//	return;
	//}
}