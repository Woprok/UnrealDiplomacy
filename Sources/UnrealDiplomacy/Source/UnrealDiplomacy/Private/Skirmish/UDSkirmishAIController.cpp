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
}

void AUDSkirmishAIController::OnActionExecuted(FUDActionData& executedAction)
{
	// Here goes code that implement decisions for player to do whenever they make actions.
	if (executedAction.ActionTypeId != 0 && executedAction.InvokerPlayerId == GetControllerUniqueId())
	{
		// Simple log for now.
		// Executed everytime this controller was forced to do an action, that was not a log.
		FUDActionData log(0, 0);
		OnActionDecidedDelegate.ExecuteIfBound(log);
	}
}