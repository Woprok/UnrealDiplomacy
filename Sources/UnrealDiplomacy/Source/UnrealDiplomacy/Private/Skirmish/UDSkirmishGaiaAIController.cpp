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
	GaiaState = authority->GetSpecificState(GetControllerUniqueId());
}

void AUDSkirmishGaiaAIController::OnActionExecuted(FUDActionData& executedAction)
{
	// Simulator called this after action was executed
	// We have to pass our action if its our turn
	// We are quite lazy so let's do the simplest one for now.

	// After start of the game do some exclusive actions.
	if (executedAction.ActionTypeId == UUDStartGameAction::ActionTypeId)
	{
		FUDActionData createResources(UUDGenerateIncomeAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(createResources);
		return;
	}

	// End turn once income was generated for the first time.
	if (executedAction.ActionTypeId == UUDGenerateIncomeAction::ActionTypeId)
	{
		FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(endTurn);
		return;
	}
	
	// End turn automatically as there is no other action to await for.
	if (executedAction.ActionTypeId == UUDEndTurnAction::ActionTypeId && GaiaState->CurrentTurnPlayerId == GetControllerUniqueId())
	{
		FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(endTurn);
		return;
	}
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
