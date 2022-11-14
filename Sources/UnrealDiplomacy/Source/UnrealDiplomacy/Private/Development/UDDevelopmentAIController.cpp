// Copyright Miroslav Valach


#include "Development/UDDevelopmentAIController.h"

AUDDevelopmentAIController::AUDDevelopmentAIController() {
	UE_LOG(LogTemp, Log, TEXT("Initialized AI controller for debugging purposes!"));
	PrimaryActorTick.bCanEverTick = false;
}

void AUDDevelopmentAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Log, TEXT("AI DID NOTHING ON THIS TICK!"));
}
