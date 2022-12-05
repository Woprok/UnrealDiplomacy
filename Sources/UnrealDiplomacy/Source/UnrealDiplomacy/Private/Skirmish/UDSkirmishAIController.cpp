// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishAIController.h"

void AUDSkirmishAIController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishAIController::GetControllerUniqueId()
{
	return UniqueControllerId;
}

void AUDSkirmishAIController::ListenActionExecutor(TObjectPtr<AUDWorldSimulation> authority)
{
	authority->OnBroadcastActionExecutedDelegate.AddUObject(this, &AUDSkirmishAIController::OnActionExecuted);
	PersonalState = authority->GetSpecificState(GetControllerUniqueId());
}

void AUDSkirmishAIController::OnActionExecuted(FUDActionData& executedAction)
{
	// This should contain decisions, or alternatively if we reach a point where behaviour tree is implemented
	// this should be left empty :)
	// TODO behaviour tree

	if (executedAction.ActionTypeId == UUDGiftAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	{
		// Ai is able to accept gift without any worry about it future :)
		FUDActionData confirm(executedAction, UUDConfirmGiftAction::ActionTypeId);
		OnActionDecidedDelegate.ExecuteIfBound(confirm);
		return;
	}

	if (executedAction.ActionTypeId == UUDTransferTileAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	{
		// Ai is able to accept tile without any worry about it future :)
		FUDActionData confirm(executedAction, UUDConfirmTransferTileAction::ActionTypeId);
		OnActionDecidedDelegate.ExecuteIfBound(confirm);
		return;
	}

	if (executedAction.ActionTypeId == UUDGrantExploitTilePermissionAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	{
		// Exploit your new tile in use
		FUDActionData exploitNewTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, GetControllerUniqueId(), executedAction.TileParameter);
		OnActionDecidedDelegate.ExecuteIfBound(exploitNewTile);
		return;
	}

	if (executedAction.ActionTypeId == UUDConfirmTransferTileAction::ActionTypeId && executedAction.TargetPlayerId == GetControllerUniqueId())
	{
		FUDActionData exploitNewTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, GetControllerUniqueId(), executedAction.TileParameter);
		OnActionDecidedDelegate.ExecuteIfBound(exploitNewTile);
		return;
	}

	// End turn automatically as there is no other action to await for.
	if (executedAction.ActionTypeId == UUDEndTurnAction::ActionTypeId && PersonalState->CurrentTurnPlayerId == GetControllerUniqueId())
	{
		FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(endTurn);
		return;
	}
}