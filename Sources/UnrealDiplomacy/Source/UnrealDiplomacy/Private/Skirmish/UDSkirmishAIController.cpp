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

	if (executedAction.ActionTypeId == UUDGiftAction::ActionTypeId)
	{
		// Ai is able to accept gift without any worry about it future :)
		FUDActionData confirm(executedAction, UUDConfirmGiftAction::ActionTypeId);
		OnActionDecidedDelegate.ExecuteIfBound(confirm);
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