// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Skirmish/UDSkirmishGameMode.h"

void AUDSkirmishGaiaAIController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishGaiaAIController::GetControllerUniqueId()
{
	return UniqueControllerId;
}

void AUDSkirmishGaiaAIController::ListenActionExecutor(TObjectPtr<AUDWorldSimulation> authority)
{
	authority->OnBroadcastActionExecutedDelegate.AddUObject(this, &AUDSkirmishGaiaAIController::OnActionExecuted);
}

void AUDSkirmishGaiaAIController::OnActionExecuted(FUDActionData& executedAction)
{
	// Do nothing. Whoever inherits this should consider override for custom logic.
	// Simulator called this after action was executed
	// We have to pass our action if its our turn
	// We are quite lazy so let's do the simplest one

	if (executedAction.ActionTypeId == 2)
	{
		FUDActionData createResources(100, 0);
		OnActionDecidedDelegate.ExecuteIfBound(createResources);
	}

	//void IUDActionHandlingInterface::BindActionExecuted(TObjectPtr<AUDWorldSimulation> authority)
	//{
	//	authority->OnNotifyActionExecutedDelegate.AddUObject(this, &IUDActionHandlingInterface::OnActionExecuted);
	//}
}

//AUDSkirmishGaiaAIController::AUDSkirmishGaiaAIController() {
//	UE_LOG(LogTemp, Log, TEXT("Initialized GAIA AI controller for debugging purposes!"));
//	PrimaryActorTick.bCanEverTick = false;
//}
//
//void AUDSkirmishGaiaAIController::Tick(float DeltaSeconds)
//{
//	Super::Tick(DeltaSeconds);
//	UE_LOG(LogTemp, Log, TEXT("AI DID NOTHING ON THIS TICK!"));
//	const auto& gm = Cast<AUDSkirmishGameMode>(GetWorld()->GetAuthGameMode());
//	gm->WorldSimulation->ExecuteAction()
//}
