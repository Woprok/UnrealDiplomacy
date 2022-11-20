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
