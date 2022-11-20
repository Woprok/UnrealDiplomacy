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
	// Do nothing. Whoever inherits this should consider override for custom logic.
	// Simulator called this after action was executed

	// We have to pass our action if its our turn

	// We are quite lazy so let's do the simplest one
	//	FUDActionData log(0, 0);


	//OnActionDecidedDelegate.ExecuteIfBound(log);

	//void IUDActionHandlingInterface::BindActionExecuted(TObjectPtr<AUDWorldSimulation> authority)
	//{
	//	authority->OnNotifyActionExecutedDelegate.AddUObject(this, &IUDActionHandlingInterface::OnActionExecuted);
	//}
}